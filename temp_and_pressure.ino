#include <SimpleDHT.h>                   // Data ---> D3 VCC ---> 3V3 GND ---> GND
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID       "Rishi"
#define WLAN_PASS       "123456778"
// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "*****" //username of adafruit io
#define AIO_KEY         "*****" //key of the adafruit io
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");//topic to be subscribed
Adafruit_MQTT_Publish pressure = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/pressure");//topic to be subscribed

int pinDHT11 = 0;
SimpleDHT11 dht11(pinDHT11);
byte pres = 0;  //Stores pressure value
byte temp = 0; //Stores temperature value
void setup() {
  Serial.begin(115200);//the purpose of the serial montior is to check whether the wifi is connected or not and to display the respone code fo the Mqtt

  WiFi.begin(WLAN_SSID, WLAN_PASS);
while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
connect();
}
void connect() {

  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if (ret >= 0)
      mqtt.disconnect();


  }

}

void loop() {

  if (! mqtt.ping(3)) {
    if (! mqtt.connected())
      connect();
  }
  dht11.read(&temp, &pres, NULL);




}
