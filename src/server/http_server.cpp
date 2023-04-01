#include "../include/communication/http_server.h"
#include "../include/communication/html_page.h"

HttpServer::HttpServer() : server(80) {
    server.on("/", HTTP_GET, [this](){
        server.send(200, "text/html", html_site);
      });
      
      server.on("/left", HTTP_GET, [this](){
        Serial.println("Steer Left");
        server.send(200);
      });

      server.on("/right", HTTP_GET, [this](){
        Serial.println("Steer Right");
        server.send(200);
      });
      
    server.on("/controller1", HTTP_GET, [this](){
        if (server.args() > 0) {
            String x_position = server.arg("x");
            String y_position = server.arg("y");
            Serial.print("Position des Reglers: "); 
            Serial.print(x_position);
            Serial.print(" ");
            Serial.println(y_position);
        }
        server.send(200);
      });

        server.on("/down", HTTP_GET, [this](){
        Serial.println("Steer Down");
        server.send(200);
      });
}

void HttpServer::start() {
    server.begin();
}

void HttpServer::handleClient() {
    server.handleClient();
}