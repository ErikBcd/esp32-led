#pragma once
#include "led_manager.h"

#include <WiFi.h>
#include <ESPAsyncWebServer.h>

class WebsiteHandler
{
private:
        AsyncWebServer *webserver;
        /** LED Manager we send LED_Commands to. */
        LED_Manager *manager;
        bool params_exist(AsyncWebServerRequest *request, String *params, size_t length);
        /**
         * Requesthandler for web requests.
         */
        void onRequest(AsyncWebServerRequest *request);
        void off();
        void color(uint8_t r, uint8_t g, uint8_t b);
        void color(CRGB colors[3]);
        void mode_change(uint8_t mode);
        void mode_change(uint8_t mode, CRGB colors[3]);
        void mode_change(uint8_t mode, uint8_t r, uint8_t g, uint8_t b);
public:
        WebsiteHandler(AsyncWebServer *server, LED_Manager *mngr);
};
