#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define DHTPIN D4          // DHT11 data pin connected to D4 (GPIO2)
#define DHTTYPE DHT11      // Define DHT sensor type

const char* ssid = "YOUR_WIFI_SSID";                // Your Wi-Fi SSID
const char* password = "YOUR_WIFI_PASSWORD";        // Your Wi-Fi Password
const char* serverUrl = "http://yourdomain.com/save_data.php"; // Server endpoint to receive data

DHT dht(DHTPIN, DHTTYPE);   // Create DHT sensor object

void setup() {
  Serial.begin(115200);      // Start serial communication
  WiFi.begin(ssid, password); // Connect to Wi-Fi
  dht.begin();               // Initialize DHT sensor

  // Wait until Wi-Fi is connected
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void loop() {
  float humidity = dht.readHumidity();       // Read humidity value
  float temperature = dht.readTemperature(); // Read temperature in Celsius

  // Check if reading is successful
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // If Wi-Fi is still connected
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);                          // Specify destination for POST
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Prepare POST data
    String postData = "temp=" + String(temperature) + "&hum=" + String(humidity) + "&device=wemos1";

    // Send the POST request
    int httpResponseCode = http.POST(postData);

    Serial.print("POST Data: ");
    Serial.println(postData);
    Serial.print("Response code: ");
    Serial.println(httpResponseCode);

    http.end();  // Close HTTP connection
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(10000); // Wait 10 seconds before next reading
}