#include "../LED_state.h"

LED_State::LED_State()
{
        supported_modes.push_back(MODE::OFF);
        supported_modes.push_back(MODE::STATIC);
        supported_modes.push_back(MODE::PRIDE);
        supported_modes.push_back(MODE::RAINBOW);
        supported_modes.push_back(MODE::FLICKER);
        supported_modes.push_back(MODE::BLEND);
}

void LED_State::update()
{
        switch (mode)
        {
        case MODE::OFF:
                mode_off();
                break;
        case MODE::STATIC:
                mode_static();
                break;
        case MODE::RAINBOW:
                rainbow();
                break;
        case MODE::FLICKER:
                flicker();
                break;
        case MODE::PRIDE:
                pride();
                break;
        case MODE::BLEND:
                blend();
                break;
        default:
                break;
        }

        hue++;

        FastLED.show();
        FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void LED_State::set_brightness(uint8_t brightness)
{
        _brightness = brightness;
        FastLED.setBrightness(_brightness);
        update();
}

void LED_State::set_colors(CRGB colors[3])
{
        main_colors[0] = CRGB(colors[0]);
        main_colors[1] = CRGB(colors[1]);
        main_colors[2] = CRGB(colors[2]);
        if (_brightness == 0)
        {
                set_brightness(DEFAULT_BRIGHTNESS);
        }
        update();
}

void LED_State::change_mode(MODE m)
{
        // check if mode is supported
        bool supported = false;
        for (MODE ms : supported_modes) {
                if (ms == m) {
                        supported = true;
                        break;
                }
        }
        if (!supported) { return; }
        mode = m;
        if (_brightness == 0 || FastLED.getBrightness() == 0)
        {
                set_brightness(DEFAULT_BRIGHTNESS);
        }
        update();
}

void LED_State::mode_off()
{
        FastLED.setBrightness(0);
}

void LED_State::mode_static()
{
        fill_solid(leds, NUM_LEDS, main_colors[0]);
}

void LED_State::rainbow()
{
        fill_rainbow(leds, NUM_LEDS, hue, 7);
}

void LED_State::flicker()
{
        fadeToBlackBy( leds, NUM_LEDS, 10);
        int pos = random16(NUM_LEDS);
        leds[pos] += CHSV( hue + random8(64), 200, 255);
}

void LED_State::blend() {
        static uint8_t active = 0;
        // interpolate between this and the next color
        CHSV current_hue = rgb2hsv_approximate(main_colors[active%3]);
        CHSV next_hue = rgb2hsv_approximate(main_colors[(active+1)%3]);
        double norm_delta_h = ((double) (next_hue.h - current_hue.h)) / 255.0;
        double norm_delta_s = ((double) (next_hue.s - current_hue.s)) / 255.0;
        double norm_delta_v = ((double) (next_hue.v - current_hue.v)) / 255.0;
        current_hue.h += hue * norm_delta_h;
        current_hue.s += hue * norm_delta_s;
        current_hue.v += hue * norm_delta_v;
        fill_solid(leds, NUM_LEDS, current_hue);
        if (hue == 255) {
                active++;
        }
}

void LED_State::pride()
{       
        static uint8_t mode = 0;
        switch (mode % 4)
        {
        case 0: // the gay
                for (size_t i = 0; i < NUM_LEDS; i++) {
                        leds[i] = gay[i%6];
                }
                break;
        case 1: // le lesbian
                for (size_t i = 0; i < NUM_LEDS; i++) {
                        leds[i] = lesbian[i%7];
                }
                break;
        case 2: // le bi
                for (size_t i = 0; i < NUM_LEDS; i++) {
                        leds[i] = bi[i%5];
                }
                break;
        case 3: // le trans
                for (size_t i = 0; i < NUM_LEDS; i++) {
                        leds[i] = trans[i%5];
                }
                break;
        default:
                break;
        }
        if (hue == 255) {
                mode++;
        }
}

MODE LED_State::getMode()
{
        return mode;
}
CRGB *LED_State::get_main_colors()
{
        return main_colors;
}
uint8_t LED_State::getBrightness()
{
        return _brightness;
}