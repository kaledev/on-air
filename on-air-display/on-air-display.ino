#include <WiFi.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include "iot.h"

unsigned long previousMillis = 0;
unsigned long interval = 30000;

WiFiClient net;
MQTTClient client;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
int led_color = strip.Color(0, 150, 0);

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');

    strip.clear();
    led_color = strip.Color(0, 160, 255); // Blue
    for (int i = 0; i < LED_COUNT; i++) {
      strip.setPixelColor(i, led_color);
      strip.show();
    }
    
    delay(500);
  }
  
  strip.clear();
  strip.show();
  
  Serial.print("\nIp: ");
  Serial.println(WiFi.localIP());
}

void initIOT()
{
  client.begin(IOT_ENDPOINT, net);
  Serial.print("Connecting to MQTT ..");
  while (!client.connect(THINGNAME)) {
    Serial.print(".");

    strip.clear();
    led_color = strip.Color(255, 251, 0); // Yellow
    for (int i = 0; i < LED_COUNT; i++) {
      strip.setPixelColor(i, led_color);
      strip.show();
    }
    delay(100);
  }
  
  strip.clear();
  strip.show();

  client.onMessage(onMqttMessage);
  // Subscribe to a topic
  client.subscribe(IOT_SUBSCRIBE_TOPIC);

  Serial.println("MQTT Connected!");
  Serial.print("Broker IP: ");
  Serial.println(IOT_ENDPOINT);
}

void initLed()
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void setup() {
  Serial.begin(115200);
  
  initLed();

  // WIFI Connection
  initWiFi();

  // IOT Connection
  initIOT();
}

void loop() {
  client.loop();

   // if WiFi is down, try reconnecting
  if (WiFi.status() != WL_CONNECTED){
    initWiFi();
  }

  // if MQQT is down, try reconnecting
  if (!client.connected()){
    initIOT();
  }
}

void onMqttMessage(String &topic, String &payload) {
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);

  int onair = doc["onair"];
  stateChange(onair);
  Serial.println();
}

void stateChange(int state) {
  //On-Off Air LED Trigger

  char jsonBuffer[512];

  if (state == 1) {
    Serial.println("ON AIR TRIGGER");
    strip.clear();
    led_color = strip.Color(255, 0, 0);
    for (int i = 0; i < LED_COUNT; i++) {
      strip.setPixelColor(i, led_color);
      strip.show();
    }
  }
  else {
    state = 0;
    Serial.println("OFF AIR TRIGGER");
    led_color = strip.Color(0, 0, 0);
    strip.clear();
    for (int i = 0; i < LED_COUNT; i++) {
      strip.setPixelColor(i, led_color);
      strip.show();
    }
  }

}
