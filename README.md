# KNF-FMM20-Arduino
# PumpController Library Documentation

The **PumpController** library is designed to control a linear stroke pump for dispensing liquids with high accuracy. It supports setting flow rates, counting dispensed volumes, and detecting abrupt changes using analog sensors to prevent over-dispensing.

---

## Class: `PumpController`

### **Constructor**: `PumpController(int pin, float strokeVol, int strokeTime, int maxFreq)`

- **Description**: Initializes the pump controller.
- **Parameters**:
  - `pin`: The digital output pin connected to the pump.
  - `strokeVol`: The volume per stroke in mL.
  - `strokeTime`: The time per stroke in milliseconds.
  - `maxFreq`: The maximum frequency in Hertz (Hz) that the pump can operate at.
  
- **Example**:
  ```cpp
  PumpController pump(9, 0.015, 25, 20);
