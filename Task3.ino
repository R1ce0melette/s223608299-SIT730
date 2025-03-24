#include <BH1750FVI.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include "secrets.h" 
#include <ThingSpeak.h>


// Create the Lightsensor instance
BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
WiFiClient client;

// ThingSpeak communication
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

void setup() {
    Serial.begin(115200);
    LightSensor.begin();
    ThingSpeak.begin(client);
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
  // var previous data ><= current lux level
  // put your main code here, to run repeatedly:
  // Read light level from sensor
    uint16_t lux = LightSensor.GetLightIntensity();
    Serial.print("Light: ");
    Serial.println(lux);
    ThingSpeak.setField(3, lux);
    // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(60000); // Wait 60 seconds to update the channel again
}
