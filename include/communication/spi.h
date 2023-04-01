#ifndef SPI_H
#define SPI_H

#include <cstdint>
#include <cstddef>

#include <SPI.h>

#include "driver/spi_master.h"

#include "bus.h"

class Spi : public Bus {
    public:
    Spi(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t cs);

    void write(uint8_t data) override;
    void write_bytes(std::vector<uint8_t> data) override;

    void select();
    void deselect();
    
    private:
    const int SCK;
    const int MISO;
    const int MOSI;
    const int CS;
    spi_device_handle_t spi;
};

#endif