/*
   Effect for WS2815 LED strips - multiple pulses heading the same direction but at different speeds and different hues.
   The program has been created for optimal memory usage.

   Tested on ESP8266

   Credits: litechniks (@github)
*/

#include <FastLED.h>
#include "pulse.h"


#define NUM_LEDS 60           //Number of LEDs in the strip, min 12 pcs
#define DATA_PIN 5            //Data pin on the controller
#define CLOCK_PIN 13          //Not needed for WS2815
#define DEBUG                 //To see a representation of the pulses in the Serial Monitor (ruins performance)

CRGB global_leds[NUM_LEDS];   //global variable that holds the values of the LEDs
uint32_t system_time;         //used for timing of th debugging mesages

ledPulseArray pulsearray;     //array of light pulses
String debug;

void setup() {
  // put your setup code here, to run once:
  delay(3000);
  Serial.begin(115200);
  FastLED.addLeds<WS2813, DATA_PIN, RGB>(global_leds, NUM_LEDS);

}

void loop() {
  pulsearray.checkIfNewPulseNeeded();
  pulsearray.findInactivePulse();
  pulsearray.calcNextState();
  pulsearray.updateLedOutput();
#ifdef DEBUG
  if ((millis() - system_time) > 100) {
    debug = "";
    for (int i = 0; i < NUM_LEDS; i++) {
      debug += String(global_leds[i].red) + " ";
    }
    Serial.println(debug);
    system_time = millis();
  }
#endif
}
