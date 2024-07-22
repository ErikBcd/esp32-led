#pragma once

#include <WiFi.h>
#include <AsyncUDP.h>
#include <FastLED.h>

// #include "device.h"
#include "led_message.h"
#include "common.h"

#define QUEUE_SIZE 10
#define DEVICE_MAX 20

/**
 * Stores information about one device.
*/
class Device
{
private:
public:
        /** Address of the device. */
        IPAddress device_addr;
        /** The modes that are supported by this device. */
        uint8_t supported_modes[20];
        /** Current colors */
        CRGB current_colors[3];
        /** Current mode */
        MODE current_mode = MODE::OFF;
        /** Number of LEDs this devices has (if it has as many). */
        uint16_t num_leds;

        /**
         * Creates a device according to the given information in the _udp_packet
         * Note: Packet has to conform to welcome message structure.
        */
        Device(_udp_packet *p);

        /**
         * Creates an empty device.
        */
        Device();

        /**
         * Sends the given message to this device using UDP.
         */
        int send_to(AsyncUDP *udp, uint8_t *data, size_t length);

        /**
         * Sends the given message to this device using UDP.
         */
        int send_to(AsyncUDP *udp, AsyncUDPMessage msg);
};

/** Helper for storing a command. */
struct led_command
{
        Device *to;
        uint8_t *msg = nullptr;
        size_t length;
        bool broadcast = false;
};

/**
 * @brief Manages the led devices that are connected to central device.
 *        Number of the devices that can be registered is given by DEVICE_MAX
*/
class LED_Manager
{
private:
        /** List with devices that are connected to us.*/
        Device connected_devices[DEVICE_MAX];
        /** Queue with recent commands that have to be forwarded to the devices. */
        led_command led_command_queue[QUEUE_SIZE];
        /** UDP socket used for device communication. */
        AsyncUDP *udp;

        /** Variables used for queuing led commands */
        bool lock_queue = false;
        int queue_size = 0;
        int queue_index = 0;
        bool queue_empty = true;

public:
        /**
         * Creates a new LED Manager.
         * @param udp: The udp port that will be used for communication.
         */
        LED_Manager(AsyncUDP *udp);

        /**
         * Executes every command in the command queue
         */
        void update();

        /**
         * Adds a device to the device list, as long as it isn't full.
         */
        void add_device(Device device);

        /**
         * Adds a command to the list.
         */
        void add_command(led_command cmd);

        bool device_exists(IPAddress addr);
};
