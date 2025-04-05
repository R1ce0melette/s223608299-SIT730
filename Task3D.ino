#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include "secrets.h"
#include <HCSR04.h>
#include <ThingSpeak.h>
#define LED_PIN 6

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "192.168.0.254";
int        port     = 1883;
const char topic[]  = "SIT210/wave";

//Init sensor
UltraSonicDistanceSensor distanceSensor(9, 10); // trig pin 9, echo pin 10
//Init variable count
int count = 0;
// ThingSpeak communication
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
//Distance data
float distance = 0;
String message = "test";

void setup() {
  //set pin mode
  pinMode(LED_PIN, OUTPUT);
  //Init ThingSpeak client
  ThingSpeak.begin(wifiClient);
  // Wifi connect
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
  //connect to mqtt client
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    while (1);
  }
  //subscribe to topic
  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
  mqttClient.subscribe(topic);
  Serial.println("Subscribed to topic");
  //test led
  digitalWrite(LED_PIN, HIGH);
  delay(3000);
  digitalWrite(LED_PIN, LOW);
}
void loop() {
  mqttClient.poll();
  distance = distanceSensor.measureDistanceCm();
  //listen to subscribed topic
  while (mqttClient.available()) {
    message = mqttClient.readString();
    if (message.equals("Long Hoang Do")){
        Serial.print("Received message on topic SIT210/wave: ");
        Serial.print(message);
        Serial.println(count);
        wave_led();
        count++;
        if ((count%10)==0){
          write_to_TS();
        } else {
          continue;
        }
    } else if (message.equals("Long Hoang Do pat")){
        Serial.print("Received PAT message on topic SIT210/wave: ");
        pat_led();
        Serial.println(message);
    }
  }
  if (distance <= 30) {
    sendMQTT("Long Hoang Do pat");
  } else if (distance > 30 && distance <= 100) {
    sendMQTT("Long Hoang Do");
  }
  delay(5000); // Send distance data every 15 seconds
}
void wave_led(){
  //flash led 3 times
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(500);
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(500);
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(500);
  }
void pat_led(){
  //flash on 5s
    digitalWrite(LED_PIN, HIGH);
    delay(5000);
    digitalWrite(LED_PIN, LOW);
  }

void sendMQTT(String input){
  Serial.println(distance);
  mqttClient.beginMessage(topic);
  mqttClient.print(input);
  mqttClient.endMessage();
}
void write_to_TS(){
    ThingSpeak.setField(4, "1");
    // write to the ThingSpeak channel
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
  }