#pragma once

#include <WiFi.h>
#include <AsyncUDP.h>

#include "led_manager.h"

#define MAX_BUFFER_SIZE 10

class UDP_handler
{
private:
        /** Manager that controls the LEDs. */
        LED_Manager *manager;
        /** Buffer that holds the last n received packets. */
        _udp_packet _packetBuffer[MAX_BUFFER_SIZE];
        /** Number of active items in the buffer. */
        size_t buffer_index_max = 0;
        /** Location to store next incoming packet in. */
        size_t buffer_index = 0;
        /** Semaphore for _packeyBuffer access. */
        bool lock_buffer = false;
        /** Indicates wether something was added to the buffer since the last check. */
        bool buffer_empty = true;

public:
        /** 
         * @brief Creates a new UDP Handler.
         * @param manager The LED Manager this handler is connected to.
         */
        UDP_handler(LED_Manager *manager);

        /**
         * @brief Puts the incoming AsyncUDPPacket into a shortened
         *        _udp_packet that is placed into the _packetBuffer.
         * @param packet UDP Packet
        */
        void add_to_buffer(AsyncUDPPacket packet);

        /**
         * @brief Handles all the packets that are saved in the _packetBuffer
        */
        void handle_buffer();
};
