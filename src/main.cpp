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
    std::vector<uint16_t> pixel_data(100, 0x0000);
    Sprite a(gsl::make_span(pixel_data), 0, 0, 0, 10, 10, 0x00FF);

    myClass.add_sprite(a);
    for(int i = 0; i < 240; i += 2) {
        a.update_position(i, i);
        myClass.render();
    }
}
}