#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const int Pot = 35;
int nilaiPot = 0;

// Set these to your desired credentials.
const char *ssid = "ESP32 Kontrol";
const char *password = "coba_aja";

unsigned long oldtime[2];
String h, j;
WiFiServer server(409500);


void setup() {
  Serial.begin(115200);
  pinMode(Pot, INPUT);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients
  nilaiPot = analogRead(Pot);
  nilaiPot = map(nilaiPot, 0, 4095, 0, 255);
  Serial.print("nilai potensio:");
  Serial.println(nilaiPot);
  client.println(nilaiPot);
  //client.stop();
}
