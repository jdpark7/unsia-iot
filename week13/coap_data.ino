#include <ESP8266WiFi.h>
#include <DHT.h>
#include <coap-simple.h>  // use CoAP Simple library
#include <WiFiUdp.h>


#define DHTPIN 0           // DHT11 sensor pin
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "your-ssid";
const char* password = "your-password";

const char* coap_server = "192.168.x.x";  // IP of CoAP server

const int coap_port = 5683;

WiFiUDP udp;

void callback_response(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[Coap Response got]");
  
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  
  Serial.println(p);
}

Coap coap(udp);

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(" connected!");
  coap.response(callback_response);  
  coap.start();
}

void loop() {
  IPAddress serverIP;
  serverIP.fromString(coap_server);  
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT");
    delay(2000);
    return;
  }

  int msgid;

  String payload = "{\"temperature\":";
  payload += t;
  payload += ",\"humidity\":";
  payload += h;
  payload += "}";

  Serial.println("Sending CoAP POST...");
  Serial.println(payload);

  msgid=coap.post(serverIP, coap_port, "sensor/data", payload.c_str(), payload.length());


  coap.loop(); 
  
  delay(10000);  // send every 10 seconds
}
