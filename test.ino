#include <PumpController.h>

// Create a PumpController object
// Parameters: pump pin, stroke volume (mL), stroke time (ms), max frequency (Hz)
PumpController pump(9, 0.015, 25, 20);

int sensorPin = A0;  // Example analog pin for sensor
int digitalSensorPin = 2;  // Example digital pin for sensor

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);

  // Example for priming with a digital sensor
  // The digital sensor returns '1' when priming is successful
  pump.primeWithDigitalSensor(digitalSensorPin, 1);
  
  // Example for calibrating and priming with an analog sensor
  int calibratedThreshold = pump.calibrateAnalogSensor(sensorPin, 5000); // 5 sec calibration
  Serial.print("Calibrated threshold: ");
  Serial.println(calibratedThreshold);
  pump.primeWithAnalogSensor(sensorPin, calibratedThreshold);

  // Example for dispensing a specific volume
  float targetVolume = 50.0;  // Target volume in mL
  float flowRate = 18.0;      // Flow rate in mL/min
  pump.dispenseVolume(targetVolume, flowRate);
}

void loop() {
  // Update the pump state
  pump.update();
}
