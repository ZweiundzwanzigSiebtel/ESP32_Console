#include <memory>

#include "../include/communication/http_server.h"
#include "../include/communication/spi.h"
#include "../include/devices/spi_display.h"
#include "../include/game/game.h"

#define SSID "ESP32"
#define PASSWORD "12345678"

HttpServer httpServer;

//MOSI: Pin 10
//MISO: Pin 9
//SCK: Pin 8
//SS/CS: Pin 7 (oder ein anderer Pin Ihrer Wahl, der als CS-Pin konfiguriert werden kann)
extern "C" {
    void app_main();
}

void app_main() {
//void setup() {
    Serial.begin(115200);
    //WiFi.begin(SSID, PASSWORD);
//    WiFi.softAP(SSID, PASSWORD);
//    while (WiFi.status() != WL_CONNECTED) {
//        delay(1000);
//        Serial.println("Connecting to WiFi...");
//    }
    Serial.println("Connected to WiFi");
    Serial.print("Connected to WiFi, IP address: ");
    Serial.println(WiFi.softAPIP().toString());
      
    //httpServer.start();
    
    Spi spi(8, 9, 10, 7);
    Display display(spi);
    display.set_background_color(0xFFFF);
    display.draw_line(Point{0, 0}, Point{100, 100}, 0x00FF);
    display.draw_rectangle(Point{180, 190}, Point{240, 240}, 0xF800);
    //TODO: Game game(display);
//}

}
//
//void loop() {
//        httpServer.handleClient();
//}