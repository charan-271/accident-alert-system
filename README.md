# Arduino Accident Alert System

## Overview
This project is an IoT-based accident alert system designed to detect vehicle accidents and automatically send emergency notifications with location data to a predefined contact. The system utilizes accelerometer data to detect abnormal tilts and a flame sensor to detect potential fires, making it effective for various accident scenarios.

## How It Works
The system continuously monitors:
1. Vehicle orientation using an ADXL345 accelerometer
2. Fire/flame detection using a flame sensor
3. Location data using a GPS module

When an accident is detected (abnormal tilt or flame), the system:
1. Captures the current GPS coordinates
2. Sends an SMS alert with a Google Maps link to a pre-configured phone number
3. Displays real-time status information on an OLED display

## Components
- **Arduino Board**: Main microcontroller
- **ADXL345 Accelerometer**: Detects vehicle tilt and orientation changes
- **GPS Module**: Provides location data (latitude and longitude)
- **SIM900 GSM Module**: Handles SMS communication
- **Flame Sensor**: Detects fire
- **OLED Display**: Shows real-time system status
- **Matrix Keypad**: Allows user to input emergency contact number

## Pin Connections
- SIM900 GSM Module: TX->5, RX->4
- GPS Module: TX->3, RX->2
- Flame Sensor: Digital pin 13
- Keypad: Rows->12,11,10,9, Columns->8,7,6
- OLED Display: Connected via I2C (SDA, SCL)
- ADXL345: Connected via I2C (SDA, SCL)

## Features
- **Real-time Monitoring**: Continuously checks vehicle orientation and surroundings
- **Automated Alerts**: Sends SMS with precise location when an accident is detected
- **User Configuration**: Allows easy setup of emergency contact number
- **Visual Feedback**: Displays system status and sensor readings
- **Dual-Detection System**: Uses both tilt and flame detection for comprehensive accident detection

## Challenges Faced
1. **GPS Signal Reliability**: GPS modules often struggle with indoor testing and initial satellite acquisition time
2. **Power Management**: Balancing the power requirements of multiple sensors and communication modules
3. **Sensor Calibration**: Determining appropriate threshold values for the accelerometer to accurately detect accidents while avoiding false positives
4. **GSM Network Dependency**: System reliability depends on cellular network coverage
5. **Integration Complexity**: Coordinating multiple sensors and communication modules with limited Arduino I/O pins

## Future Improvements
1. **Battery Backup System**: Adding a reliable power source for post-accident operation
2. **Multiple Contact Support**: Allow configuration of multiple emergency contacts
3. **Audio Alarm System**: Add local audio alerts for nearby assistance
4. **Data Logging**: Record historical data for analysis
5. **Bluetooth/WiFi Integration**: Allow for mobile app connectivity and configuration
6. **Impact Force Measurement**: Add capability to measure and report impact severity
7. **Cloud Integration**: Upload accident data to a cloud platform for centralized monitoring

## Setup Instructions
1. Connect all components according to the circuit diagram in the images folder
2. Upload the Arduino sketch to your Arduino board
3. Power on the system
4. Use the keypad to enter an emergency contact number (format: +91XXXXXXXXXX)
5. The system will begin monitoring for potential accidents

## Usage
Once set up, the system requires no user interaction during normal operation. The OLED display will show real-time sensor data. In the event of an accident, an SMS will be automatically sent to the configured phone number.

## License

MIT License

Copyright (c) 2025 - charan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
