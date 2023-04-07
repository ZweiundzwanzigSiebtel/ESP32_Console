#include "../include/game/sprite.h"

Sprite::Sprite(
    gsl::span<const uint16_t> pixel_data, uint16_t x_position, uint16_t y_position, uint16_t z_value, uint16_t width, uint16_t height,
    uint16_t transparent_pixel)
    : pixel_data(pixel_data),
      x_position(x_position),
      y_position(y_position),
      z_value(z_value),
      width(width),
      height(height),
      transparent_pixel(transparent_pixel) {
    Ensures(pixel_data.size() == width * height);
}

void Sprite::update_position(uint16_t x_position, uint16_t y_position) {
    this->x_position = x_position;
    this->y_position = y_position;
}

gsl::span<const uint16_t> Sprite::get_pixel_data() const {
    return pixel_data;
}

uint16_t Sprite::get_x_position() const {
    return x_position;
}

uint16_t Sprite::get_y_position() const {
    return y_position;
}

uint16_t Sprite::get_z_value() const {
    return z_value;
}

uint16_t Sprite::get_width() const {
    return width;
}

uint16_t Sprite::get_height() const {
    return height;
}

uint16_t Sprite::get_transparent_pixel() const {
    return transparent_pixel;
}
