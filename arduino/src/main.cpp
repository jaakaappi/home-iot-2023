#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <config.hpp>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <HTTPClient.h>

#define DATA_SEND_INTERVAL_MS 15 * 60 * 1000

Adafruit_BME280 bme;

int lastDataSentTimestamp = 0;
unsigned long previousMillis = 0;
unsigned long interval = 30000;

void sendData()
{
  digitalWrite(GPIO_NUM_2, HIGH);
  bme.takeForcedMeasurement();
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  Serial.print("temp ");
  Serial.println(temperature);
  Serial.print("hum ");
  Serial.println(humidity);

  WiFiClient *client = new WiFiClient;
  HTTPClient http;
  http.begin(*client, API_URL);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("authorization", API_AUTH_KEY);

  String json = "{";
  json += String("\"sensorlocation\":") + "\"living room\"" + ",";
  json += "\"temperature\":" + String(temperature) + ",";
  json += "\"humidity\":" + String(humidity) + ",";
  json += String("\"soilhumidity\":") + "null";
  json += "}";

  Serial.println(json);

  int httpResponseCode = http.POST(json);
  delay(100);
  Serial.println("Response: " + String(httpResponseCode));
  http.end();
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
    WiFi.reconnect();
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

  bme.begin();
  Serial.println("BME280 ready");
  bme.takeForcedMeasurement();
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  Serial.print("temp ");
  Serial.println(temperature);
  Serial.print("hum ");
  Serial.println(humidity);
}

void loop()
{
  unsigned long currentMillis = millis();
  // if WiFi is down, try reconnecting
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval))
  {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillis = currentMillis;
  }
  ArduinoOTA.handle();
  if (millis() > lastDataSentTimestamp + DATA_SEND_INTERVAL_MS)
  {
    sendData();

    Serial.print("Millis since last measurement: ");
    Serial.println(millis() - lastDataSentTimestamp);
    lastDataSentTimestamp = millis();
  }
}