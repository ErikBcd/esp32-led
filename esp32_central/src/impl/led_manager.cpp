#include "../led_manager.h"
#include "../common.h"

Device::Device() {}

Device::Device(_udp_packet *p)
{
        size_t i = 1;
        for (; i < 21; i++)
        {
                if (p->data[i] == 'X')
                {
                        i--;
                        break;
                }
                supported_modes[i - 1] = p->data[i];
        }
        uint16_t num_leds_upper = p->data[i++] << 8;
        uint16_t num_leds_lower = p->data[i++];
        num_leds = num_leds_upper | num_leds_lower;

        current_mode = (MODE)(p->data[i++]);
        current_colors[0] = CRGB(p->data[i++],
                                 p->data[i++],
                                 p->data[i++]);
        current_colors[1] = CRGB(p->data[i++],
                                 p->data[i++],
                                 p->data[i++]);
        current_colors[2] = CRGB(p->data[i++],
                                 p->data[i++],
                                 p->data[i++]);
        device_addr = p->localIP;
}

int Device::send_to(AsyncUDP *udp, uint8_t *data, size_t length)
{
        AsyncUDPMessage msg = AsyncUDPMessage(length);
        msg.printf("%s", data);
        size_t err = udp->sendTo(msg, device_addr, 6969, TCPIP_ADAPTER_IF_AP);
        return err;
}

int Device::send_to(AsyncUDP *udp, AsyncUDPMessage msg)
{
        size_t err = udp->sendTo(msg, device_addr, 6969, TCPIP_ADAPTER_IF_AP);
        delay(10);
        return err;
}

LED_Manager::LED_Manager(AsyncUDP *udp)
{
        LED_Manager::udp = udp;
}

void LED_Manager::add_device(Device device)
{
        static size_t i = 0;

        // Check if we know this device already
        for (size_t x = 0; x <= i; x++)
        {
                if (connected_devices[x].device_addr == device.device_addr)
                {
                        connected_devices[x] = device;
                        uint8_t msg[WELCOME_SIZE];
                        led_message::create_message_welcome(msg);
                        device.send_to(LED_Manager::udp, msg, WELCOME_SIZE);
                        return;
                }
        }

        if (i >= DEVICE_MAX - 1)
        {
                Serial.println("Can't add another device: Device list is full!");
                return;
        }
        connected_devices[i] = device;
        i++;

        uint8_t msg[WELCOME_SIZE];
        led_message::create_message_welcome(msg);
        device.send_to(LED_Manager::udp, msg, WELCOME_SIZE);
        Serial.printf("(+) [LED_Manager::add_device] Added device. Now having %d device(s).\n", i);
}

void LED_Manager::update()
{
        // make sure that we are the only ones accessing the command queue
        while (lock_queue)
        {
                delay(1);
        }
        lock_queue = true;
        if (queue_empty)
        {
                lock_queue = false;
                return;
        }
        for (size_t i = 0; i < queue_size; i++)
        {
                if (led_command_queue[i].broadcast)
                {
                        size_t err = udp->broadcastTo(led_command_queue[i].msg, led_command_queue[i].length, UDP_LED_PORT, TCPIP_ADAPTER_IF_AP);
                }
                else
                {
                        led_command_queue[i].to->send_to(udp, led_command_queue[i].msg, led_command_queue[i].length);
                }
        }
        queue_size = 0;
        queue_index = 0;
        lock_queue = false;
}

void LED_Manager::add_command(led_command cmd)
{
        // make sure that we are the only one accessing the command queue
        while (lock_queue)
        {
                delay(1);
        }
        lock_queue = true;
        if (queue_empty)
        {
                led_command_queue[queue_index] = {};
        }

        if (led_command_queue[queue_index].msg != nullptr) {
                delete[] led_command_queue[queue_index].msg;
        }
        led_command_queue[queue_index].msg = (uint8_t *)malloc(cmd.length);
        memcpy(led_command_queue[queue_index].msg, cmd.msg, cmd.length);

        led_command_queue[queue_index].broadcast = cmd.broadcast;
        led_command_queue[queue_index].length = cmd.length;
        led_command_queue[queue_index].to = cmd.to;

        queue_index = (queue_index == QUEUE_SIZE - 1) ? 0 : queue_index + 1;
        queue_size += (queue_size == QUEUE_SIZE - 1) ? 0 : 1;
        queue_empty = false;
        lock_queue = false;
}