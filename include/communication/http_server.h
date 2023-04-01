#ifndef SERVER_H
#define SERVER_H

#include <WiFi.h>
#include <WebServer.h>

class HttpServer {
    public:
    HttpServer();

    void start();
    void handleClient();

    private:
    WebServer server;
};


#endif