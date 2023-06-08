#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <config.hpp>
#include <DHT.h>
#include <HTTPClient.h>

DHT dht = DHT(GPIO_NUM_23, DHT11);
int lastDataIndex = 0;
int lastReadingTimestamp = 0;
int lastDataSentTimestamp = 0;

class Data
{
public:
  float temperature;
  float humidity;
  Data() {}
  Data(float temperature, float humidity)
  {
    this->temperature = temperature;
    this->humidity = humidity;
  }
};

Data readings[60] = {};

void readData()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  Serial.print("temp ");
  Serial.println(temperature);
  Serial.print("hum ");
  Serial.println(humidity);
  readings[lastDataIndex] = Data(temperature, humidity);
  lastDataIndex++;
}

void sendData()
{
  digitalWrite(GPIO_NUM_2, HIGH);
  Data averageData = Data(0.0f, 0.0f);
  for (int i = 0; i < lastDataIndex; i++)
  {
    averageData.temperature += readings[i].temperature;
    averageData.humidity += readings[i].humidity;
  }
  averageData.temperature /= lastDataIndex + 1;
  averageData.humidity /= lastDataIndex + 1;
  Serial.print("average temp ");
  Serial.println(averageData.temperature);
  Serial.print("average hum ");
  Serial.println(averageData.humidity);
  lastDataIndex = 0;

  WiFiClientSecure *client = new WiFiClientSecure;
  HTTPClient https;
  client->setCACert(CA_CERT);
  https.begin(*client, API_URL);
  https.addHeader("Content-Type", "application/json");
  https.addHeader("authorization", API_AUTH_KEY);

  String json = "{";
  json += String("\"sensorlocation\":") + "\"bathroom\"" + ",";
  // json += String("\"sensorlocation\":") + "\"living room\"" + ",";
  json += "\"temperature\":" + String(averageData.temperature) + ",";
  json += "\"humidity\":" + String(averageData.humidity) + ",";
  json += String("\"soilhumidity\":") + "null";
  json += "}";

  Serial.println(json);

  int httpResponseCode = https.POST(json);
  delay(100);
  Serial.println("Response: " + String(httpResponseCode));
  https.end();
  digitalWrite(GPIO_NUM_2, LOW);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Booting");

  pinMode(GPIO_NUM_2, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID.c_str(), PWD.c_str());
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // ArduinoOTA.setPassword("esp32iot");

  ArduinoOTA
      .onStart([]()
               {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type); })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed"); });

  ArduinoOTA.begin();

  Serial.println("OTA & WIFI ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  dht.begin();
  Serial.println("DHT ready");
}

void loop()
{
  ArduinoOTA.handle();
  if (millis() > lastDataSentTimestamp + 60 * 1000)
  {
    sendData();
    lastDataSentTimestamp = millis();
  }
  else if (millis() > lastReadingTimestamp + 1000)
  {
    readData();
    lastReadingTimestamp = millis();
  }
}