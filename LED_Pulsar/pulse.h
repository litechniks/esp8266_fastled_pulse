/*
 * This code implement a single light pulse (ledPulse) and the array of pulses (ledPulseArray)
 * To minimize memory usage the pulse brightness info is not stored in an array but calculated in each iteration.
 * The virtual pulse array is defined by position (right-most element), start (first element to be displayed inside the array)
 * and end (last element to be displayed inside the array). The array is adjusted so it is always an even number.
 * 
 * For example:
 * LED Array
 * 0 1 2 3 4 5 6 7 8 9 1 1 1 1 1 1
 *                     0 1 2 3 4 5
 *                     
 * Virtual Pulse Array (length: 4)
 * 3
 * 2 3
 * 1 2 3
 * 0 1 2 3 
 *   0 1 2 3
 *             ...
 *                         0 1 2 3
 *                           0 1 2
 *                             0 1
 *                               0
 * 
 * 
 * 
 * Credits: litechniks (@github)
 */



#define PULSE_NO 10       //maximum number of simultaneous light pulses
#define LED_RATE 5        //LED update rate

#ifndef pulse_H
#define pulse_H

// Class related to a single light pulse
class ledPulse {
  public:
    // Using the HSV model to minimize calculations - only brightness is iterated on
    struct HSV_struct
    {
      uint16_t hue;
      uint16_t saturation;
      uint16_t brightness;
    };
    HSV_struct HSV;
    // Brightness step is calculated during instantiation of the object to minimize calculations
    uint16_t brightness_step;
    // Leading edge of a pulse. Range 0..(NUM_LEDS + (length - 1))
    uint16_t pos;
    // Start of the light pulse - the index of the virtual pulse array 
    int16_t start;
    // End of the light pulse - the index of the virtual pulse array 
    int16_t end;
    // Length of the virtual ligth pulse array
    uint16_t length;
    // Speed of the light pulse
    uint16_t time_step;
    uint32_t last_update_time;
    bool marked_active;
    bool marked_for_deletion;
    //uint8_t pulse_array_index;

    ledPulse(void);
    // All pulses are instantiated at the beginning of the application so we are switching them active/disabled
    void setActive(void);
    // To check if the pulse went off the LED strip, also to calculate start and end indexes if at the edge of the LED strip
    void calcPulseIntBoundaries(void);
    // Adding the calculated values to the LED global
    void addValuesToLedArray(void);
    // Encapsulating other functions to simlifz calling
    void updatePulse(void);
    // Whenever a new pulse is added to the pulse array
    void reinitPulse(void);

};

//Class related to an array of light pulses
class ledPulseArray {
  public:
    // Array of light pulses
    ledPulse pulseArray[PULSE_NO];
    // To track the number of active pulses
    uint8_t no_of_pulses;
    // Array to track which pulses are active
    int8_t active_pulses[PULSE_NO];
    // Flag to show if the array needs to be recalculated
    bool marked_recalc;
    uint32_t lastLedUpdate;
    uint32_t lastTimePulseAdded;
    uint32_t nextPulseStep;

    ledPulseArray(void);
    // Create a new light pulse and register it in the active_pulses
    void createNewPulse(void);
    // Find pulses marked for deletion
    void findInactivePulse(void);
    // Remove the inactive pulse
    void removePulse(uint8_t index);
    // Calculate next state for all pulses and leds
    void calcNextState(void);
    // Actually output the LED array
    void updateLedOutput(void);
    // Generate new pulse after random time or if there are zero pulses
    void checkIfNewPulseNeeded(void);
};
#endif
