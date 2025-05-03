// #define FW_VERSION "v0.1.1"
// const char* firmwareVersion = FW_VERSION;
#define BLYNK_TEMPLATE_ID "TMP--------"
#define BLYNK_TEMPLATE_NAME "lolin mini"
#define BLYNK_FIRMWARE_VERSION      "0.1.1"
#define BLYNK_PRINT Serial
#define APP_DEBUG
#include <DHT.h>
#include "BlynkEdgent.h"
#define DHTPIN 0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

#define VPIN_TEMP V0
#define VPIN_HUM  V1

void sendSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (!isnan(h) && !isnan(t)) {
    Serial.print("Temp: "); Serial.print(t); Serial.print(" °C ");
    Serial.print(" Humidity: "); Serial.println(h); Serial.println(" %");
    Blynk.virtualWrite(V0, t);
    Blynk.virtualWrite(V1, h);
  } else {
    Serial.println("DHT read failed");
  }
}

void setup()
{
  Serial.begin(115200);
  delay(100);

  dht.begin();
  delay(300);
  BlynkEdgent.begin();
  // Serial.println();
  // Serial.print("Device IP Address: ");
  // Serial.println(WiFi.localIP());
  timer.setInterval(7000L, sendSensor);  // 5 sec
}

void loop() {
  BlynkEdgent.run();
  delay(300);
  timer.run();
}
