#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define DHTPIN 0        // DHT11 data pin
#define DHTTYPE DHT11    // DHT11 type

const char* ssid = "Your SSID";
const char* password = "Your Password";
const char* serverUrl = "https://your-subdomain.belajarhub.id/save_data.php";  // your server address

DHT dht(DHTPIN, DHTTYPE);
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
  client.setInsecure();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // 

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(client,serverUrl);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String postData = "temp=" + String(temperature) + "&hum=" + String(humidity) + "&apikey=123456&device_id=wemos1";
    int httpResponseCode = http.POST(postData);

    Serial.print("POST Data: ");
    Serial.println(postData);
    Serial.print("Response code: ");
    Serial.println(httpResponseCode);

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(10000); // send data in every 10 sec
}
