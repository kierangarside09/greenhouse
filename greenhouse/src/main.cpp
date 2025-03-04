/*
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <NTPClient.h>
#include "ESPNowW.h"
#include "DHT.h"

// https://randomnerdtutorials.com/power-esp32-esp8266-solar-panels-battery-level-monitoring/ have a gander at this webpage for info on solar power, just incase you lose the bookmark, nobhead



#define DHTPIN 14
#define DHTTYPE DHT11

const char *ssid = "TP-Link_9692";
const char *password = "88089423";

// sender mac address: 30:ae:a4:29:fc:ec
// receiver mac address: bc:dd:c2:c3:13:d0

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

uint8_t receiver_mac[] = {0x36, 0x33, 0x33, 0x33, 0x33, 0x33};

const unsigned long RECORDING_DELAY = 5000; // 300000; 5mins - currently set to 5 seconds for testing
unsigned long RECORDING_LAST;

DHT dht(DHTPIN, DHTTYPE);

bool timeDiff(unsigned long start, unsigned long delay)
{
    return (millis() - start) >= delay;
}

const char *ntpServer = "pool.ntp.org";

long getTimeData()
{
    timeClient.update();
    return timeClient.getEpochTime();
}

typedef struct reading_struct
{
    float temperature;
    float humidity;
    long time;
} reading_struct;

reading_struct reading;

void setup()
{
    Serial.begin(115200);

    // Initialize a NTPClient to get time
    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" CONNECTED");

    WiFi.mode(WIFI_MODE_STA);

    WiFi.disconnect();
    ESPNow.init();
    ESPNow.add_peer(receiver_mac);

    configTime(0,0,ntpServer);

    //connect to other ESP32 using ESPNow
    // set this up later as I cant be arsed setting up the other device. For now TODO: get the monitor device to a point where it can send the required data using json because json is pretty cool
    // ESPNow.init();
    // ESPNow.add_peer(receiver_mac);

    // Initialize DHT sensor
    dht.begin();
}

void loop()
{
    // Wait a few seconds between measurements.
    delay(2000);

    if(timeDiff(RECORDING_LAST, RECORDING_DELAY))
    {
        RECORDING_LAST = millis();

        // Take DHT11 readings
        float h = dht.readHumidity();
        float t = dht.readTemperature();

        // Check if any reads failed and exit early (to try again).
        if (isnan(h) || isnan(t))
        {
            Serial.println(F("Failed to read from DHT sensor!"));
            return;
        }

        // assign readings to the struct
        reading.temperature = t;
        reading.humidity = h;
        reading.time = getTimeData();  
        
        // send data using ESPNow
        ESPNow.send_message(receiver_mac, (uint8_t *)&reading, sizeof(reading_struct));
    }
}
    */

    /*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/get-change-esp32-esp8266-mac-address-arduino/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.  
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/
#include <WiFi.h>
#include <esp_wifi.h>

void readMacAddress(){
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
  } else {
    Serial.println("Failed to read MAC address");
  }
}

void setup(){
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin();

  Serial.print("[DEFAULT] ESP32 Board MAC Address: ");
  readMacAddress();
}
 
void loop(){

}