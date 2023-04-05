#include "Arduino.h"
// #include "SPI.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include <memory>   // für std::unique_ptr
#include <cstring>  // für std::memset
#include <gsl/gsl>
#include "driver/spi_common.h"
#include "esp_console.h"

#include "../include/communication/spi.h"

#define PARALLEL_LINES 16
#define LCD_HOST SPI2_HOST

spi_device_handle_t spi_handle;

Spi::Spi(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t cs) : SCK(sck), MISO(miso), MOSI(mosi), CS(cs) {
    esp_err_t ret;
    spi_bus_config_t buscfg = {
        .mosi_io_num     = mosi,
        .miso_io_num     = miso,
        .sclk_io_num     = sck,
        .quadwp_io_num   = -1,
        .quadhd_io_num   = -1,
        .max_transfer_sz = PARALLEL_LINES * 320 * 2 + 8};

    spi_device_interface_config_t devcfg = {
        .mode           = 0,                 // SPI mode 0
        .clock_speed_hz = 26 * 1000 * 1000,  // Clock out at 26 MHz
        .spics_io_num   = cs,                // CS pin
        .queue_size     = 16,                // We want to be able to queue 7 transactions at a time
        .pre_cb         = NULL  // lcd_spi_pre_transfer_callback, // Specify pre-transfer callback to handle D/C line
    };
    // Initialize the SPI bus
    ret = spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);
    // Attach the LCD to the SPI bus
    ret = spi_bus_add_device(LCD_HOST, &devcfg, &spi_handle);
    printf("spi handle nach bus add device: %p\n", spi_handle);
    ESP_ERROR_CHECK(ret);
}

spi_device_t* Spi::get_spi_handle() {
    return spi_handle;
}

void Spi::write(gsl::span<const uint8_t> data) {
    if(data.size() == 0) {
        return;
    }
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));                                   // Zero out the transaction
    t.length    = data.size() * 8;                              // Command is 8 bits
    t.tx_buffer = data.data();                                  // The data is the cmd itself
    ret         = spi_device_polling_transmit(spi_handle, &t);  // Transmit!
    assert(ret == ESP_OK);                                      // Should have had no issues
}

void Spi::get_transaction_result() {
    spi_transaction_t* rtrans;
    esp_err_t ret;
    // Wait for all 6 transactions to be done and get back the results.
    ret = spi_device_get_trans_result(spi_handle, &rtrans, portMAX_DELAY);
    assert(ret == ESP_OK);
    // We could inspect rtrans now if we received any info back. The LCD is treated as write-only, though.
}

void Spi::write_bytes(gsl::span<const uint8_t> data) {
    assert(data.size() == 320 * 2 * PARALLEL_LINES);
    esp_err_t ret;
    // Transaction descriptors. Declared static so they're not allocated on the stack; we need this memory even when
    // this function is finished because the SPI driver needs access to it even while we're already calculating the next
    // line.
    static spi_transaction_t trans;

    // In theory, it's better to initialize trans and data only once and hang on to the initialized
    // variables. We allocate them on the stack, so we need to re-init them each call.
    memset(&trans, 0, sizeof(spi_transaction_t));
    trans.flags     = SPI_TRANS_USE_TXDATA;
    trans.tx_buffer = data.data();                   // finally send the line data
    trans.length    = 320 * 2 * PARALLEL_LINES * 8;  // Data length, in bits
    trans.flags     = 0;                             // undo SPI_TRANS_USE_TXDATA flag

    // Queue all transactions.
    ret = spi_device_queue_trans(spi_handle, &trans, portMAX_DELAY);
    assert(ret == ESP_OK);
}