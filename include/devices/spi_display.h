#ifndef SCREEN_H
#define SCREEN_H
#include "driver/spi_master.h"

#include "../include/game/sprite.h"

#include <gsl/gsl>
#include <vector>
#include <array>

class SpiDisplay {
   public:
    SpiDisplay(int height, int width);
    //~SpiDisplay() override;
    /**
     * @brief Render all sprites that were added with SpiDisplay::add_sprite()
     * @note This function is blocking. It will not return until the display has been updated.
     */
    void render();

    /**
     * @brief Add a sprite. On the next call to SpiDisplay::render() this sprite is rendered.
     * @param sprite The sprite to add
     * @note The sprites are sorted based on their y position. Sprites with a lower y position are rendered first.
     */
    void add_sprite(Sprite& sprite);

   private:
    spi_device_handle_t spi;
    std::vector<std::reference_wrapper<Sprite>> sprites;
    std::array<uint16_t*, 2> lines;
    const int height;
    const int width;

    void lcd_init();
    void calculate_lines(gsl::span<uint16_t> allocated_area, uint16_t from_y);
    void send_lines(gsl::span<const uint16_t> linedata);
    void send_line_finish();
    void send_data(gsl::span<const uint8_t> data);
    void send_command(const uint8_t cmd);
    void send(gsl::span<const uint8_t> data);
};

#endif