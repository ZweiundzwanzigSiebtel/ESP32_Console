#include <memory>

#include "../include/communication/http_server.h"
#include "../include/communication/spi.h"
#include "../include/devices/spi_display.h"
#include "../include/game/game.h"
#include "../include/game/pong.h"
#include "../include/game/sprite.h"

#define SSID "ESP32"
#define PASSWORD "12345678"

HttpServer httpServer;

// MOSI: Pin 10
// MISO: Pin 9
// SCK: Pin 8
// SS/CS: Pin 7 (oder ein anderer Pin Ihrer Wahl, der als CS-Pin konfiguriert
// werden kann)
extern "C" {
void app_main(void) {
    //    esp_console_config_t console_config = {
    //        .max_cmdline_length = 256,
    //        .max_cmdline_args = 8,
    //    };
    //    esp_console_init(&console_config);
    SpiDisplay myClass;
    std::vector<uint16_t> pixel_data(9600, 0x0F0F);
    Sprite a(gsl::make_span(pixel_data), 0, 0, 320, 30, 0xFFFF);

    myClass.add_sprite(a);
    myClass.render();
}
}