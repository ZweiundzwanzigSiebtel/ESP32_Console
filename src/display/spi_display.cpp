#include "../include/devices/spi_display.h"
#include "../include/utils/utils.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "driver/gpio.h"
#include "esp_console.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LCD_HOST SPI2_HOST

#define PIN_NUM_MISO 9
#define PIN_NUM_MOSI 10
#define PIN_NUM_CLK 8
#define PIN_NUM_CS 7

#define PIN_NUM_DC 18
#define PIN_NUM_RST 19
#define PIN_NUM_BCKL 5

// To speed up transfers, every SPI transfer sends a bunch of lines. This define
// specifies how many. More means more memory use, but less overhead for setting
// up / finishing transfers. Make sure height is dividable by this.
#define PARALLEL_LINES 16

/*
 The LCD needs a bunch of command/argument values to be initialized. They are
 stored in this struct.
*/
typedef struct {
    uint8_t cmd;
    uint8_t data[16];
    uint8_t databytes;  // No of data in data; bit 7 = delay after set; 0xFF =
        // end of cmds.
} lcd_init_cmd_t;

typedef enum {
    LCD_TYPE_ILI = 1,
    LCD_TYPE_ST,
    LCD_TYPE_MAX,
} type_lcd_t;

// Place data into DRAM. Constant data gets placed into DROM by default, which
// is not accessible by DMA.
DRAM_ATTR static const lcd_init_cmd_t lcd_init_cmds[] = {
    /* Power contorl B, power control = 0, DC_ENA = 1 */
    {0xCF, {0x00, 0x83, 0X30}, 3},
    /* Power on sequence control,
     * cp1 keeps 1 frame, 1st frame enable
     * vcl = 0, ddvdh=3, vgh=1, vgl=2
     * DDVDH_ENH=1
     */
    {0xED, {0x64, 0x03, 0X12, 0X81}, 4},
    /* Driver timing control A,
     * non-overlap=default +1
     * EQ=default - 1, CR=default
     * pre-charge=default - 1
     */
    {0xE8, {0x85, 0x01, 0x79}, 3},
    /* Power control A, Vcore=1.6V, DDVDH=5.6V */
    {0xCB, {0x39, 0x2C, 0x00, 0x34, 0x02}, 5},
    /* Pump ratio control, DDVDH=2xVCl */
    {0xF7, {0x20}, 1},
    /* Driver timing control, all=0 unit */
    {0xEA, {0x00, 0x00}, 2},
    /* Power control 1, GVDD=4.75V */
    {0xC0, {0x26}, 1},
    /* Power control 2, DDVDH=VCl*2, VGH=VCl*7, VGL=-VCl*3 */
    {0xC1, {0x11}, 1},
    /* VCOM control 1, VCOMH=4.025V, VCOML=-0.950V */
    {0xC5, {0x35, 0x3E}, 2},
    /* VCOM control 2, VCOMH=VMH-2, VCOML=VML-2 */
    {0xC7, {0xBE}, 1},
    /* Memory access contorl, MX=MY=0, MV=1, ML=0, BGR=1, MH=0 */
    {0x36, {0x28}, 1},
    /* Pixel format, 16bits/pixel for RGB/MCU interface */
    {0x3A, {0x55}, 1},
    /* Frame rate control, f=fosc, 70Hz fps */
    {0xB1, {0x00, 0x1B}, 2},
    /* Enable 3G, disabled */
    {0xF2, {0x08}, 1},
    /* Gamma set, curve 1 */
    {0x26, {0x01}, 1},
    /* Positive gamma correction */
    {0xE0, {0x1F, 0x1A, 0x18, 0x0A, 0x0F, 0x06, 0x45, 0X87, 0x32, 0x0A, 0x07, 0x02, 0x07, 0x05, 0x00}, 15},
    /* Negative gamma correction */
    {0XE1, {0x00, 0x25, 0x27, 0x05, 0x10, 0x09, 0x3A, 0x78, 0x4D, 0x05, 0x18, 0x0D, 0x38, 0x3A, 0x1F}, 15},
    /* Column address set, SC=0, EC=0xEF */
    {0x2A, {0x00, 0x00, 0x00, 0xEF}, 4},
    /* Page address set, SP=0, EP=0x013F */
    {0x2B, {0x00, 0x00, 0x01, 0x3f}, 4},
    /* Memory write */
    {0x2C, {0}, 0},
    /* Entry mode set, Low vol detect disabled, normal display */
    {0xB7, {0x07}, 1},
    /* Display function control */
    {0xB6, {0x0A, 0x82, 0x27, 0x00}, 4},
    /* Sleep out */
    {0x11, {0}, 0x80},
    /* Display on */
    {0x29, {0}, 0x80},
    {0, {0}, 0xff},
};

