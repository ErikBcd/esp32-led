#pragma once

#include <FastLED.h>
#include <vector>

#define NUM_LEDS 5
#define DATA_PIN 4
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define DEFAULT_BRIGHTNESS 96
#define FRAMES_PER_SECOND 120

/** All modes that can be available for devices. Does not mean this device supports all of these. */
enum MODE
{
        OFF = 'A',
        STATIC,
        PRIDE,
        RAINBOW,
        FLICKER,
        BLEND
};

/** Holds the current state of the LEDs and provides functions for the modes. */
class LED_State
{
private:
        /** Just turns the LEDs off.*/
        void mode_off();
        /** Sets all the LEDs to the same color. */
        void mode_static();
        void rainbow();
        void flicker();
        void pride();
        void blend();

        /** Main colors the different mode functions can work with. */
        CRGB main_colors[3];
        /** A hue that can be used by effects that don't use all colors. */
        uint8_t hue = 0;
        /** The current brightness of the LED strip. */
        uint8_t _brightness = DEFAULT_BRIGHTNESS;
        /** The mode the device is in right now.*/
        MODE mode = MODE::OFF;

        // Some neat color (groups)
        CRGB gay[6] = {
                CRGB(240, 0, 1), 
                CRGB(255, 127, 0), 
                CRGB(255, 255, 0), 
                CRGB(0, 121, 64), 
                CRGB(64, 65, 254), 
                CRGB(160, 1, 192)
        };

        CRGB trans[5] = {
                CRGB(91, 207, 250),
                CRGB(245, 171, 185),
                CRGB(255, 255, 255),
                CRGB(245, 171, 185),
                CRGB(91, 207, 250)
        };

        CRGB lesbian[7] = {
                CRGB(215, 44, 0), 
                CRGB(240, 116, 42), 
                CRGB(255, 154, 87), 
                CRGB(236, 237, 234), 
                CRGB(209, 98, 166), 
                CRGB(183, 85, 146),
                CRGB(165, 1, 98)
        };

        CRGB bi[5] = {
                CRGB(215, 2, 112),
                CRGB(215, 2, 112),
                CRGB(115, 79, 150),
                CRGB(0, 56, 168),
                CRGB(0, 56, 168)
        };
public:
        LED_State();
        /** Contains the colors of the LEDs of the LED strip. */
        CRGB leds[NUM_LEDS];

        /**
         * Updates the LEDs.
         * This will execute the function linked to the current mode.
         */
        void update();

        /**
         * Set the current colors.
         */
        void set_colors(CRGB colors[3]);
        /**
         * Set the current brightness of the LEDs
         */
        void set_brightness(uint8_t brightness);
        /**
         * Set the current mode of the LEDs
         */
        void change_mode(MODE m);

        /** List of supported modes on this device. Is filled at instantiation. */
        std::vector<MODE> supported_modes;
        /** Gets the currently active mode. */
        MODE getMode();
        /** Gets an array of the current main colors. */
        CRGB *get_main_colors();
        /** Gets the current brightness of the strip. */
        uint8_t getBrightness();

        /** Is the device currently synced to all other devices in the network? */
        bool synced = true;
        /** Is the device connected to a network? */
        bool connected = false;
};
