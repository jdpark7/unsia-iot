#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>  // use WiFiClientSecure 
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#define DHTPIN 0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#ifndef STASSID
#define STASSID "Your SSID"
#define STAPSK "Your password"
#endif
#define STUDENTID "Your ID"

const char* ssid = STASSID;
const char* password = STAPSK;


const char* server = "https://iot.learnina.org/dht.php";
const char* stationid = STUDENTID;

WiFiClientSecure client;  // HTTPS client

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  dht.begin();

  // ★ only for test
  client.setInsecure();  
  // => skip ssl certificate
  // => In production, use fingerprint or CA cert 

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient https;  // use HTTPClient. (if client is secure, process https)
    String url = String(server) + "?stationid=" + stationid + 
                 "&temperature=" + String(temperature, 1) + 
                 "&humidity=" + String(humidity, 1);

    Serial.print("Requesting URL: ");
    Serial.println(url);

    https.begin(client, url);  // use WiFiClientSecure 

    int httpCode = https.GET(); // GET request
    if (httpCode > 0) {
      String payload = https.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    } else {
      Serial.print("Error on HTTPS request: ");
      Serial.println(httpCode);
    }
    https.end();
  }

  delay(10000); // 10 sec
}
