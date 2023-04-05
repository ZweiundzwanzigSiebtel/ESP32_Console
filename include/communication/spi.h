#ifndef SPI_H
#define SPI_H

#include <cstdint>
#include <cstddef>

#include <SPI.h>

#include "driver/spi_master.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_console.h"

#include "bus.h"

class Spi : public Bus {
    public:
    Spi(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t cs);
    void write(gsl::span<const uint8_t> data) ;
    void write_bytes(gsl::span<const uint8_t> data) ;
//
    void get_transaction_result();
    
    spi_device_t *get_spi_handle();
    
//    void select();
//    void deselect();
    
    private:
    const int SCK;
    const int MISO;
    const int MOSI;
    const int CS;
};

#endif