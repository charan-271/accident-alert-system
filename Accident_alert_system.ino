#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>
#include <Keypad.h>

#define SCREEN_I2C_ADDR 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
SSD1306AsciiWire oled;

#define SIM_TX 5
#define SIM_RX 4 
#define GPS_TX 3 
#define GPS_RX 2 
#define FLAME_SENSOR_PIN 13  

SoftwareSerial sim900(SIM_TX, SIM_RX);
SoftwareSerial gpsSerial(GPS_TX, GPS_RX);
TinyGPSPlus gps;
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(123);

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};
byte rowPins[ROWS] = {12, 11, 10, 9};
byte colPins[COLS] = {8, 7, 6};     
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String phoneNumber = "+91";
double lastValidLat = 00000.00, lastValidLon = 00000.00;

void setup() {
    Serial.begin(9600);
    sim900.begin(9600);
    gpsSerial.begin(9600);
    Wire.begin();
    pinMode(FLAME_SENSOR_PIN, INPUT);

    oled.begin(&Adafruit128x64, SCREEN_I2C_ADDR);
    oled.setFont(System5x7);
    oled.clear();
    oled.println("System Starting...");
    delay(2000);

    if (!accel.begin()) {
        oled.println("No ADXL345 found!");
        while (1);
    }

    enterPhoneNumber();

    oled.clear();
    oled.println("GPS Initializing...");
    delay(1000);
}

void loop() {
    double latitude = 00000.00, longitude = 00000.00;

    if (getGPSCoordinates(latitude, longitude)) {
        lastValidLat = latitude;
        lastValidLon = longitude;
    }

    bool flameDetected = digitalRead(FLAME_SENSOR_PIN) == LOW;
    displayData(lastValidLat, lastValidLon, flameDetected);

    if (isAbnormalTilt() || flameDetected) {
        oled.clear();
        oled.println("Alert Sent!");
        sendSMS(lastValidLat, lastValidLon);
    }

    delay(5000);
}

bool getGPSCoordinates(double &lat, double &lon) {
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());
        if (gps.location.isValid()) {
            lat = gps.location.lat();
            lon = gps.location.lng();
            return true;
        }
    }
    return false;
}


void sendSMS(double lat, double lon) {
    Serial.println("Sending SMS...");
    sim900.println("AT+CMGF=1");
    delay(1000);
    sim900.print("AT+CMGS=\"");
    sim900.print(phoneNumber);
    sim900.println("\"");
    delay(1000);
    sim900.print("Alert! \nLocation: ");
    sim900.print("http://maps.google.com/maps?q=");
    sim900.print(lat, 6);
    sim900.print(",");
    sim900.println(lon, 6);
    delay(1000);
    sim900.write(26);
    delay(3000);
    Serial.println("SMS Sent!");
}


bool isAbnormalTilt() {
    sensors_event_t event;
    accel.getEvent(&event);

    float xAngle = atan2(event.acceleration.x, event.acceleration.z) * 180.0 / M_PI;
    float yAngle = atan2(event.acceleration.y, event.acceleration.z) * 180.0 / M_PI;

    Serial.print("Tilt X: "); Serial.print(xAngle);
    Serial.print(" Tilt Y: "); Serial.println(yAngle);

    if (abs(xAngle) > 60 || abs(yAngle) > 60) {
        Serial.println("Abnormal tilt detected!");
        return true;
    }
    return false;
}

void displayData(double lat, double lon, bool flameDetected) {
    sensors_event_t event;
    accel.getEvent(&event);

    oled.clear();
    oled.setCursor(0, 0);
    oled.println("Lat:"); oled.setCursor(40, 0); oled.println(lat, 6);
    oled.setCursor(0, 10);
    oled.println("Lng:"); oled.setCursor(40, 10); oled.println(lon, 6);
    oled.setCursor(0, 20);
    oled.println("X:"); oled.setCursor(40, 20); oled.println(event.acceleration.x, 3);
    oled.setCursor(0, 30);
    oled.println("Y:"); oled.setCursor(40, 30); oled.println(event.acceleration.y, 3);
    oled.setCursor(0, 40);
    oled.println("Flame:"); oled.setCursor(40, 40);
    oled.println(flameDetected ? "YES" : "NO");
}

void enterPhoneNumber() {
    oled.clear();
    oled.println("Enter Number:");
    oled.println(phoneNumber);
    delay(500);

    while (true) {
        char key = keypad.getKey();
        if (key) {
            if (key == '*') { 
                if (phoneNumber.length() > 3) {
                    phoneNumber.remove(phoneNumber.length() - 1);
                }
            } else if (key == '#') { 
                if (phoneNumber.length() > 3) {
                    oled.clear();
                    oled.println("Saved: ");
                    oled.println(phoneNumber);
                    delay(1000);
                    break;
                }
            } else if (phoneNumber.length() < 13) {
                phoneNumber += key;
            }

            oled.clear();
            oled.println("Enter Number:");
            oled.println(phoneNumber);
        }
    }
}
