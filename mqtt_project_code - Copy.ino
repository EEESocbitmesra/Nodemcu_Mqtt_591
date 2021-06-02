
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"



#define WLAN_SSID       "bixby"
#define WLAN_PASS       "...your password..."



#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                  
#define AIO_USERNAME    "...your AIO username..."
#define AIO_KEY         "...your AIO key..." 
int output = 2;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe Light = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Light");

void MQTT_connect();
void setup() {
  Serial.begin(230400);
  delay(10);
  pinMode(2, OUTPUT);
  Serial.println(F("Adafruit MQTT "));

  Serial.println(); Serial.println();
  Serial.print("Connecting to bixby");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  mqtt.subscribe(&Light);
}

uint32_t x=0;

void loop() {
  
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &Light) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light.lastread);
      if(!strcmp((char*)Light.lastread,"ON"))
      {
        digitalWrite(2,LOW);
      }
      else
      {
        digitalWrite(2,HIGH);
      }
    }
  }
}

void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }

  Serial.print("CONNECTING TO THE MQTT SERVER... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { 
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("RETRYING MQTT CONNECTION IN 3 SECONDS ...");
       mqtt.disconnect();
       delay(3000); 
       retries--;
       if (retries == 0) {
         
         while (1);
       }
  }
  Serial.println("MQTT CONNECTED :) ");
}
