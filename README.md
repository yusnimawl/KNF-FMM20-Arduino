
# PumpController Library Documentation

The **PumpController** library is designed to control a linear stroke pump for dispensing liquids with high accuracy. It supports setting flow rates, counting dispensed volumes, and detecting abrupt changes using analog sensors to prevent over-dispensing.

## Class: `PumpController`

### Constructor: `PumpController(int pin, float strokeVol, int strokeTime, int maxFreq)`

- **Description**: Initializes the pump controller.
- **Parameters**:
  - `pin`: The digital output pin connected to the pump.
  - `strokeVol`: The volume per stroke in mL.
  - `strokeTime`: The time per stroke in milliseconds.
  - `maxFreq`: The maximum frequency in Hertz (Hz) that the pump can operate at.
  
- **Example**:
  ```cpp
  PumpController pump(9, 0.015, 25, 20);
  ```

## Public Methods

### `void setFlowRate(float flowRate)`

- **Description**: Sets the flow rate of the pump.
- **Parameters**:
  - `flowRate`: Desired flow rate in mL/min.
- **Example**:
  ```cpp
  pump.setFlowRate(18.0);
  ```

### `void start()`

- **Description**: Starts the pump.
- **Example**:
  ```cpp
  pump.start();
  ```

### `void stop()`

- **Description**: Stops the pump.
- **Example**:
  ```cpp
  pump.stop();
  ```

### `void reset()`

- **Description**: Resets the volume counter and stroke counter.
- **Example**:
  ```cpp
  pump.reset();
  ```

### `void update()`

- **Description**: This method should be called repeatedly in the main loop to update the pump state. It calculates the pump operation based on time and ensures the pump continues to run according to the set flow rate.
- **Example**:
  ```cpp
  void loop() {
    pump.update();
  }
  ```

### `void primeWithDigitalSensor(int sensorPin, int successState)`

- **Description**: Primes the pump until a specific digital sensor state is detected.
- **Parameters**:
  - `sensorPin`: The digital sensor pin to monitor.
  - `successState`: The state (`0` or `1`) indicating that priming is complete.
- **Example**:
  ```cpp
  pump.primeWithDigitalSensor(2, 1);
  ```

### `void primeWithAnalogSensor(int sensorPin, int threshold)`

- **Description**: Primes the pump based on an analog sensor value and stops once the sensor exceeds a given threshold.
- **Parameters**:
  - `sensorPin`: The analog sensor pin to monitor.
  - `threshold`: The analog value that determines when priming is successful.
- **Example**:
  ```cpp
  pump.primeWithAnalogSensor(A0, 600);
  ```

### `int calibrateAnalogSensor(int sensorPin, unsigned long calibrationTime)`

- **Description**: Calibrates an analog sensor by monitoring its value over a period of time and determining the min and max values, which can be used to set a threshold.
- **Parameters**:
  - `sensorPin`: The analog sensor pin to monitor.
  - `calibrationTime`: Duration in milliseconds for calibration.
- **Returns**: The calculated threshold value.
- **Example**:
  ```cpp
  int threshold = pump.calibrateAnalogSensor(A0, 5000); // 5 seconds calibration
  ```

### `void monitorSensorForChange(int sensorPin, int changeThreshold)`

- **Description**: Monitors an analog sensor for abrupt changes in value. If the difference between consecutive readings exceeds the given `changeThreshold`, the pump stops to avoid over-dispensing.
- **Parameters**:
  - `sensorPin`: The analog sensor pin to monitor.
  - `changeThreshold`: The threshold for detecting abrupt changes.
- **Example**:
  ```cpp
  pump.monitorSensorForChange(A0, 50);
  ```

### `float getTotalVolume()`

- **Description**: Returns the total volume pumped so far.
- **Returns**: Total volume dispensed in mL.
- **Example**:
  ```cpp
  float totalVolume = pump.getTotalVolume();
  Serial.print("Total volume pumped: ");
  Serial.println(totalVolume);
  ```

## Additional Methods for Dispensing Volume

### `void dispenseVolume(float volumeTarget, float flowRate)`

- **Description**: Dispenses a specified target volume at the given flow rate. This function calculates the required number of strokes to reach the target volume and stops the pump when the target is reached.
- **Parameters**:
  - `volumeTarget`: The target volume to dispense in mL.
  - `flowRate`: The desired flow rate in mL/min.
- **Example**:
  ```cpp
  pump.dispenseVolume(100.0, 18.0);  // Dispense 100 mL at 18 mL/min
  ```

## Example Usage

Here’s an example sketch showing how to use the library:

```cpp
#include <PumpController.h>

// Create a PumpController object
PumpController pump(9, 0.015, 25, 20);  // Pump pin, stroke volume (mL), stroke time (ms), max frequency (Hz)

int sensorPin = A0;  // Example analog pin for sensor
int digitalSensorPin = 2;  // Example digital pin for sensor

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);

  // Set flow rate for continuous operation
  float flowRate = 18.0; // Set flow rate in mL/min
  pump.setFlowRate(flowRate);
  pump.start(); // Start the pump

  // Example for priming with a digital sensor
  pump.primeWithDigitalSensor(digitalSensorPin, 1);

  // Example for priming with an analog sensor with calibration
  int calibratedThreshold = pump.calibrateAnalogSensor(sensorPin, 5000); // 5 seconds calibration
  Serial.print("Calibrated threshold: ");
  Serial.println(calibratedThreshold);
  pump.primeWithAnalogSensor(sensorPin, calibratedThreshold);
}

void loop() {
  // Continuously monitor the sensor for abrupt changes
  pump.monitorSensorForChange(sensorPin, 50); // Example threshold of 50

  // Update the pump state if needed
  pump.update();
  
  // Optional: Dispense a target volume
  pump.dispenseVolume(100.0, 18.0);  // Dispense 100 mL at 18 mL/min
}
```
