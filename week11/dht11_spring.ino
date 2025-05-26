#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define DHTPIN 0        
#define DHTTYPE DHT11    

const char* ssid = "U+Net3FD3";
const char* password = "5D833323M@";
//const char* serverUrl = "https://localhost:8080/api/sensors";
const char* serverUrl = "https://unsia.belajarhub.id/spring/api/sensors";  
const char* api_key = "abc123";
const char* deviceId = "device01";

DHT dht(DHTPIN, DHTTYPE);
//WiFiClient client;
WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  dht.begin();

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  client.setInsecure(); // comment out for http protocol
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); 

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(client,serverUrl);
    http.addHeader("Content-Type", "application/json");
    String json = "{\"apikey\":\"" + String(api_key) + "\",";
    json += "\"deviceId\":\"" + String(deviceId) + "\",";
    json += "\"temperature\":" + String(temperature, 1) + ",";
    json += "\"humidity\":" + String(humidity, 1) + "}";  
    // String json = "{\"deviceId\":\"wemos1\",\"temperature\":24.3,\"humidity\":40.5,\"apikey\":\"unsiaiot601\"}";
   
    int httpResponseCode = http.POST(json);

    Serial.print("POST Data: ");
    Serial.println(json);
    Serial.print("Response code: ");
    Serial.println(httpResponseCode);

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(10000); // 10초마다 전송
}
