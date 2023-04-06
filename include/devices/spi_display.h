#ifndef SCREEN_H
#define SCREEN_H
#include "driver/spi_master.h"

#include <gsl/gsl>

class SpiDisplay {
   public:
    SpiDisplay();
    //~SpiDisplay() override;
    void render();

   private:
    spi_device_handle_t spi;
    void lcd_init();
    void send_lines(gsl::span<const uint8_t> linedata);
    void send_line_finish();
    void send_data(gsl::span<const uint8_t> data);
    void send_command(const uint8_t cmd);
    void send(gsl::span<const uint8_t> data);
};

#endif