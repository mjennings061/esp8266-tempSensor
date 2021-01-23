# esp8266-tempSensor
Wi-Fi enabled temperature and humidity sensor broadcasting to an IoT dashboard

## Dependencies
- ESP8266WiFi.h
- DHT.h
- DHT_U.h
- ThingSpeak.h

## Functions
- initDHT(void) - Setup DHT sensor
- getTemp(void) - Return the temperature in degrees C
- getHumid(void) - Return the humidity in % saturation
- connectWiFi(void) - Connect to the WiFi details provided in main
- postData(float, float) - Pass two channels of information to the ThingSpeak server

## Author
Michael Jennings (MJ) - mjenings061@gmail.com
