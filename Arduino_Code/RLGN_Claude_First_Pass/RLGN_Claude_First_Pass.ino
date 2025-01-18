// Required libraries
#include <ArduinoBLE.h>
#include <Wire.h>
#include <Arduino.h>

// Constants
#define LED_PIN 4  // Pin 4 (corresponding to A4)
#define BATTERY_PIN A0
#define BLE_SERVICE_UUID        "19B10000-E8F2-537E-4F6C-D104768A1214"
#define BLE_CHARACTERISTIC_UUID "19B10001-E8F2-537E-4F6C-D104768A1214"

// Global variables
BLEService calendarService(BLE_SERVICE_UUID);
BLEBoolCharacteristic eventCharacteristic(BLE_CHARACTERISTIC_UUID, BLERead | BLEWrite);
bool eventActive = false;
unsigned long lastBatteryCheck = 0;
const unsigned long BATTERY_CHECK_INTERVAL = 300000; // 5 minutes

void setup() {
  Serial.begin(115200);
  delay(1000);  // Give serial monitor time to open
  
  Serial.println("Calendar Light Starting...");
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  // Turn on LED to show we're starting
  delay(1000);
  digitalWrite(LED_PIN, LOW);   // Turn off LED
  
  pinMode(BATTERY_PIN, INPUT);
  
  // Initialize BLE with retry
  initBLE();
  
  Serial.println("Bluetooth device active, waiting for connections...");
  digitalWrite(LED_PIN, HIGH);  // Turn on LED to show we're ready
  delay(500);
  digitalWrite(LED_PIN, LOW);
}

void initBLE() {
  int retries = 3;
  while (retries > 0) {
    Serial.println("Initializing BLE...");
    if (BLE.begin()) {
      // Set up BLE service
      BLE.setLocalName("Calendar Light");
      BLE.setAdvertisedService(calendarService);
      calendarService.addCharacteristic(eventCharacteristic);
      BLE.addService(calendarService);
      eventCharacteristic.writeValue(false);
      BLE.advertise();
      Serial.println("BLE initialized successfully");
      return;
    }
    
    Serial.print("BLE init failed, retries left: ");
    Serial.println(retries);
    retries--;
    delay(1000);
  }
  
  // If we get here, BLE failed to initialize
  Serial.println("Failed to initialize BLE after retries!");
  while (1) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
}

void loop() {
  BLEDevice central = BLE.central();
  
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    digitalWrite(LED_PIN, HIGH);  // Show connection with LED
    
    while (central.connected()) {
      // Check if event status has changed
      if (eventCharacteristic.written()) {
        eventActive = eventCharacteristic.value();
        digitalWrite(LED_PIN, eventActive ? HIGH : LOW);
        Serial.print("Event status changed to: ");
        Serial.println(eventActive);
      }
      
      // Check battery level periodically
      unsigned long currentMillis = millis();
      if (currentMillis - lastBatteryCheck >= BATTERY_CHECK_INTERVAL) {
        checkBattery();
        lastBatteryCheck = currentMillis;
      }
      
      // Small delay to prevent tight loop
      delay(10);
    }
    
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
    digitalWrite(LED_PIN, LOW);
    
    // Re-advertise after disconnection
    BLE.advertise();
  }
  
  // Quick blink every 3 seconds while waiting for connection
  if (!central) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(2900);
  }
}

void checkBattery() {
  // Read battery voltage through voltage divider
  int rawValue = analogRead(BATTERY_PIN);
  float voltage = (rawValue * 3.3) / 1024.0; // Convert to voltage
  float batteryVoltage = voltage * 2; // Assuming 1:1 voltage divider
  
  // Print battery status
  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage);
  Serial.println("V");
  
  // Add low battery warning if needed
  if (batteryVoltage < 3.3) { // Assuming 3.3V is low battery threshold
    Serial.println("Warning: Low battery!");
  }
}