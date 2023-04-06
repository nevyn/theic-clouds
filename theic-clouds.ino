#include <M5Unified.h>
#include "FastLED.h"

static const int lstrip_count = 80;
CRGB lstrip[lstrip_count];
static const int rstrip_count = 80;
CRGB rstrip[rstrip_count];


uint8_t gHue = 0;

void setup() {
    M5.begin();
    FastLED.addLeds<WS2811, G26, GRB>(lstrip, lstrip_count);
    FastLED.addLeds<WS2811, G32, GRB>(rstrip, rstrip_count);
    FastLED.setBrightness(50);
    fill_solid(lstrip, lstrip_count, CRGB::Black);
    fill_solid(rstrip, rstrip_count, CRGB::Black);
    FastLED.show();
}

void loop() {
    M5.update();
    //fill_solid(leds, NUM_LEDS, CRGB::Black);
    
    fill_rainbow(lstrip, lstrip_count, gHue,7);
    fill_rainbow(rstrip, rstrip_count, gHue,7);


    FastLED.show();
    EVERY_N_MILLISECONDS(1) {
        gHue++;
    }
}
