#ifndef PUMP_CONTROLLER_H
#define PUMP_CONTROLLER_H

#include <Arduino.h>

class PumpController {
  public:
    // Constructor to initialize the pump settings
    PumpController(int pin, float strokeVol, int strokeTime, int maxFreq);

    // Function to set the desired flow rate
    void setFlowRate(float flowRate);

    // Start, stop, reset, and update functions
    void start();
    void stop();
    void reset();
    void update();

    // Priming function for digital sensor
    void primeWithDigitalSensor(int sensorPin, int successState);

    // Priming function for analog sensor with calibration
    void primeWithAnalogSensor(int sensorPin, int threshold);

    // Calibrate an analog sensor's threshold
    int calibrateAnalogSensor(int sensorPin, unsigned long calibrationTime);

    // Dispense a specific volume
    void dispenseVolume(float volumeTarget, float flowRate);

    // Get the total volume pumped
    float getTotalVolume();

  private:
    int pumpPin;            // Pin to control the pump
    float strokeVolume;     // Stroke volume in mL
    int strokeTimeMs;       // Stroke time in milliseconds
    int maxFrequency;       // Maximum frequency in Hz
    bool isRunning;         // Whether the pump is running

    float currentFlowRate;  // Current flow rate in mL/min
    float calculatedFreq;   // Frequency needed for the given flow rate
    unsigned long strokePeriodMs; // Full period in milliseconds
    bool pumpOn;            // Pump on/off state
    unsigned long lastUpdate; // Time of the last pump state change
    unsigned long strokeCount; // Number of strokes (pumps)
    
    // Volume dispense tracking
    float volumeDispensed; // Volume dispensed in mL
    float volumeTarget;    // Target volume for dispensing

    // Private function to calculate the frequency and flow rate
    void calculateFrequency();
};

#endif
