/* ESP8266-based data logger
 * Author: Michael Jennings
 * To-do
 * - Add low power mode
 * - Low power wakeup timer
 * 
 * Moisture levels
 * 0 to 340 (0-31%) dry soil
 * 341 to 700 (32-73%) humid soil
 * 701 to 1023 (74-100%) in water
*/

#include <ESP8266WiFi.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ThingSpeak.h>

#define LEDPIN D4       //inbuilt LED pin
#define DHTPIN D6       //DHT sensor pin
#define DHTTYPE DHT11   // Type of sensor is a DHT11

// Sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview
DHT_Unified dht(DHTPIN, DHTTYPE); //declare DHT sensor object

/////// FUNCTION DECLARATIONS ///////
void initDHT(void);         //initialise the DHT11
float getTemp(void);        //get temperature from the DHT11
float getHumid(void);       //get relative humidity from the DHT11
void connectWiFi(void);     //wait for wifi connection
void postData(float field1Data, float field2Data); //send data to Thingspeak

/////// GLOBAL VARIABLES ///////
// Network information
char ssid[] = "VM8773153";
char password[] = "bjmw3BhNcrjs";
WiFiClient client;

// ThingSpeak settings
unsigned long myChannelNumber = 1081009;
const char * myWriteAPIKey = "D3OJ6EMLX6PP244E";
// Timer variables
unsigned long delayMS;
unsigned long old_ms = 0;
unsigned long loopTime = 30000;
unsigned long lastConnectionTime;

void setup() {
  Serial.begin(115200);
  pinMode(LEDPIN, 1);
  digitalWrite(LEDPIN, 1);
  initDHT();
  connectWiFi();
  ThingSpeak.begin(client);
}

void loop() {
  // In each loop, make sure there is an Internet connection.
  if (WiFi.status() != WL_CONNECTED) { 
    connectWiFi();
  }
    
  // Perform measurements every loopTime milliseconds unless button is pressed
  if((millis() - old_ms) > loopTime){
    old_ms = millis(); //update the "last" timestamp
    digitalWrite(LEDPIN, 0);
    float tem = getTemp();  //get the temperature from the DHT11
    float hum = getHumid(); //get the relative humidity from the DHT11
    postData(tem, hum); //transmit data to thingspeak
    digitalWrite(LEDPIN, 1);
  }
}
