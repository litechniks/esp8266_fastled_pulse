#include <FastLED.h>
#include "pulse.h"


#define NUM_LEDS 60 //min 12 pcs
#define DATA_PIN 5
#define CLOCK_PIN 13

CRGB global_leds[NUM_LEDS];
uint32_t system_time;

ledPulseArray pulsearray;
String debug;

void setup() {
  // put your setup code here, to run once:
  delay(3000);
  Serial.begin(115200);
  FastLED.addLeds<WS2813, DATA_PIN, RGB>(global_leds, NUM_LEDS);

}

void loop() {
  pulsearray.findInactivePulse();
  pulsearray.calcNextState();
  pulsearray.updateLedOutput();
  pulsearray.checkIfNewPulseNeeded();
  if ((millis() - system_time) > 100) {
    debug = "";
    for (int i = 0; i < NUM_LEDS; i++) {
      debug += String(global_leds[i].red) + " ";

    }
    Serial.println(debug);
    system_time = millis();
  }
}
