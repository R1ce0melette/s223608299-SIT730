#include <DHT11.h>
//pin
#define LED_PIN 10
#define SWITCH_PIN 2
#define SEN_PIN 9
#define TEMP_THRESHOLD 30
//var
volatile bool ledSwitchState = false;
volatile bool switchStateChanged = false;
unsigned long lastReadTime = 0;
const unsigned long readInterval = 2000;  // 2 seconds
bool manualOverride = false;
unsigned long manualOverrideStartTime = 0;
const unsigned long overrideTimeout = 120000; 
DHT11 dht11(SEN_PIN);

void setup() {
  //set pin mode
  pinMode(LED_PIN, OUTPUT);
  //set LOW slide switch
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), switchISR, CHANGE);
  Serial.begin(9600);
}

void loop() {
  int temperature = 0;
  int humidity = 0;
  unsigned long currentMillis = millis();

  if (switchStateChanged) {
    manualOverride = true;
    manualOverrideStartTime = currentMillis;
    digitalWrite(LED_PIN, ledSwitchState ? HIGH : LOW);
    Serial.print("Switch override. LED is now: ");
    Serial.println(ledSwitchState ? "ON" : "OFF");
    switchStateChanged = false; // reset the flag
  }

  if (manualOverride && (currentMillis - manualOverrideStartTime >= overrideTimeout)) {
    manualOverride = false;
    Serial.println("Manual override expired. Sensor regains control.");
  }

  if (currentMillis - lastReadTime >= readInterval) {
    lastReadTime = currentMillis;
  
    int result = dht11.readTemperatureHumidity(temperature, humidity);
    if (!manualOverride) {
      if (temperature >= TEMP_THRESHOLD) {
          digitalWrite(LED_PIN, HIGH);  // blink LED to alert
          Serial.print("Temperature too high: ");
          Serial.println(temperature);
          Serial.print("LED is now: ");
          Serial.println(ledSwitchState ? "ON" : "OFF");
        } else {
          digitalWrite(LED_PIN, ledSwitchState ? HIGH : LOW);
        }
    }
  }
  
}

void switchISR() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  if (interruptTime - lastInterruptTime > 200) {  // Debounce
    ledSwitchState = !ledSwitchState;
    switchStateChanged = true;
  }
  lastInterruptTime = interruptTime;
}