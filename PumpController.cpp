#include "PumpController.h"

// Constructor to initialize the pump controller with parameters
PumpController::PumpController(int pin, float strokeVol, int strokeTime, int maxFreq) {
  pumpPin = pin;
  strokeVolume = strokeVol;
  strokeTimeMs = strokeTime;
  maxFrequency = maxFreq;
  pumpOn = false;
  lastUpdate = 0;

  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, LOW); // Make sure pump is initially off
}

// Function to set the desired flow rate and calculate the frequency
void PumpController::setFlowRate(float flowRate) {
  currentFlowRate = flowRate;
  calculateFrequency();
}

// Function to calculate the required frequency and flow rate
void PumpController::calculateFrequency() {
  // Calculate the required frequency for the input flow rate
  calculatedFreq = (currentFlowRate * 1000.0) / (strokeVolume * 60.0);

  // Cap the frequency to the maximum allowed frequency
  if (calculatedFreq > maxFrequency) {
    calculatedFreq = maxFrequency;
  }

  // Calculate the full period for each stroke
  strokePeriodMs = 1000 / calculatedFreq;
}

// Function to update the pump signal based on the calculated frequency
void PumpController::update() {
  unsigned long currentTime = millis();

  // If the pump is ON and it's time to turn it off
  if (pumpOn && (currentTime - lastUpdate >= strokeTimeMs)) {
    digitalWrite(pumpPin, LOW); // Turn pump OFF
    pumpOn = false;
    lastUpdate = currentTime;   // Update the time
  }

  // If the pump is OFF and it's time to start a new stroke
  else if (!pumpOn && (currentTime - lastUpdate >= (strokePeriodMs - strokeTimeMs))) {
    digitalWrite(pumpPin, HIGH); // Turn pump ON
    pumpOn = true;
    lastUpdate = currentTime;    // Update the time
  }
}
