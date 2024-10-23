#include <PumpController.h>

// Create an instance of the DispenseController
PumpController pump(9, 15, 30, 20); // Pin 9, 500 µL stroke volume, 25 ms stroke time, max frequency 20 Hz
float flow =0;
void setup() {
    Serial.begin(9600);

}

// void loop() {
//     // Example usage
//     //pump.frequencyOut(5,25000);
//     // pump.flowOut(1);

//     float targetVolume = 5.0; // Target volume in mL
//     float flowRate = 20; // Flow rate in mL/min

//     pump.dispense(targetVolume, flowRate);
//     Serial.print("Total volume dispensed: ");
//     Serial.println(pump.readVolumeDispensed());

//     delay(10000); // Wait 10 seconds before dispensing again
// }
void loop(){
    if (Serial.available() > 0) {
    // Read the input from Serial
    String input = Serial.readStringUntil('\n');
    float newFlow = input.toFloat();

    // Validate the input and update the frequency if it's valid
    if (newFlow >= 0) {
      flow = newFlow;
      Serial.print("Flow updated to: ");
      Serial.println(flow);
    } else {
      Serial.println("Invalid input. Please enter a positive number.");
    }
  }
  pump.flowOut(flow);

    static unsigned long lastPrintTime = 0;
  if (micros() - lastPrintTime >= 100000) { // Print every 0.1 second
    Serial.print("Total volume dispensed: ");
    Serial.println(pump.readVolumeDispensed());
    lastPrintTime = micros();
  }
}