SpiDisplay::SpiDisplay(int height, int width) : sprites(), height(height), width(width) {
    Expects(height > 0 && width > 0 && width >= height);
    for(int i = 0; i < 2; ++i) {
        lines[i] = static_cast<uint16_t*>(heap_caps_malloc(width * PARALLEL_LINES * sizeof(uint16_t), MALLOC_CAP_DMA));
        assert(lines[i] != NULL);
    }
    spi_device_interface_config_t devcfg = {
        .mode = 0,  // SPI mode 0
        .clock_speed_hz = 26 * 1000 * 1000,  // Clock out at 26 MHz
        .spics_io_num = PIN_NUM_CS,  // CS pin
        .queue_size = 16,  // We want to be able to queue 7 transactions at a time
        .pre_cb = NULL  // lcd_spi_pre_transfer_callback, // Specify
        // pre-transfer callback to handle D/C line
    };
    spi_bus_config_t buscfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = PARALLEL_LINES * width * 2 * 8};
    spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO);
    spi_bus_add_device(LCD_HOST, &devcfg, &spi);
    lcd_init();
}

void SpiDisplay::render() {
    // Indexes of the line currently being sent to the LCD and the line
    // we're calculating.
    int sending_line = -1;
    int calc_line = 0;

    for(int y = 0; y <= height; y += PARALLEL_LINES) {
        // Calculate a line.
        calculate_lines(gsl::make_span(lines[calc_line], width * PARALLEL_LINES), y);

        // Finish up the sending process of the previous line, if any
        if(sending_line != -1) {
            send_line_finish();
            if(y == height) {
                break;  // We're waiting for the last line to be sent.
            }
        }
        // Swap sending_line and calc_line
        sending_line = calc_line;
        calc_line = (calc_line == 1) ? 0 : 1;
        // Send the line we currently calculated.
        send_command(0x2A);
        uint8_t data[4] = {0, 0, (width) >> 8, (width)&0xff};
        send_data(gsl::make_span(data));
        //
        send_command(0x2B);
        uint8_t datab[4] = {y >> 8, y & 0xff, (y + PARALLEL_LINES) >> 8, (y + PARALLEL_LINES) & 0xff};
        send_data(gsl::make_span(datab));
        //
        send_command(0x2C);
        gpio_set_level(static_cast<gpio_num_t>(PIN_NUM_DC), 1);
        send_lines(gsl::make_span(lines[sending_line], width * PARALLEL_LINES));
        // The line set is queued up for sending now; the actual sending
        // happens in the background. We can go on to calculate the next
        // line set as long as we do not touch line[sending_line]; the
        // SPI sending process is still reading from that.
    }
}

void SpiDisplay::add_sprite(Sprite& sprite) {
    sprites.push_back(sprite);
    std::sort(sprites.begin(), sprites.end(), [](const Sprite& a, const Sprite& b) { return a.get_z_value() < b.get_z_value(); });
}

void SpiDisplay::calculate_lines(gsl::span<uint16_t> allocated_area, uint16_t from_y) {
    auto check_intersection = [from_y](const Sprite& sprite) {
        return sprite.get_y_position() <= from_y + PARALLEL_LINES && from_y <= sprite.get_y_position() + sprite.get_height();
    };
    for(int i = 0; i < allocated_area.size(); ++i) {
        allocated_area[i] = 0xF00F;
    }
    for(auto& sprite : sprites) {
        if(check_intersection(sprite)) {
            int sprite_x = sprite.get().get_x_position();
            int sprite_y = sprite.get().get_y_position();
            for(int y = from_y; y < from_y + PARALLEL_LINES; ++y) {
                int row_offset = (y - sprite_y) * sprite.get().get_width();
                for(int x = 0; x < width; ++x) {
                    if(sprite_x <= x && x < sprite_x + sprite.get().get_width() && sprite_y <= y && y < sprite_y + sprite.get().get_height()) {
                        allocated_area[(y - from_y) * width + x] = sprite.get().get_pixel_data()[row_offset + (x - sprite_x)];
                    } else {
                        allocated_area[(y - from_y) * width + x] = 0xF00F;  //render background
                    }
                }
            }
        }
    }
}

