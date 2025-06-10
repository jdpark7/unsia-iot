#include <ESP8266WiFi.h>             
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <DHT.h>

// Wi-Fi setting
const char* ssid = "your-ssid";
const char* password = "your-password";

// MQTT broker settung
const char* mqtt_server = "mqtt.belajarhub.id";
const int mqtt_port = 8883;
const char* mqtt_user = "username"; // change with your username
const char* mqtt_pass = "username*01"; // change with your username*01
const char* mqtt_topic = "unsia/unsia/iot"; // Or username/any-topic for you only

// DHT sensor
#define DHTPIN 0            // pin number
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#include "rootCA_der.h"  // xxd -i

WiFiClient plainClient;
BearSSL::WiFiClientSecure secureClient;
PubSubClient mqttClient(secureClient);

BearSSL::X509List cert(rootCA_der, rootCA_der_len);  // 

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
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=" );
      Serial.print(mqttClient.state());
      Serial.print("\n" );
      delay(5000);
    }
  }
}


void setup() {

  Serial.begin(115200);
  dht.begin();
  setup_wifi();

  secureClient.setTrustAnchors(&cert);
  secureClient.setBufferSizes(512, 512); 
  secureClient.setInsecure();
  Serial.print("CA cert length: ");
  Serial.println(rootCA_der_len);

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

