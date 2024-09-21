#include <PumpController.h>

// Create a PumpController object
// Parameters: pump pin, stroke volume (mL), stroke time (ms), max frequency (Hz)
PumpController pump(9, 0.015, 25, 20);

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);

  // Inform the user to enter the flow rate
  Serial.println("Enter desired flow rate in mL/min: ");
}

void loop() {
  // Check if data is available on the Serial input
  if (Serial.available() > 0) {
    // Read the input flow rate from the Serial monitor
    float flowRate = Serial.parseFloat();
    
    // Set the flow rate on the pump
    pump.setFlowRate(flowRate);

    // Print out the actual flow rate and frequency
    Serial.print("Flow rate set to: ");
    Serial.print(flowRate);
    Serial.println(" mL/min");
  }

  // Continuously update the pump state
  pump.update();
}
