#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>  // WiFiClientSecure 사용
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#define DHTPIN 0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#ifndef STASSID
#define STASSID "U+Net3FD3"
#define STAPSK "5D833323M@"
#endif
#define STUDENTID "123456"

const char* ssid = STASSID;
const char* password = STAPSK;


const char* server = "https://iot.learnina.org/dht.php";
const char* stationid = STUDENTID;

WiFiClientSecure client;  // HTTPS용 클라이언트

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

  // ★ 서버 인증 무시 설정 (테스트용)
  client.setInsecure();  
  // => 이 코드는 서버 SSL 인증서 확인을 생략합니다.
  // => production에서는 fingerprint나 CA cert 등록하는 것이 안전합니다.
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient https;  // HTTPClient 그대로 사용합니다. (client가 secure면 https 자동 처리)
    String url = String(server) + "?stationid=" + stationid + 
                 "&temperature=" + String(temperature, 1) + 
                 "&humidity=" + String(humidity, 1);

    Serial.print("Requesting URL: ");
    Serial.println(url);

    https.begin(client, url);  // WiFiClientSecure 사용

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

  delay(10000); // 10초마다 전송
}