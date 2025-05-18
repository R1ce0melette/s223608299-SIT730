#include <ArduinoBLE.h>

const int LED_PIN = 9;
const int BUZ_PIN = 10;

// Custom BLE service and characteristic UUIDs
BLEService lightService("19b10000-e8f2-537e-4f6c-d104768a1214");
BLEByteCharacteristic luxChar("19b10010-e8f2-537e-4f6c-d104768a1214", BLEWrite);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZ_PIN, OUTPUT);

  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE!");
    while (1);
  }
  // Print BLE MAC address
  Serial.print("BLE MAC Address: ");
  Serial.println(BLE.address()); 
  BLE.setLocalName("Nano33IoT");
  BLE.setAdvertisedService(lightService);
  lightService.addCharacteristic(luxChar);
  BLE.addService(lightService);

  BLE.advertise();
  Serial.println("BLE device ready, advertising...");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    String allowedMac = "b8:27:eb:d5:cc:57"; //R4 BLE Mac address
    Serial.print("Connected to: ");
    Serial.println(central.address());

    if (central.address() != allowedMac) {
      Serial.println("Unauthorized device! Disconnecting...");
      central.disconnect();
      return;
    }
    Serial.println("Authorized Pi connected.");
    while (central.connected()) {
      if (luxChar.written()) {
        int alert_level = luxChar.value();  // 0 (no alert) to 255 (max alert)

        // Scale values to LED/Buzzer PWM
        analogWrite(LED_PIN, alert_level);  // LED brightness
        analogWrite(BUZ_PIN, alert_level);  // Buzzer intensity
        Serial.print("Received alert level: ");
        Serial.println(alert_level);
      }
    }

    Serial.println("Disconnected");
    BLE.advertise();
  }
}
