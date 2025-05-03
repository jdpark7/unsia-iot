// 시리얼모니터로 온도 습도 출력

#include "DHT.h"
#define DHTPIN 0
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);

float humidityData;
float temperatureData;

void setup()
{
 Serial.begin(115200);
 delay(10);
 dht.begin();

}
void loop()
{ 
  humidityData = dht.readHumidity();
  temperatureData = dht.readTemperature(); 
    Serial.print("Humidity=");
    Serial.print(humidityData);
    Serial.print("     Temperature=");
    Serial.println(temperatureData);
  delay(3000); // interval
 }