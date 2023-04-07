#ifndef SPRITE_H
#define SPRITE_H

#include <gsl/gsl>

class Sprite {
   public:
    Sprite(
        gsl::span<const uint16_t> pixel_data, uint16_t x_position, uint16_t y_position, uint16_t width, uint16_t height,
        uint16_t transparent_pixel);
    void update_position(uint16_t x_position, uint16_t y_position);
    gsl::span<const uint16_t> get_pixel_data() const;
    uint16_t get_x_position() const;
    uint16_t get_y_position() const;
    uint16_t get_width() const;
    uint16_t get_height() const;
    uint16_t get_transparent_pixel() const;

   private:
    gsl::span<const uint16_t> pixel_data;
    uint16_t x_position;
    uint16_t y_position;
    uint16_t width;
    uint16_t height;
    uint16_t transparent_pixel;
};

#endif