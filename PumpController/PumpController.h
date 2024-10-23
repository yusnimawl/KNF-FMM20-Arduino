#ifndef PumpController_h
#define PumpController_h

#include <Arduino.h>

class PumpController {
public:
    // Constructor
    PumpController(int pin, float strokeVol, int strokeTime, int maxFreq);

    // Methods
    void frequencyOut(float freq, float pulseWidth);
    void flowOut(float flowRateMlPerMin);
    float readVolumeDispensed();
    void dispense(float targetVolumeInMl, float flowRate);

private:
    int pulsePin;
    float strokeVolMicroliters;
    int strokeTime;      // Stroke time in milliseconds
    int maxFreq;
    unsigned long previousMicros;
    bool pulseState;
    unsigned long pulseCount;
    unsigned long period;
};

#endif
