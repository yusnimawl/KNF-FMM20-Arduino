#include "PumpController.h"

// Constructor
PumpController::PumpController(int pin, float strokeVol, int strokeTime, int maxFreq)
  : pulsePin(pin), strokeVolMicroliters(strokeVol), strokeTime(strokeTime), maxFreq(maxFreq), previousMicros(0), pulseState(LOW), pulseCount(0) {
    pinMode(pulsePin, OUTPUT);
}

// Function to generate the pulse with constant width and variable frequency
void PumpController::frequencyOut(float freq, float pulseWidthUs) {
    unsigned long currentMicros = micros();
    if (freq >= 0 && freq <= maxFreq) {
        period = 1000000 / freq; // Calculate period based on frequency
    }

    // Check if it's time for the next pulse
    if (pulseState == LOW && (currentMicros - previousMicros >= period - pulseWidthUs)) {
        // Turn on the pulse
        digitalWrite(pulsePin, HIGH);
        previousMicros = currentMicros;
        pulseState = HIGH;
        pulseCount++; // Increment the pulse count
    } 
    else if (pulseState == HIGH && (currentMicros - previousMicros >= pulseWidthUs)) {
        // Turn off the pulse after the pulseWidth duration
        digitalWrite(pulsePin, LOW);
        pulseState = LOW;
        previousMicros = currentMicros;
    }
}

// Calculate frequency based on flow rate
void PumpController::flowOut(float flowRateMlPerMin) {
    float frequency = (flowRateMlPerMin*1000 / strokeVolMicroliters) / 60;
    frequencyOut(frequency, strokeTime * 1000); // Convert stroke time to microseconds
}

// Read total volume dispensed
float PumpController::readVolumeDispensed() {
    return pulseCount * strokeVolMicroliters / 1000.0; // Convert to mL
}

// Dispense a target volume at a given flow rate
void PumpController::dispense(float targetVolumeInMl, float flowRate) {
    unsigned long targetPulses = static_cast<unsigned long>(targetVolumeInMl * 1000 / strokeVolMicroliters); // Convert to micro-liters
    pulseCount = 0; // Reset pulse count

    // Continue dispensing until the target volume is reached
    while (pulseCount <= targetPulses) {
        flowOut(flowRate); // Control flow
        // You may want to add a small delay to prevent the loop from running too fast
        delay(10); // Adjust delay as necessary
    }
    digitalWrite(pulsePin, LOW);
}
