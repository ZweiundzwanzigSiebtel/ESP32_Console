#include "Arduino.h"
#include "SPI.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include <memory>   // für std::unique_ptr
#include <cstring>  // für std::memset

#include "../include/communication/spi.h"

#define PARALLEL_LINES 16

Spi::Spi(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t cs) : SCK(sck), MISO(miso), MOSI(mosi), CS(cs) {
    SPI.begin(SCK, MISO, MOSI, CS);
    esp_err_t ret;
    spi_bus_config_t buscfg={
        .mosi_io_num=MOSI,
        .miso_io_num=MISO,
        .sclk_io_num=SCK,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
        .max_transfer_sz=PARALLEL_LINES*320*2+8
    };
    spi_device_interface_config_t devcfg = {
        .mode=0,                                //SPI mode 0
        .clock_speed_hz=10*1000*1000,           //Clock out at 10 MHz
        .spics_io_num=CS,                      //CS pin
        .queue_size=7,                          //We want to be able to queue 7 transactions at a time
    };
//    //Initialize the SPI bus
    ret=spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);
    ret=spi_bus_add_device(SPI2_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);
}


void Spi::write(uint8_t data) {
    SPI.write(data);
//    spi_transaction_t transaction = {};
//    transaction.length = 8;                   
//    transaction.tx_buffer = &data;            
//    spi_device_transmit(spi, &transaction);   
}

//void Spi::write_bytes(std::vector<uint8_t> data) {
//    esp_err_t ret;
//    static spi_transaction_t transaction[6];
//    for(int i = 0; i < 6; ++i) {
//        transaction[i].length = 8;                   
//        transaction[i].tx_buffer = &data[i];            
//    }
//    for(int i=0; i<6; ++i) {
//        ret=spi_device_queue_trans(spi, &transaction[i], 0xffffffffUL);
//        assert(ret==ESP_OK);
//    }
//}
void Spi::write_bytes(std::vector<uint8_t> data) {
    SPI.writeBytes(data.data(), data.size());
//    esp_err_t ret;
//    std::vector<spi_transaction_t> transactions(data.size());
//    for (int i = 0; i < data.size(); ++i) {
//        transactions[i].length = 8;                   
//        transactions[i].tx_buffer = &data[i];            
//    }
//    for (auto& transaction : transactions) {
//        ret = spi_device_queue_trans(spi, &transaction, 0xffffffffUL);
//        assert(ret == ESP_OK);
//        Serial.write("spi ok");
//    }
}

void Spi::select() {
}

void Spi::deselect() {
}