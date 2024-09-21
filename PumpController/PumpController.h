#ifndef PUMP_CONTROLLER_H
#define PUMP_CONTROLLER_H

#include <Arduino.h>

class PumpController {
  public:
    // Constructor to initialize the pump settings
    PumpController(int pin, float strokeVol, int strokeTime, int maxFreq);

    // Function to set the desired flow rate
    void setFlowRate(float flowRate);

    // Function to be called inside the main loop
    void update();

  private:
    int pumpPin;            // Pin to control the pump
    float strokeVolume;     // Stroke volume in mL
    int strokeTimeMs;       // Stroke time in milliseconds
    int maxFrequency;       // Maximum frequency in Hz

    float currentFlowRate;  // Current flow rate in mL/min
    float calculatedFreq;   // Frequency needed for the given flow rate
    unsigned long strokePeriodMs; // Full period in milliseconds
    bool pumpOn;            // Pump on/off state
    unsigned long lastUpdate; // Time of the last pump state change

    // Private function to calculate the frequency and flow rate
    void calculateFrequency();
};

#endif
