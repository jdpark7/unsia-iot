#include <ESP8266WiFi.h>            
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <DHT.h>

// Wi-Fi 
const char* ssid = "your-ssid";
const char* password = "your-password";

// MQTT 
const char* mqtt_server = "iot.unsia.local";
const int mqtt_port = 1883;
const char* mqtt_user = "unsia";
const char* mqtt_pass = "unsia*01";
const char* mqtt_topic = "unsia/iot";

// DHT sensor
#define DHTPIN 0            // pin number for DHT11 sensor data
DHT dht(DHTPIN, DHTTYPE);

#include "rootCA.der.h"  // xxd -i 

WiFiClient plainClient;
BearSSL::WiFiClientSecure secureClient;

BearSSL::X509List cert(rootCA_der, rootCA_der_len);  // 

PubSubClient* mqttClient; 

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
  while (!mqttClient->connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient->connect("dhtClient", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=" );
      Serial.print(mqttClient->state());
      Serial.print("\n" );
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();

  if (mqtt_port == 8883) {
    BearSSL::X509List cert(rootCA_der, rootCA_der_len);
    secureClient.setTrustAnchors(&cert);
    secureClient.setBufferSizes(512, 512); 
    mqttClient = new PubSubClient(secureClient);
  } else {
    mqttClient = new PubSubClient(plainClient);
  }
  mqttClient->setServer(mqtt_server, mqtt_port);

}

void loop() {
  if (!mqttClient->connected()) {
    reconnect();
  }
  mqttClient->loop();

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (!isnan(temp) && !isnan(hum)) {
    char payload[64];
    snprintf(payload, sizeof(payload), "{\"temperature\":%.1f,\"humidity\":%.1f}", temp, hum);
    mqttClient->publish(mqtt_topic, payload);
    Serial.println(payload);
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }

  delay(10000); // 10 sec
}
