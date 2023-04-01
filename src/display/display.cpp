#include <SPI.h>
#include <Arduino.h>

#include "../include/communication/spi.h"
#include "../include/devices/spi_display.h"

Display::Display(Spi& spi) : spi(spi) {
    // Initialisiere die Pins
    pinMode(TFT_CS, OUTPUT);
    pinMode(TFT_DC, OUTPUT);
    pinMode(TFT_RST, OUTPUT);

    // Setze das Display zurück
    reset();

    // Initialisiere das Display
    write_command(0xEF); write_data(0x03); write_data(0x80); write_data(0x02);
    write_command(0xCF); write_data(0x00); write_data(0XC1); write_data(0X30);
    write_command(0xED); write_data(0x64); write_data(0x03); write_data(0X12); write_data(0X81);
    write_command(0xE8); write_data(0x85); write_data(0x00); write_data(0x78);
    write_command(0xCB); write_data(0x39); write_data(0x2C); write_data(0x00); write_data(0x34); write_data(0x02);
    write_command(0xF7); write_data(0x20);
    write_command(0xEA); write_data(0x00); write_data(0x00);
    write_command(0xC0); write_data(0x1B);
    write_command(0xC1); write_data(0x01);
    write_command(0xC5); write_data(0x30); write_data(0x30);
    write_command(0xC7); write_data(0XB7);
    write_command(0x36); write_data(0x48);
    write_command(0x3A); write_data(0x55);
    write_command(0xB1); write_data(0x00); write_data(0x1A);
    write_command(0xB6); write_data(0x0A); write_data(0xA2);
    write_command(0xF2); write_data(0x00);
    write_command(0x26); write_data(0x01);
    write_command(0xE0); write_data(0x0F); write_data(0x2A); write_data(0x28); write_data(0x08); write_data(0x0E); write_data(0x08);
    write_command(0xE1); write_data(0x0E); write_data(0x14); write_data(0x03); write_data(0x11); write_data(0x07); write_data(0x31);
    write_command(0x11); delay(120);
    write_command(0x29); delay(120);
    

}

Display::~Display() {
    // TODO
}

void Display::set_background_color(int color) {
    draw_rectangle(Point{0, 0}, Point{240, 320}, color);
}

void Display::draw_pixel(Point point, int color) {
  set_cursor(point);
  write_pixel(color);
}

void Display::draw_line(Point start, Point end, int color) {
    double slope = (end.y - start.y) / (end.x - start.x);
    double y_intercept = start.y - slope * start.x;
    for (int x = start.x; x <= end.x; x++) {
        int y = slope * x + y_intercept;
        draw_pixel(Point{x, y}, color);
    }
}

void Display::draw_rectangle(Point start, Point end, int color) {
    uint8_t color_msb = (color >> 8) & 0xFF;
    uint8_t color_lsb = color & 0xFF;
    uint16_t x_from = start.x;
    uint16_t x_to = end.x;
    uint16_t y_from = start.y;
    uint16_t y_to = end.y;
    write_command(0x2A);
    write_data(std::vector<uint8_t>{static_cast<uint8_t>((x_from >> 8) & 0xFF),
                                    static_cast<uint8_t>(x_from & 0xFF),
                                    static_cast<uint8_t>((x_to >> 8) & 0xFF),
                                    static_cast<uint8_t>(x_to & 0xFF)});
    write_command(0x2B);
    write_data(std::vector<uint8_t>{static_cast<uint8_t>((y_from >> 8) & 0xFF),
                                    static_cast<uint8_t>(y_from & 0xFF),
                                    static_cast<uint8_t>((y_to >> 8) & 0xFF),
                                    static_cast<uint8_t>(y_to & 0xFF)});
    write_command(0x2C);
    for(int i = 0; i < (x_to - x_from) * (y_to - y_from); i++) {
        write_data(std::vector<uint8_t>{color_msb, color_lsb});
    }
}

void Display::reset() {
    digitalWrite(TFT_RST, HIGH);
    delay(5);
    digitalWrite(TFT_RST, LOW);
    delay(20);
    digitalWrite(TFT_RST, HIGH);
    delay(150);
}

void Display::write_command(std::vector<uint8_t> command) {
    digitalWrite(TFT_DC, LOW);
    digitalWrite(TFT_CS, LOW);
    spi.write_bytes(command);
    digitalWrite(TFT_CS, HIGH);
}

void Display::write_data(std::vector<uint8_t> data) {
    digitalWrite(TFT_DC, HIGH);
    //digitalWrite(TFT_CS, LOW);
    spi.write_bytes(data);
    digitalWrite(TFT_CS, HIGH);
}

void Display::write_command(uint8_t command) {
    digitalWrite(TFT_DC, LOW);
    digitalWrite(TFT_CS, LOW);
    spi.write(command);
    digitalWrite(TFT_CS, HIGH);
}

void Display::write_data(uint8_t data) {
    digitalWrite(TFT_DC, HIGH);
    digitalWrite(TFT_CS, LOW);
    spi.write(data);
    digitalWrite(TFT_CS, HIGH);
}

void Display::set_cursor(Point point) {
    write_command(0x2A);
    write_data(std::vector<uint8_t>{ 
        static_cast<uint8_t>(point.x >> 8), 
        static_cast<uint8_t>(point.x & 0xFF),
        static_cast<uint8_t>((point.x + 1) >> 8),
        static_cast<uint8_t>((point.x + 1) & 0xFF)
    });

    write_command(0x2B);
    write_data(std::vector<uint8_t>{ 
        static_cast<uint8_t>(point.x >> 8), 
        static_cast<uint8_t>(point.x & 0xFF),
        static_cast<uint8_t>((point.x + 1) >> 8),
        static_cast<uint8_t>((point.x + 1) & 0xFF)
    });
}

void Display::write_pixel(int color) {
  write_command(0x2C);
  write_data(std::vector<uint8_t>{static_cast<uint8_t>(color >> 8),
                                  static_cast<uint8_t>(color & 0xFF)});
}
