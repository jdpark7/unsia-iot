#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "U+Net3FD3";
const char* password = "5D833323M@";

const char* host = "api.thingspeak.com";
String apiKey = "UHO227M9UUTT667Y"; // ThingSpeak에서 복사

#define DHTPIN 0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  dht.begin();
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (!isnan(h) && !isnan(t)) {
    WiFiClient client;
    HTTPClient http;

    String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                 "&field1=" + String(t) + "&field2=" + String(h);
    http.begin(client, url);
    int httpCode = http.GET();
    http.end();
  }
  delay(15000); // ThingSpeak는 15초 간격 이상 전송 권장
}
