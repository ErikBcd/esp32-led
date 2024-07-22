#include "../udp_handler.h"
#include "../led_message.h"
#include "../common.h"

#include <String>

UDP_handler::UDP_handler(LED_Manager *manager)
{
        this->manager = manager;
}

void UDP_handler::add_to_buffer(AsyncUDPPacket packet)
{
        while (lock_buffer)
        {
                delay(1);
        }
        lock_buffer = true;

        uint8_t *data = (uint8_t *)malloc(packet.length());
        memcpy(data, packet.data(), packet.length());
        _udp_packet p = {
            data,
            packet.length(),
            packet.remoteIP()};
        _packetBuffer[buffer_index] = p;

        buffer_index = (buffer_index == MAX_BUFFER_SIZE - 1) ? 0 : buffer_index + 1;
        buffer_index_max += (buffer_index_max == MAX_BUFFER_SIZE - 1) ? 0 : 1;
        buffer_empty = false;
        lock_buffer = false;
}

void UDP_handler::handle_buffer()
{
        while (lock_buffer)
        {
                delay(1);
        }
        lock_buffer = true;
        if (buffer_empty)
        {
                lock_buffer = false;
                return;
        }
        for (size_t i = 0; i < buffer_index_max; i++)
        {
                // handle the packet in some way

                switch ((msg_operator)_packetBuffer[i].data[0])
                {
                case WELCOME:
                        if (_packetBuffer[i].length >= 2)
                        {
                                manager->add_device(Device(&_packetBuffer[i]));
                        }
                        break;
                default:
                        // we don't handle messages with commands for LEDs
                        break;
                }
        }
        buffer_index_max = 0;
        buffer_index = 0;
        lock_buffer = false;
        buffer_empty = true;
}