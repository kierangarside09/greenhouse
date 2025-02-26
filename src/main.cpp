#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <NTPClient.h>
#include "DHT.h"

// https://randomnerdtutorials.com/power-esp32-esp8266-solar-panels-battery-level-monitoring/ have a gander at this webpage for info on solar power, just incase you lose the bookmark, nobhead

#define DHTPIN 14
#define DHTTYPE DHT11

const char *ssid = "TP-Link_9692";
const char *password = "88089423";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

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

class Reading
{
private:
    float temperature;
    float humidity;
    long timestamp;

public:
    Reading() : temperature(0), humidity(0), timestamp(0) {}
    Reading(float temperature, float humidity)
    {
        this->temperature = temperature;
        this->humidity = humidity;
        this->timestamp = getTimeData();
    }

    float getTemperature()
    {
        return this->temperature;
    }

    float getHumidity()
    {
        return this->humidity;
    }

    long getTimestamp()
    {
        return this->timestamp;
    }

    String toString()
    {
        return String(this->temperature) + "," + String(this->humidity) + "," + String(this->timestamp);
    }
};

class Greenhouse
{
private:
    Reading currentReading;
public:
    Greenhouse()
    {
        this->currentReading = Reading(dht.readTemperature(), dht.readHumidity());
    }
};

void setup()
{
    Serial.begin(115200);
    Serial.println(F("DHTxx test!"));

    dht.begin();
}

void loop()
{
    // Wait a few seconds between measurements.
    delay(2000);

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f))
    {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
    }

    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(f);
    Serial.print(F("°F  Heat index: "));
    Serial.print(hic);
    Serial.print(F("°C "));
    Serial.print(hif);
    Serial.println(F("°F"));
}