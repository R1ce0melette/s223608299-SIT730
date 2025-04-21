#include <WiFiNINA.h>
#include "secrets.h"
#define RED_PIN 10
#define YELLOW_PIN 9
#define GREEN_PIN 8

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

WiFiServer server(80); //listen for command

// Timer structs
struct LEDTimer {
  bool active;
  unsigned long endTime;
  int pin;
};

LEDTimer redTimer = {false, 0, RED_PIN};
LEDTimer yellowTimer = {false, 0, YELLOW_PIN};
LEDTimer greenTimer = {false, 0, GREEN_PIN};

void setup() {
  // put your setup code here, to run once:
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  WiFi.begin(ssid, pass);

  Serial.begin(9600);
  while (!Serial);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected!");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    // Parse GET /led?recolor=red&state=on
    if (request.indexOf("GET /led") >= 0) {
      int colorIndex = request.indexOf("color=");
      int stateIndex = request.indexOf("state=");
      if (colorIndex >= 0 && stateIndex >= 0) {
        String color = request.substring(colorIndex + 6, request.indexOf("&", colorIndex));
        String state = request.substring(stateIndex + 6, request.indexOf(" ", stateIndex));

        int ledPin = -1;
        if (color == "red") ledPin = RED_PIN;
        else if (color == "yellow") ledPin = YELLOW_PIN;
        else if (color == "green") ledPin = GREEN_PIN;

        if (ledPin != -1) {
          digitalWrite(ledPin, state == "on" ? HIGH : LOW);
        }
      }
    }

    // Send basic response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    client.println("LED updated.");
    delay(1);
    client.stop();
  }
}
