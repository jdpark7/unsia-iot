#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>  // WiFiClientSecure 사용
#include <ESP8266HTTPClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <DHT.h>

#define DHTPIN 0
#define DHTTYPE DHT11
#define STUDENTID "123456"
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Your SSID";
const char* password = "Your Password";
const char* dataserver = "https://iot.learnina.org/dht.php";
const char* stationid = STUDENTID;

const uint8_t fingerprint[] = {----------------};

AsyncWebServer server(80);
// WiFiClientSecure client; 
std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  client->setFingerprint(fingerprint);
  // client->setCACert(root_ca);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  dht.begin();

  // ★ 서버 인증 무시 설정 (테스트용)
  // client.setInsecure();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am ESP8266.");
  });

  server.begin();
  Serial.println("HTTP server started");

  ElegantOTA.begin(&server,"admin","iot601");    // Start ElegantOTA
}

void loop(void) {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient https;  // HTTPClient 그대로 사용합니다. (client가 secure면 https 자동 처리)
    String url = String(dataserver) + "?stationid=" + stationid + 
                 "&temperature=" + String(temperature, 1) + 
                 "&humidity=" + String(humidity, 1);

    Serial.print("Requesting URL: ");
    Serial.println(url);

    https.begin(*client, url);  // WiFiClientSecure 사용

    int httpCode = https.GET(); // GET 요청
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
  ElegantOTA.loop();
  delay(10000);

}

