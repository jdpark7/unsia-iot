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

const char* ssid = "U+Net3FD3";
const char* password = "5D833323M@";
const char* dataserver = "https://iot.learnina.org/dht.php";
const char* stationid = STUDENTID;
const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIFazCCA1OgAwIBAgISA6RjD1pHEQVzEf2Lzh+6TS//MA0GCSqGSIb3DQEBCwUA\n" \
"MEoxCzAJBgNVBAYTAlVTMRMwEQYDVQQIDApDYWxpZm9ybmlhMRMwEQYDVQQHDApD\n" \
"YWxpZm9ybmlhMRowGAYDVQQKDBFMZXQncyBFbmNyeXB0IExEQzAeFw0yMTA2MDEx\n" \
"NzI1MThaFw0yNTA1MzAxNzI1MThaMEoxCzAJBgNVBAYTAlVTMRMwEQYDVQQIDApD\n" \
"YWxpZm9ybmlhMRMwEQYDVQQHDApDYWxpZm9ybmlhMRowGAYDVQQKDBFMZXQncyBF\n" \
"bmNyeXB0IExEQzCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAKRjMdpL\n" \
"u4FY2e5LrhOnvIB6p6C2wShvqJ4lTYpR1Nz4XyrfqTltvJEtTZjeTO9AtS+Xw3qT\n" \
"Nm6cZlBKC0OHKzHVQdpmY9ldMF4mLPgqTPb6cCq8ap4yD1JkqRsvCuLUX1av7IYi\n" \
"ggsSK2o7x5bpIK2lg7pAeImZjPU+1rR5cLZ/hZMLFgpHofON+uRS5pV+jMJG8wYy\n" \
"x3oD5fFJm6cI74Ekk7n3A2coCdt7neMYvG6SIXc5ho+G49TfJX/W/kqVnyN3zMVG\n" \
"qlOlEOsNByN+BULR07R9rtd1RukEmsiRG4BIMJKsq9D8VdfH3iP1WBp84B+8E5cE\n" \
"Q1eDPo8A3MQ/EmYQJ8NkG5rxJQDeXUpF9gId5lGLKFXaIhSmlfzyu5/Z3QfVYImT\n" \
"2pBgv//qv3xMBc01S9ByNgd1PpytS/vIQTAHgbg79b82SmWrkMg3+HGD3rg4C3eX\n" \
"lJj0fWqWDWXLU2lhqGMG5VE4hnm4JMiV2VoYV0t2t7CBBT6cN5hPflD+Pl9sP0ok\n" \
"twgmAKC3dwk/CG1PpKZc2Eb5fGsPff61NPBv0WrZzQd2GFOeua7T58XxzRCPXwAA\n" \
"ucXMG8PddFEm3X8VdnDQA9SxkSDPMCWuIpX89Q9xqZ1M1eU0rGEAOoHzSr0GOF8v\n" \
"tzgS4cWjZ3rzpD2uNZoRMkZ6XLf/nQIDAIYYoDANBgkqhkiG9w0BAQsFAAOCAgEA\n" \
"exSWSzehN9+d8eMeGUpiRzScnTqetQtw5FY4h7+gQAKFvbhZhwXN4G2VZ9X5+Xt3\n" \
"PVZ+7EVz64Nl+d6rb5WiPhU46r5pXltV21htW7/cpY86OSgeZY8U0dZy8U/0uG2N\n" \
"2m7MR0IgwqUm8h+SSgGH4kEQAl3/6nJbGWrOr/VIl4B7d6XolC/Uox4zJeOX8kkH\n" \
"OYO4+RRX6D8V8FtM2s77IuCNoVmK96WBiEedxESzmhFToCIE5UptR8CJ6Bq/Rrr/\n" \
"qRCIRlo5t6ptJRODCRavXY4DYoexWeC9rq8jRtqzjPAKUpPHKcDrcMr18isGGRWR\n" \
"RveCE6XHhx/ER82p6Nz1qlu96B/k5yq96aH6rwO6uDYHoRPlb8LvDdHOgPqBF4oA\n" \
"y1Jw0NB5VxgfFw7egD1ky+Sr9F0UvqI15Qy7yK5fxmd6UkJ++9/cE5TR3h2KHnc5\n" \
"YLeYHSkmF4ttBvD7AkZ6J52N9xJb9OngM9kH4+fyY31DEvVu+Bhp0wV+nNglw4O3\n" \
"fbIR6eR4A8u1dAuhs+QmlHzHJ/mzSgHJl0nEm4MTTwARwYZpEF5v5Vvh1N2LQO+9\n" \
"fPB3y/yxRtQoIT91E4O0/51UwBjv58F0whmMm3dHqPyRhpDmmvmFbOdnATzRb8sq\n" \
"2APb2pCtvXDYBVu02lFzqS1gdqtchmt4t8lE6cRWhtDnwwk=\n" \
"-----END CERTIFICATE-----\n";

const uint8_t fingerprint[] = {0x5D,0x07,0x6E,0xB5,0x48,0xE8,0x81,0x1F,0x2A,0x42,0x2C,0x14,0x65,0x20,0x23,0xD3,0x1F,0x6C,0xAC,0x85};

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

