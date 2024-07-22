#include "../LED_Message.h"

AsyncUDPMessage led_message::create_message_welcome(uint8_t mode, CRGB colors[3], uint16_t num_led, std::vector<MODE> modes) {
    AsyncUDPMessage msg = AsyncUDPMessage(14 + modes.size());
    msg.write(msg_operator::WELCOME);
    for (char c: modes) {
        msg.write(c);
    }
    msg.write('X'); // terminator char 
    uint8_t upper = (num_led >> 8);
    uint8_t lower = (num_led & 0xFF);
    msg.write(upper);
    msg.write(lower);
    msg.write(mode);
    msg.write(colors[0].r);
    msg.write(colors[0].g);
    msg.write(colors[0].b);
    msg.write(colors[1].r);
    msg.write(colors[1].g);
    msg.write(colors[1].b);
    msg.write(colors[2].r);
    msg.write(colors[2].g);
    msg.write(colors[2].b);

    return msg;
}

void led_message::parse_message(led_message_container msg, LED_State *state) {
    switch ((msg_operator) msg.data[0])
    {
    case msg_operator::CHANGE_MODE: {
        Serial.println("Got change of mode!");
        if (msg.broadcast && !(state->synced))
            break;
        state->change_mode((MODE) msg.data[1]);
        if (msg.length == 11) {
                CRGB c[3] = {CRGB(msg.data[2], msg.data[3], msg.data[4]),
                        CRGB(msg.data[5], msg.data[6], msg.data[7]),
                        CRGB(msg.data[8], msg.data[9], msg.data[10]) };
                state->set_colors(c);
        }
        break;
    }
    case msg_operator::CHANGE_COLOR: {
        Serial.println("Got change of colors!");
        if (msg.broadcast && !(state->synced))
            break;
        if (msg.length < 10) // check if packet seems correct
            break;
        CRGB c[3] = {CRGB(msg.data[1], msg.data[2], msg.data[3]),
                     CRGB(msg.data[4], msg.data[5], msg.data[6]),
                     CRGB(msg.data[7], msg.data[8], msg.data[9]) };
        state->set_colors(c);
        break;
    }
    case msg_operator::TURN_OFF: {
        Serial.println("Told me to turn off!");
        if (msg.broadcast && !(state->synced))
            break;
        state->change_mode(MODE::OFF);
        break;
    }
    case msg_operator::TOGGLE_SYNC: {
        Serial.println("Toggling sync!");
        state->synced = !state->synced;
        break;
        }
    case msg_operator::WELCOME:{
        Serial.println("Got welcome message!");
        state->connected = true;
        break;
    }
    default:
        break;
    }
}