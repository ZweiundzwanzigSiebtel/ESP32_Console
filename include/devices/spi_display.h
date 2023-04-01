#ifndef SCREEN_H
#define SCREEN_H

#include <cstdint>
#include <memory>

#include "abstract_display.h"
#include "communication/spi.h"


class Display : public AbstractDisplay {
    public:
    Display(Spi& spi);
    ~Display() override;
    void set_background_color(int color) override;
    void draw_pixel(Point point, int color) override;
    void draw_line(Point start, Point end, int color);
    void draw_rectangle(Point upperLeft, Point lowerRight, int color);

    private:
    Spi& spi;
    // Pins für SPI-Verbindung zum Display
    const int TFT_CS = 7;
    const int TFT_DC = 18;
    const int TFT_RST = 19;

    // Größe des Displays
    const uint16_t TFT_WIDTH = 240;
    const uint16_t TFT_HEIGHT = 320;
    
    void reset();
    void write_command(std::vector<uint8_t> command);
    void write_data(std::vector<uint8_t> data);
    void write_command(uint8_t command);
    void write_data(uint8_t data);
    void set_cursor(Point point);
    void write_pixel(int color);
};

#endif