#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Your SSID";
const char* password = "Your Password";

const char* host = "api.thingspeak.com";
String apiKey = "Your Api Key"; // get from ThingSpeak

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
  delay(15000); // ThingSpeak recommends more than 15 sec
}
