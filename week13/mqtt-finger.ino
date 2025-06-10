#include <ESP8266WiFi.h>   // for ESP8266 
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <DHT.h>

// Wi-Fi Setting
const char* ssid = "your ssid";
const char* password = "your-pass";

// MQTT Broker Setting
const char* mqtt_server = "your-ip";
const int mqtt_port = 8883;
const char* mqtt_user = "iotuser";
const char* mqtt_pass = "iotuser*01";
const char* mqtt_topic = "unsia/iot";

// Server Certificate Fingerprint (SHA1)
const char* fingerprint = "4E:27:52:98:17:D8:B4:DD:B6:9D:2B:77:0E:21:C7:7C:3C:C8:35:FB"; //replace with your finger print

// DHT Sensor Pin
#define DHTPIN 0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

WiFiClientSecure secureClient;
PubSubClient mqttClient(secureClient);

void setup_wifi() {
  delay(100);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (mqttClient.connect("dhtClient", mqtt_user, mqtt_pass)) {
      Serial.println("MQTT connected");
    } else {
      Serial.print("Connect failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();

  // use fingerprint for server authentication 
  secureClient.setFingerprint(fingerprint);
  mqttClient.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (!isnan(temp) && !isnan(hum)) {
    char payload[64];
    snprintf(payload, sizeof(payload), "{\"temperature\":%.1f,\"humidity\":%.1f}", temp, hum);
    mqttClient.publish(mqtt_topic, payload);
    Serial.println(payload);
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }

  delay(10000); // 10 sec
}
