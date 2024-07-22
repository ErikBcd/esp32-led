#pragma once

#include <AsyncUDP.h>
#include <cstdint>
#include <FastLED.h>
#include <vector>

#include "LED_state.h"

/** Stores the most important contents of a message. */
struct led_message_container {
    uint8_t * data;
    size_t length;
    bool broadcast;
};

/** Possible operators a message can have. */
enum msg_operator {
        CHANGE_MODE='1', CHANGE_COLOR, TURN_OFF, TOGGLE_SYNC, WELCOME
};

class led_message {
private:
public:
    /** Creates a new welcome message containing info about this device, as documented in the documentation. */
    static AsyncUDPMessage create_message_welcome(uint8_t mode, CRGB colors[3], uint16_t num_led, std::vector<MODE> modes);

    /** Parses a given message and changes up the LED_State accordingly. */
    static void parse_message(led_message_container msg, LED_State *state);
};