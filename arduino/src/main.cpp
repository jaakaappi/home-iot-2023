#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <config.hpp>
#include <DHT.h>
#include <HTTPClient.h>

#define DATA_SEND_INTERVAL_MS 15 * 60 * 1000

DHT dht = DHT(GPIO_NUM_23, DHT11);
int lastDataSentTimestamp = 0;

float measureAverageTemperature(int count)
{
  float readings = 0;
  for (size_t i = 0; i < count; i++)
  {
    readings += dht.readTemperature();
    delay(100);
  }
  return readings / count;
}

float measureAverageHumidity(int count)
{
  float readings = 0;
  for (size_t i = 0; i < count; i++)
  {
    readings += dht.readHumidity();
  }
  return readings / count;
}

void sendData()
{
  digitalWrite(GPIO_NUM_2, HIGH);
  float temperature = measureAverageTemperature(10);
  float humidity = measureAverageHumidity(10);

  Serial.print("temp ");
  Serial.println(temperature);
  Serial.print("hum ");
  Serial.println(humidity);

  WiFiClientSecure *client = new WiFiClientSecure;
  HTTPClient https;
  client->setCACert(CA_CERT);
  https.begin(*client, API_URL);
  https.addHeader("Content-Type", "application/json");
  https.addHeader("authorization", API_AUTH_KEY);

  String json = "{";
  json += String("\"sensorlocation\":") + "\"living room\"" + ",";
  json += "\"temperature\":" + String(temperature) + ",";
  json += "\"humidity\":" + String(humidity) + ",";
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
  float temperature = measureAverageTemperature(10);
  float humidity = measureAverageHumidity(10);
  Serial.print("temp ");
  Serial.println(temperature);
  Serial.print("hum ");
  Serial.println(humidity);
}

void loop()
{
  ArduinoOTA.handle();
  if (millis() > lastDataSentTimestamp + DATA_SEND_INTERVAL_MS)
  {
    sendData();
    lastDataSentTimestamp = millis();
  }
}