#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <ArduinoJson.h>
// WiFi credentials
const char* ssid = "<SSID>"; //WIFI NAME
const char* password = "<WIFI PASSWORD>"; //WIFI PASSWORD

// MQTT broker info
const char* mqttBroker = "192.168.0.253";  // Your Raspberry Pi IP
const int mqttPort = 1883;

// LED PWM pins
const int ledPins[3] = {11, 10, 9};
int ledValues[3] = {0, 0, 0};

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

void connectToWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void messageReceived(int messageSize) {
  Serial.print("Received MQTT message: ");
  String message;
  while (mqttClient.available()) {
    char c = (char)mqttClient.read();
    message += c;
  }
  Serial.println(message);

  // Parse JSON string: {"led1": 123, "led2": 150, "led3": 200}
  StaticJsonDocument<128> doc;
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  // Read LED values
  ledValues[0] = constrain(doc["led1"] | 0, 0, 255);
  ledValues[1] = constrain(doc["led2"] | 0, 0, 255);
  ledValues[2] = constrain(doc["led3"] | 0, 0, 255);

  // Apply PWM to pins
  for (int i = 0; i < 3; i++) {
    analogWrite(ledPins[i], ledValues[i]);
  }

  Serial.print("Set LED values: ");
  Serial.print(ledValues[0]); Serial.print(", ");
  Serial.print(ledValues[1]); Serial.print(", ");
  Serial.println(ledValues[2]);
}

void connectToMqtt() {
  Serial.print("Connecting to MQTT broker...");
  while (!mqttClient.connect(mqttBroker, mqttPort)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" connected");
  mqttClient.subscribe("led/control");
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
    analogWrite(ledPins[i], 0);
  }

  connectToWiFi();

  mqttClient.onMessage(messageReceived);
  connectToMqtt();
}

void loop() {
  if (!mqttClient.connected()) {
    connectToMqtt();
  }
  mqttClient.poll();
}