/* To send a set of lines we have to send a command, 2 data bytes, another
 * command, 2 more data bytes and another command before sending the line data
 * itself; a total of 6 transactions. (We can't put all of this in just one
 * transaction because the D/C line needs to be toggled in the middle.) This
 * routine queues these commands up as interrupt transactions so they get sent
 * faster (compared to calling spi_device_transmit several times), and at the
 * mean while the lines for next transactions can get calculated.
 */
void SpiDisplay::send_lines(gsl::span<const uint16_t> linedata) {
    esp_err_t ret;
    // Transaction descriptors. Declared static so they're not allocated on the
    // stack; we need this memory even when this function is finished because
    // the SPI driver needs access to it even while we're already calculating
    // the next line.
    static spi_transaction_t trans;

    // In theory, it's better to initialize trans and data only once and hang on
    // to the initialized variables. We allocate them on the stack, so we need
    // to re-init them each call.
    memset(&trans, 0, sizeof(spi_transaction_t));
    trans.flags = SPI_TRANS_USE_TXDATA;
    trans.tx_buffer = linedata.data();  // finally send the line data
    trans.length = linedata.size() * 16;  // width * 2 * 8 * PARALLEL_LINES;  // Data length, in bits
    trans.flags = 0;  // undo SPI_TRANS_USE_TXDATA flag

    // Queue all transactions.
    ret = spi_device_queue_trans(spi, &trans, portMAX_DELAY);
    assert(ret == ESP_OK);

    // When we are here, the SPI driver is busy (in the background) getting the
    // transactions sent. That happens mostly using DMA, so the CPU doesn't have
    // much to do here. We're not going to wait for the transaction to finish
    // because we may as well spend the time calculating the next line. When
    // that is done, we can call send_line_finish, which will wait for the
    // transfers to be done and check their status.
}

void SpiDisplay::send_line_finish() {
    spi_transaction_t* rtrans;
    esp_err_t ret;
    // Wait for all 6 transactions to be done and get back the results.
    ret = spi_device_get_trans_result(spi, &rtrans, portMAX_DELAY);
    assert(ret == ESP_OK);
    // We could inspect rtrans now if we received any info back. The LCD is
    // treated as write-only, though.
}

void SpiDisplay::lcd_init() {
    int cmd = 0;
    // Initialize non-SPI GPIOs
    gpio_config_t io_conf = {
        .pin_bit_mask = ((1ULL << PIN_NUM_DC) | (1ULL << PIN_NUM_RST) | (1ULL << PIN_NUM_BCKL)),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = (gpio_pullup_t) true,
    };
    gpio_config(&io_conf);

    // Reset the display
    gpio_set_level((gpio_num_t)PIN_NUM_RST, 0);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level((gpio_num_t)PIN_NUM_RST, 1);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    // Send all the commands
    while(lcd_init_cmds[cmd].databytes != 0xff) {
        send_command(lcd_init_cmds[cmd].cmd);
        send_data(gsl::make_span(lcd_init_cmds[cmd].data, lcd_init_cmds[cmd].databytes & 0x1F));
        if(lcd_init_cmds[cmd].databytes & 0x80) {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        cmd++;
    }
}

/* Send a command to the LCD. Uses spi_device_polling_transmit, which waits
 * until the transfer is complete.
 *
 * Since command transactions are usually small, they are handled in polling
 * mode for higher speed. The overhead of interrupt transactions is more than
 * just waiting for the transaction to complete.
 */
void SpiDisplay::send_command(const uint8_t cmd) {
    gpio_set_level(static_cast<gpio_num_t>(PIN_NUM_DC), 0);
    send(gsl::make_span(&cmd, 1));
}

/* Send data to the LCD. Uses spi_device_polling_transmit, which waits until the
 * transfer is complete.
 *
 * Since data transactions are usually small, they are handled in polling
 * mode for higher speed. The overhead of interrupt transactions is more than
 * just waiting for the transaction to complete.
 */
void SpiDisplay::send_data(gsl::span<const uint8_t> data) {
    gpio_set_level(static_cast<gpio_num_t>(PIN_NUM_DC), 1);
    send(data);
}

void SpiDisplay::send(gsl::span<const uint8_t> data) {
    if(data.size() == 0) {
        return;  // no need to send anything
    }
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));  // Zero out the transaction
    t.length = data.size() * 8;  // Command is 8 bits
    t.tx_buffer = data.data();  // The data is the cmd itself
    ret = spi_device_polling_transmit(spi, &t);  // Transmit!
    assert(ret == ESP_OK);  // Should have had no issues.
}