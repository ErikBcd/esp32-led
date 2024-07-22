#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncUDP.h>

#include "website_handler.h"
#include "led_manager.h"
#include "led_message.h"
#include "udp_handler.h"

#define UDP_LISTEN_PORT 6969

// ADD YOUR WIFI CREDENTIALS HERE
const char *wifi_network_ssid = "";
const char *wifi_network_password = "";

/** Credentials for soft accesspoint. Will be used by devices. */
const char *soft_ap_ssid = "ledserver";
const char *soft_ap_password = "suchSecret";

/** A webserver on port 80. Will be used to handle website. */
AsyncWebServer webserver(80);
/** UDP Socket for LED communication. */
AsyncUDP udp;

LED_Manager led_manager = LED_Manager(&udp);
WebsiteHandler webhandler = WebsiteHandler(&webserver, &led_manager);
UDP_handler udp_handler = UDP_handler(&led_manager);

void setup()
{

        Serial.begin(115200);

        WiFi.mode(WIFI_MODE_APSTA);

        WiFi.softAP(soft_ap_ssid, soft_ap_password);
        WiFi.begin(wifi_network_ssid, wifi_network_password);

        Serial.print("ESP32 IP as soft AP: ");
        Serial.println(WiFi.softAPIP());
        delay(500);
        Serial.print("ESP32 IP on the WiFi network: ");
        Serial.println(WiFi.localIP());

        /** Create all the handlers/managers we need. */
        led_manager = LED_Manager(&udp);
        webhandler = WebsiteHandler(&webserver, &led_manager);
        udp_handler = UDP_handler(&led_manager);

        /* Create the UDP listener for the LEDs. */
        if (udp.listen(UDP_LISTEN_PORT))
        {
                udp.onPacket([](AsyncUDPPacket packet)
                             { udp_handler.add_to_buffer(packet); });
        }
        webserver.begin();
}

void periodic_change(uint8_t funny_val) 
{
        // Send a new color every ~1500 seconds
        CRGB cool_colors[3] = {CRGB(funny_val, 0, 255 - funny_val), CRGB::AliceBlue, CRGB::AliceBlue};
        led_command c = {};
        uint8_t data[CHANGE_MODE_SIZE];
        led_message::create_message_change_mode('B', cool_colors, data);

        c.broadcast = true;
        c.msg = data;
        c.length = CHANGE_MODE_SIZE;
        c.to = nullptr;

        led_manager.add_command(c);
        Serial.printf("Created message: l=%d First few bytes: %d %d %d %d\n", c.length, c.msg[0], c.msg[1], c.msg[2], c.msg[3]);

}

void loop()
{
        static uint8_t funny_val = 0;
        // Handle current incoming packets
        udp_handler.handle_buffer();
        // Update LED stuff
        led_manager.update();

        funny_val++;
        //Serial.printf("Heapsize: %d\n", ESP.getFreeHeap());
        delay(500);
}
