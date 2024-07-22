#pragma once
#include <AsyncUDP.h>
#include <cstdint>

#define CHANGE_COLORS_SIZE 11
#define CHANGE_MODE_SIZE 11
#define CHANGE_ONLY_MODE_SIZE 2
#define TURN_OFF_SIZE 1
#define TOGGLE_SYNC_SIZE 1
#define WELCOME_SIZE 1

/**
 * Helper class for creating messages.
 */
class led_message
{
private:
public:
        /**
         * @brief Create a message that tells the device to change it's colors.
         * @param colors: The colors that the device shall use
         * @param dest: uint8_t array that stores the message.
         *              Has to be big enough for the message! (use CHANGE_COLORS_SIZE for the size).
         */
        static uint8_t create_message_change_colors(
                CRGB colors[3], uint8_t dest[CHANGE_COLORS_SIZE]);
        /**
         * @brief Create a message that tells the device to change it's mode and color
         * @param mode: The mode that shall be switched to.
         * @param colors: The colors that the device shall use
         * @param dest: uint8_t array that stores the message.
         *              Has to be big enough for the message! (use CHANGE_MODE_SIZE for the size).
         */
        static uint8_t create_message_change_mode(
                uint8_t mode, CRGB colors[3], uint8_t dest[CHANGE_MODE_SIZE]);
        /**
         * @brief Create a message that tells the device to change it's mode.
         * @param mode: The mode that shall be switched to.
         * @param dest: uint8_t array that stores the message.
         *              Has to be big enough for the message! (use CHANGE_MODE_SIZE for the size).
         */  
        static uint8_t create_message_change_mode(
                uint8_t mode, uint8_t dest[CHANGE_ONLY_MODE_SIZE]);
        /**
         * @brief Create a message that tells the device to turn off.
         * @param dest: uint8_t array that stores the message.
         *              Has to be big enough for the message! (use TURN_OFF_SIZE for the size).
         */
        static uint8_t create_message_turn_off(uint8_t dest[TURN_OFF_SIZE]);

        /**
         * @brief Create a message that tells the device to toggle the sync mode.
         * @param dest: uint8_t array that stores the message.
         *              Has to be big enough for the message! (use TOGGLE_SYNC_SIZE for the size).
         */
        static uint8_t create_message_toggle_sync(uint8_t dest[TOGGLE_SYNC_SIZE]);

        /**
         * @brief Create a message that tells the device it's connected to this central.
         * @param dest: uint8_t array that stores the message.
         *              Has to be big enough for the message! (use WELCOME_SIZE for the size).
         */
        static uint8_t create_message_welcome(uint8_t dest[WELCOME_SIZE]);
};
