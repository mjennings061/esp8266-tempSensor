/* ESP8266-based data logger
 * Author: Michael Jennings
 * https://github.com/mjennings061/esp8266-tempSensor
 * 
 * To-do
 * - Add Wifi config via USB
 * - Take multiple readings and post the median
 * - Add low power mode
 * - Low power wakeup timer
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
unsigned long myChannelNumber = 1289133;
const char * myWriteAPIKey = "V8K703OF9AF96BHK";
// Timer variables
unsigned long delayMS;
unsigned long old_ms = 0;
unsigned long loopTime = 30000;   // How often to send data (30000ms minimum)
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
