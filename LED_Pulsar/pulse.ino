ledPulse::ledPulse() {
  HSV.hue = 0;
  HSV.saturation = 0;
  HSV.brightness = 0;
  pos = 0;
  start = 0;
  end = 0;
  length = 0;
  time_step = 0;
  last_update_time = 0;
  marked_active = false;
  marked_for_deletion = false;
  brightness_step = 0;
}

void ledPulse::setActive() {
  marked_active = true;
}

void ledPulse::calcPulseIntBoundaries() {
  if (pos < length) {
    start = (length - 1) - pos;
    end = length - 1;
  }
  else {
    if (pos < NUM_LEDS) {
      start = 0;
      end = length - 1;
    }
    else {
      start = 0;
      end = (length - 1) - (pos - NUM_LEDS + 1);
      if (end < 0) {
        marked_for_deletion = true;  //still executes once
      }
    }
  }
}

void ledPulse::addValuesToLedArray() {
  uint16_t brightness_var = 0;
  uint16_t ptr;
  for (int i = start; i <= end; i++) {
    if (i < (length / 2)) {
      brightness_var = brightness_step * (i + 1);
    }
    else {
      brightness_var = HSV.brightness - brightness_step * (i  - (length / 2));
    }
    ptr = pos - length + 1 + i;
    if (ptr < 0 || ptr >= NUM_LEDS) {
      Serial.println("LED array address ERROR!");
    }
    global_leds[ptr] += CHSV((uint8_t)HSV.hue, (uint8_t)HSV.saturation, (uint8_t)brightness_var);
  }
}

void ledPulse::updatePulse() {
  if (marked_active) {
    if ((millis() - last_update_time) > time_step ) {
      ledPulse::addValuesToLedArray();
      pos++;
      calcPulseIntBoundaries();
      last_update_time = millis();
    }
    else {
      ledPulse::addValuesToLedArray();
    }
  }
}

void ledPulse::reinitPulse() {
  HSV.hue = (uint16_t)random(0, 255);
  HSV.saturation = (uint16_t)random(10, 255);
  HSV.brightness = (uint16_t)random(100, 255);
  pos = 0;
  start = 0;
  end = 0;
  length = (uint8_t)random(3, (NUM_LEDS / 4));
  if ((length % 2) == 1) {
    length++;
  }
  time_step = (uint16_t)random(10, 200);
  last_update_time = 0;
  marked_active = false;
  marked_for_deletion = false;
  brightness_step = HSV.brightness * 2 / length;
}

ledPulseArray::ledPulseArray() {
  no_of_pulses = 0;
  lastLedUpdate = 0;
  lastTimePulseAdded = 0;
  nextPulseStep = random(200, 6000);
  for (int i = 0; i < PULSE_NO; i++ ) {
    active_pulses[i] = -1;
  }
  marked_recalc = true;
}

void ledPulseArray::createNewPulse() {
  bool quit = false;
  int i = 0;
  if (no_of_pulses < (PULSE_NO)) {
    while (!quit) {
      if (active_pulses[i] == -1) {
        quit = true;
      }
      else {
        i++;
      }
    }
    active_pulses[i] = 1;
    pulseArray[i].reinitPulse();
    pulseArray[i].calcPulseIntBoundaries();
    pulseArray[i].setActive();
    no_of_pulses++;
  }
}

void ledPulseArray::removePulse(uint8_t index) {
  active_pulses[index] = -1;
  no_of_pulses--;
  
}

void ledPulseArray::findInactivePulse() {
  for (int i = 0; i < PULSE_NO; i++ ) {
    if (pulseArray[i].marked_for_deletion) {
      pulseArray[i].marked_for_deletion = false;
      pulseArray[i].marked_active = false;
      removePulse(i);
    }
  }
}

void ledPulseArray::calcNextState() {
  if (marked_recalc) {
    for (int i = 0; i < NUM_LEDS; i++) {
      global_leds[i] = CHSV(0, 0, 0);
    }
    for (int i = 0; i < PULSE_NO; i++) {
      pulseArray[i].updatePulse();
    }
    marked_recalc = false;
  }
}

void ledPulseArray::updateLedOutput() {
  if ((millis() - lastLedUpdate) > LED_RATE) {
    FastLED.show();
    marked_recalc = true;
    lastLedUpdate = millis();
  }
  else {
    delay(1);
  }
}

void ledPulseArray::checkIfNewPulseNeeded() {
  if ((millis() - lastTimePulseAdded) > nextPulseStep ) {
    createNewPulse();
    nextPulseStep = random(200, 5000);
    lastTimePulseAdded = millis();
  }
  if (no_of_pulses == 0) {
    createNewPulse();
  }
}
