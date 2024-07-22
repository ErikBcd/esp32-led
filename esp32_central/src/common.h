#pragma once
#include <WiFi.h>

#define UDP_LED_PORT 6969

/** Operators of a message. Starting at char 1 (49)*/
enum msg_operator
{
        CHANGE_MODE = '1',
        CHANGE_COLOR,
        TURN_OFF,
        TOGGLE_SYNC,
        WELCOME
};

/**
 * Contains possible modes of devices.
*/
enum MODE
{
        OFF = 'A',
        STATIC,
        PRIDE,
        RAINBOW,
        FLICKER,
        BLEND
};

// For boundary checks if a given mode is actually in this enum
#define FIRST_MODE OFF
#define LAST_MODE BLEND

struct _udp_packet
{
        uint8_t *data;
        size_t length;
        IPAddress localIP;
};