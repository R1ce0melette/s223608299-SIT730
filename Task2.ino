#include <DHT11.h>
#include <BH1750FVI.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include "secrets.h" 
#include <ThingSpeak.h>

// Create an instance of the DHT11 class. Pin 7 to Signal
DHT11 dht11(7);
// Create the Lightsensor instance
BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
WiFiClient client;

// ThingSpeak communication
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

void setup() {
    // Initialize serial communication to allow debugging and data readout.
    // Using a baud rate of 115200 for compatibility with the light sensor
    Serial.begin(115200);
    LightSensor.begin();
    ThingSpeak.begin(client); 
    //dht11.setDelay(2000); delay sensor reading to avoid timeout error
    // Connect to WiFi
    if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
}

void loop() {
    int temperature = 0;
    int humidity = 0;

    // Read temperature and humidity values from sensor.
    int result = dht11.readTemperatureHumidity(temperature, humidity);
    // Read light level from sensor
    uint16_t lux = LightSensor.GetLightIntensity();
    // Print values to serial, print error msg if reading fails.
    if (result == 0) { // simulate try catch to generate exception with sensor value
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print(" °C\tHumidity: ");
        Serial.print(humidity);
        Serial.print("%\tLight: ");
        Serial.println(lux);
        // Set field value
        ThingSpeak.setField(1, temperature);
        ThingSpeak.setField(2, humidity);
        ThingSpeak.setField(3, lux);
        delay(2000); // light sensor lib doesn't have a built-in delay function, manually delay to avoid timeout err
    } else {
        // Print error message based on the error code.
        Serial.println(DHT11::getErrorString(result));
    }
    // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(20000); // Wait 20 seconds to update the channel again
  
}
