#ifndef SCREEN_H
#define SCREEN_H
#include "driver/spi_master.h"

#include "abstract_display.h"

class SpiDisplay {
   public:
    SpiDisplay();
    //~SpiDisplay() override;
    void update_screen();

    void draw_pixel(uint16_t x, uint16_t y, uint16_t color);
    void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

   private:
    spi_device_handle_t spi;
    uint8_t frame_buffer[320 * 240 * 2];
    void init();
    void send_block(uint8_t* linedata);
    spi_device_handle_t begin();
    void lcd_init();
    // void send_line_finish(spi_device_handle_t);
};

#endif