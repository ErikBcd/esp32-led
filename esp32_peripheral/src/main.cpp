#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncUDP.h>
#include <FastLED.h>

#include "LED_state.h"
#include "LED_Message.h"

#define UDP_LISTEN_PORT 6969 

/** Connection parameters for connecting to the central. */
const char *ssid = "ledserver";
const char *password = "suchSecret";

/** The UDP server. */
AsyncUDP udp;

/** Tells the program if a new packet has arrived since the last time it checked. */
bool newPacket = false;
/** Holds the last packet we got. We don't want to store too much, one packet has to suffice. */
led_message_container packetBuffer;

/** The current state of the LEDs. */
LED_State led_state;

void packet_handler(AsyncUDPPacket packet) {
#ifdef DEBUG
  // Print the packet contents
  Serial.print("UDP Packet Type: ");
  Serial.print(packet.isBroadcast() ? "Broadcast" : packet.isMulticast() ? "Multicast" : "Unicast");
  Serial.print(", From: ");
  Serial.print(packet.remoteIP());
  Serial.print(":");
  Serial.print(packet.remotePort());
  Serial.print(", To: ");
  Serial.print(packet.localIP());
  Serial.print(":");
  Serial.print(packet.localPort());
  Serial.print(", Length: ");
  Serial.print(packet.length()); 
  Serial.print(" , Data: ");
  Serial.printf("%d  - or full: ", packet.data()[0]);
  if (packet.length() ==11) {
    for (size_t i = 0; i < 11; i++) {
      Serial.printf("%d, ", packet.data()[i]);
    }
  }
  Serial.println();
#endif

  // We ignore ANY packets that aren't welcome packets while we are not connected!
  if (!led_state.connected && packet.data()[0] != WELCOME) {
    return;
  }
  // Delete old data and save the new packet
  delete[] packetBuffer.data;
  packetBuffer.data = (uint8_t*) malloc(packet.length());
  memcpy(packetBuffer.data, packet.data(), packet.length());
  packetBuffer.broadcast = packet.isBroadcast();
  packetBuffer.length = packet.length();

  newPacket = true;
}

void setup() {
  /* NETWORK SETUP */
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());
  delay(200);
  
  if (udp.listen(UDP_LISTEN_PORT)) {
    Serial.print("UDP Listening on IP: ");
    Serial.println(WiFi.localIP());
    udp.onPacket(packet_handler);
  }
  Serial.println("Setting led state");
  led_state = LED_State();

  // Setup FastLED to listen to the leds of the led_state when refreshing
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(led_state.leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(led_state.getBrightness());
}

/**
 * Checks if we disconnected in some way and waits until we established a connection.
*/
void check_reconnect() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(10);
    led_state.connected = false;
    /*
    led_state.change_mode(MODE::STATIC);
    CRGB colors[3] = {CRGB::AliceBlue, CRGB::AliceBlue, CRGB::AliceBlue};
    led_state.set_colors(colors);
    */
    led_state.change_mode(MODE::OFF);
    led_state.update();
  }

  // Case: We are connected to the WiFi, but no connection to the central 
  //       led-manager has been made.
  while (WiFi.status() == WL_CONNECTED && !led_state.connected) {
    AsyncUDPMessage welcomeMsg = 
        led_message::create_message_welcome(
          led_state.getMode(), 
          led_state.get_main_colors(), 
          NUM_LEDS, 
          led_state.supported_modes);

    udp.sendTo(welcomeMsg, WiFi.gatewayIP(), UDP_LISTEN_PORT);
#ifdef DEBUG
    Serial.printf("Sending: L=%d'c= %d %d %d %d\n", welcomeMsg.length(), 
        welcomeMsg.data()[0],
        welcomeMsg.data()[1],
        welcomeMsg.data()[2],
        welcomeMsg.data()[3]);
#endif
    // wait a little for any new packets coming in.
    delay(200);
    if (newPacket) {
      led_message::parse_message(packetBuffer, &led_state);
      newPacket = false;
    }
    // Wait longer, we don't want to spam with welcome messages in case the central 
    // has a lot to deal with (don't we all?)
    delay(3000);
  }
}

void loop() {
  // Handle new packet if needed.
  if (newPacket) {
    Serial.println("Parsing message!");
    led_message::parse_message(packetBuffer, &led_state);
    newPacket = false;
  }

  check_reconnect();

  led_state.update();
}