#define USING_TIMER_TC3         true      // Only TC3 can be used for SAMD51
#define USING_TIMER_TC4         false     // Not to use with Servo library
#define USING_TIMER_TC5         false
#define USING_TIMER_TCC         false
#define USING_TIMER_TCC1        false
#define USING_TIMER_TCC2        false 

#include <SAMDTimerInterrupt.h>
#include <SAMDTimerInterrupt.hpp>
#include <SAMD_ISR_Timer.h>
#include <SAMD_ISR_Timer.hpp>
#include <DHT11.h>

#define LED_PIN 8
#define SWITCH_PIN 10
#define TIMER_PIN 9
#define SEN_LED 7
#define SEN_PIN 6
#define TEMP_THRESHOLD 30

#define TIMER_INTERVAL_MS 1000
#define HW_TIMER_INTERVAL_MS 50L
#define TIMER_INTERVAL_2S 2000L
#define TIMER_INTERVAL_5S 5000L
#define TIMER_INTERVAL_11S 11000L

#if USING_TIMER_TC3
  #define SELECTED_TIMER      TIMER_TC3
#elif USING_TIMER_TC4
  #define SELECTED_TIMER      TIMER_TC4
#elif USING_TIMER_TC5
  #define SELECTED_TIMER      TIMER_TC5
#elif USING_TIMER_TCC
  #define SELECTED_TIMER      TIMER_TCC
#elif USING_TIMER_TCC1
  #define SELECTED_TIMER      TIMER_TCC1
#elif USING_TIMER_TCC2
  #define SELECTED_TIMER      TIMER_TCC
#else
  #error You have to select 1 Timer  
#endif

SAMDTimer ITimer(SELECTED_TIMER);
// Init SAMD_ISR_Timer
// Each SAMD_ISR_Timer can service 16 different ISR-based timers
SAMD_ISR_Timer ISR_Timer;

DHT11 dht11(SEN_PIN);

volatile bool ledSwitchState = false;    // Shared flag for LED state
volatile unsigned long lastInterruptTime = 0;
 
volatile bool ledTimerState = false;
volatile bool sensorTimerState = false;
int temperature = 0;
int humidity = 0;
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT);
  pinMode(TIMER_PIN, OUTPUT);
  pinMode(SEN_LED, OUTPUT);
  pinMode(SEN_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), switchToggled, CHANGE);
  //Timer interrupt
  if (ITimer.attachInterruptInterval_MS(HW_TIMER_INTERVAL_MS, TimerHandler))
    Serial.println("Starting  ITimer OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer. Select another freq. or timer");
  ISR_Timer.setInterval(TIMER_INTERVAL_11S, checkTemp);
  ISR_Timer.setInterval(TIMER_INTERVAL_5S, ledTimer);
}

void loop() {
  // LED 1 switch control
  digitalWrite(LED_PIN, ledSwitchState);
  // LED 2 timer interrupt
  digitalWrite(TIMER_PIN, ledTimerState);
  // Read sensor data
  temperature=dht11.readTemperature();
  delay(2000);
}

void switchToggled() {
  // Simple debounce (only accept interrupt if 200ms have passed)
  unsigned long currentTime = millis();
  Serial.println("Switch interrupt");
  if (currentTime - lastInterruptTime > 200) {
    ledSwitchState = !ledSwitchState;  // Toggle LED state
    lastInterruptTime = currentTime;
  }
}

void TimerHandler() {
  ISR_Timer.run();// Doing something here inside ISR
}
void ledTimer() {
  ledTimerState = !ledTimerState;
  Serial.println("RED LED Interrupt every 5s");
}
void checkTemp() {
  Serial.print("Temperature Interrupt every 11s - ");
  Serial.println(temperature);
  digitalWrite(SEN_LED, !digitalRead(SEN_LED));
}
