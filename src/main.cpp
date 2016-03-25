/*
 * ESP Boilerplate OS - main.cpp
 * (c) by Christoph (doebi) Döberl
 *
 */
#include "ESPThing.h"
#include "FastLED.h"

#define NUM_LEDS 50
#define DATA_PIN 12
#define CLOCK_PIN 13

CRGB leds[NUM_LEDS];

void set_cb(const MQTT::Publish& pub) {
    if (pub.payload_len() >= NUM_LEDS) {
        String DATA = pub.payload_string();
        for (int i = 0; i < NUM_LEDS; i++) {
            char s = DATA.charAt(i);
            if (s == 'r') {
                leds[i] = CRGB::Red;
            }
            if (s == 'g') {
                leds[i] = CRGB::Green;
            }
            if (s == 'b') {
                leds[i] = CRGB::Blue;
            }
            if (s == 'w') {
                leds[i] = CRGB::White;
            }
            if (s == 'n') {
                leds[i] = CRGB::Black;
            }
        }
        FastLED.show();
    }
}

void setup() {
    FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
    Thing.addInput(Input("set", set_cb));
}

void loop() {
    Thing.loop();
}

