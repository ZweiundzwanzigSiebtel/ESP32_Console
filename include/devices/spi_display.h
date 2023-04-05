#ifndef SCREEN_H
#define SCREEN_H
#include "driver/spi_master.h"

class SpiDisplay {
   public:
    SpiDisplay();
    //~SpiDisplay() override;
    void update_screen();

   private:
    spi_device_handle_t spi;
    // uint8_t frame_buffer[320 * 240 * 2] = {0};
    void send_block(uint8_t* linedata);
    spi_device_handle_t begin();
    void lcd_init();
    // void send_line_finish(spi_device_handle_t);
};

#endif