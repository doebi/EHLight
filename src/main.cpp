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

int mhexdec[256];
char mdechex[17] = "0123456789ABCDEF";

void init_hexdec () {
	int i = 0, n = 0;
	for (i; i < 256; ++i) {
		if ((i > 47 && i < 58) || (i > 64 && i < 71) || (i > 96 && i < 103)) {
			mhexdec[i] = n++;
		} else {
			mhexdec[i] = -1;
			if (i == 71) {
				n = 10;
			}
		}
	}
}

unsigned hexdec (const char *hex, const int s_hex) {
	if (s_hex == 0) {
		return 0;
	}
	unsigned d = 0, dv = 0, i;
	for (i = 0; i < s_hex; i++) {
		dv = mhexdec[(int) hex[i]];
		if (dv < 0) {
			return 0;
		}
		d += dv * (1 << ((s_hex-i-1) << 2));
	}
	return d;
}


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

void setrgb_cb(const MQTT::Publish& pub) {
    if (pub.payload_len() >= NUM_LEDS*6) {
        String DATA = pub.payload_string();
        int j = 0;
        for (int i = 0; i < NUM_LEDS; i++) {
            //leds[i] = CRGB((int)DATA[j],(int)DATA[j+1],(int)DATA[j+2]);
            leds[i] = CRGB(
                    hexdec(DATA.substring(j,   j+1).c_str(), 2),
                    hexdec(DATA.substring(j+2, j+3).c_str(), 2),
                    hexdec(DATA.substring(j+4, j+5).c_str(), 2)
                    );
            j += 6;
        }
        FastLED.show();
    }
}

void setup() {
    FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
    Thing.addInput(Input("set", set_cb));
    Thing.addInput(Input("setrgb", setrgb_cb));
    init_hexdec();
}

void loop() {
    Thing.loop();
}

