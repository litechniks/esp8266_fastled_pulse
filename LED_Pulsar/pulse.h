#define PULSE_NO 5
#define LED_RATE 5

#ifndef pulse_H
#define pulse_H

class ledPulse {
  public:
    struct HSV_struct
    {
      uint16_t hue;
      uint16_t saturation;
      uint16_t brightness;
    };
    HSV_struct HSV;
    uint16_t brightness_step;

    uint16_t pos;
    int16_t start;
    int16_t end;
    uint16_t length;
    uint16_t time_step;
    uint32_t last_update_time;
    bool marked_active;
    uint8_t pulse_array_index;

    ledPulse(void);
    void setActive(void);
    void calcPulseIntBoundaries(void);
    void checkPulsePosition(void);
    void addValuesToLedArray(void);
    void updatePulse(void);
    void reinitPulse(void);

};

class ledPulseArray {
  public:
    ledPulse pulseArray[PULSE_NO];
    uint8_t no_of_pulses;
    int8_t active_pulses[PULSE_NO];
    bool marked_recalc;
    uint32_t lastLedUpdate;
    uint32_t lastTimePulseAdded;
    uint32_t nextPulseStep;

    ledPulseArray(void);
    void createNewPulse(void);
    void findInactivePulse(void);
    void removePulse(uint8_t index);
    void calcNextState(void);
    void updateLedOutput(void);
    void checkIfNewPulseNeeded(void);
    void debugPositions(void);
};
#endif
