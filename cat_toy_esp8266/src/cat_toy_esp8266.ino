#include <Arduino.h>

#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
WiFiServer server(1337);

void printWiFiStatus();

void setup(void) {
  Serial.begin(9600);

Serial.println("Hello!");
  // Configure GPIO2 as OUTPUT.

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
   Serial.println("failed to connect, we should reset as see if it connects");
   delay(3000);
   ESP.reset();
   delay(5000);
 }

printWiFiStatus();

  // Start TCP server.
  server.begin();


}

void loop(void) {

  WiFiClient client = server.available();

  if (client) {
    Serial.println("Client connected.");

    while (client.connected()) {
      if (client.available()) {
        char command = client.read();
        if (command == 'H') {
          Serial.println("manual");
          Serial.println("left");
        }
        else if (command == 'L') {
          // digitalWrite(ledPin, LOW);
          Serial.println("LED is now off.");
        }
      }
    }
    Serial.println("Client disconnected.");
    client.stop();
  }
}

void printWiFiStatus() {
  Serial.println("");
  Serial.print("Connected to ");
  // Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
