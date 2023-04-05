#ifndef SCREEN_H
#define SCREEN_H
#include "driver/spi_master.h"

#include "abstract_display.h"

class SpiDisplay : AbstractDisplay {
   public:
    SpiDisplay();
    ~SpiDisplay() override;
    void update_screen() override;

   private:
    spi_device_handle_t spi;
    void init_ili9341();
    void send_block(uint8_t* linedata);
    void get_transmission_result();
};

#endif