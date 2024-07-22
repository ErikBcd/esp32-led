#include <FastLED.h>
#include "../led_message.h"
#include "../common.h"

uint8_t led_message::
    create_message_change_mode(uint8_t mode, CRGB colors[3], uint8_t dest[CHANGE_MODE_SIZE])
{
        dest[0] = CHANGE_MODE;
        dest[1] = mode;

        dest[2] = colors[0].r;
        dest[3] = colors[0].g;
        dest[4] = colors[0].b;

        dest[5] = colors[1].r;
        dest[6] = colors[1].g;
        dest[7] = colors[1].b;

        dest[8] = colors[2].r;
        dest[9] = colors[2].g;
        dest[10] = colors[2].b;

        return 0;
}

uint8_t led_message::
        create_message_change_mode(uint8_t mode, uint8_t dest[CHANGE_ONLY_MODE_SIZE]) 
{
        dest[0] = CHANGE_MODE;
        dest[1] = mode;
        return 0;
}

uint8_t led_message::
    create_message_change_colors(CRGB colors[3], uint8_t dest[CHANGE_COLORS_SIZE])
{
        dest[0] = CHANGE_COLOR;
        dest[1] = colors[0].r;
        dest[2] = colors[0].g;
        dest[3] = colors[0].b;

        dest[4] = colors[1].r;
        dest[5] = colors[1].g;
        dest[6] = colors[1].b;

        dest[7] = colors[2].r;
        dest[8] = colors[2].g;
        dest[9] = colors[2].b;

        dest[10] = '\r';
        return 0;
}

uint8_t led_message::
    create_message_turn_off(uint8_t dest[TURN_OFF_SIZE])
{
        dest[0] = TURN_OFF;
        return 0;
}

uint8_t led_message::
    create_message_toggle_sync(uint8_t dest[TOGGLE_SYNC_SIZE])
{

        dest[0] = TOGGLE_SYNC;
        return 0;
}

uint8_t led_message::
    create_message_welcome(uint8_t dest[WELCOME_SIZE])
{

        dest[0] = WELCOME;
        Serial.println("(+) [led_message::create_message_welcome] Done.");
        return 0;
}