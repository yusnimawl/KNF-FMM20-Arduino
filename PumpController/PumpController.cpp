#include "PumpController.h"

// Constructor to initialize the pump controller with parameters
PumpController::PumpController(int pin, float strokeVol, int strokeTime, int maxFreq) {
  pumpPin = pin;
  strokeVolume = strokeVol;
  strokeTimeMs = strokeTime;
  maxFrequency = maxFreq;
  pumpOn = false;
  isRunning = false;
  strokeCount = 0;
  volumeDispensed = 0.0;
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

// Function to start the pump
void PumpController::start() {
  isRunning = true;
}

// Function to stop the pump
void PumpController::stop() {
  isRunning = false;
  digitalWrite(pumpPin, LOW); // Ensure the pump is off when stopped
}

// Function to reset the stroke counter and total volume
void PumpController::reset() {
  strokeCount = 0;
  volumeDispensed = 0.0;
}

// Function to get the total pumped volume in mL
float PumpController::getTotalVolume() {
  return strokeCount * strokeVolume; // Total volume in mL
}

// Function to prime the pump using a digital sensor
void PumpController::primeWithDigitalSensor(int sensorPin, int successState) {
  pinMode(sensorPin, INPUT);

  while (true) {
    int sensorState = digitalRead(sensorPin);
    if (sensorState == successState) {
      stop();  // Stop the pump when priming is successful
      Serial.println("Priming successful.");
      break;
    }
    update();  // Continue pumping
  }
}

// Function to calibrate the analog sensor by detecting min/max values
int PumpController::calibrateAnalogSensor(int sensorPin, unsigned long calibrationTime) {
  unsigned long startTime = millis();
  int minValue = 1023;
  int maxValue = 0;
  
  while (millis() - startTime < calibrationTime) {
    int sensorValue = analogRead(sensorPin);
    if (sensorValue < minValue) minValue = sensorValue;
    if (sensorValue > maxValue) maxValue = sensorValue;
  }

  // Calculate and return the average threshold value
  return (minValue + maxValue) / 2;
}

// Function to prime the pump using an analog sensor with a calibrated threshold
void PumpController::primeWithAnalogSensor(int sensorPin, int threshold) {
  pinMode(sensorPin, INPUT);

  while (true) {
    int sensorValue = analogRead(sensorPin);
    if (sensorValue >= threshold) {
      stop();  // Stop the pump when priming is successful
      Serial.println("Priming successful.");
      break;
    }
    update();  // Continue pumping
  }
}

// Function to dispense a specific volume with a given flow rate
void PumpController::dispenseVolume(float volumeTarget, float flowRate) {
  setFlowRate(flowRate);  // Set the flow rate
  start();                // Start the pump

  while (getTotalVolume() < volumeTarget) {
    update();  // Continue updating the pump state
  }

  stop();  // Stop the pump once the target volume is reached
  Serial.println("Dispensing complete.");
}

// Function to update the pump signal based on the calculated frequency
void PumpController::update() {
  if (!isRunning) {
    return;  // If the pump is not running, exit early
  }

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
    strokeCount++;               // Increment the stroke counter
  }
}
