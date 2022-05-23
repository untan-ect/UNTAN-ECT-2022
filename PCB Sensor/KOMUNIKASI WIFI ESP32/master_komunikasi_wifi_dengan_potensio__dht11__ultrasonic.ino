#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define DHTTYPE DHT11
#define DHTPIN 4
#define trigPin 26
#define echoPin 25
#define pinPot 35
int nilaiPot = 0;
String Kirim_data;
DHT dht(DHTPIN, DHTTYPE);
float Temperature;
float Humidity;
long duration;
long distance;
// Set these to your desired credentials.
const char *ssid = "ESP32 Kontrol";
const char *password = "coba_aja";

WiFiServer server(409500);


void setup() {
  Serial.begin(115200);
  pinMode(pinPot, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(DHTPIN, INPUT);
  dht.begin();
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
void sensor() {
  nilaiPot = analogRead(pinPot);
  nilaiPot = map(nilaiPot, 0, 4095, 0, 180);
  Temperature = dht.readTemperature();
  Humidity = dht.readHumidity();
  digitalWrite(trigPin, HIGH);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients
  sensor();
  Serial.println();
  Serial.println("Nilai Potensio : " + String(nilaiPot));
  Serial.println("Temperature    : " + String(Temperature) + " C");
  Serial.println("Humidity       : " + String(Humidity) + "%");
  Serial.println("Jarak          : " + String(distance) + " CM");
  Kirim_data = String(nilaiPot) + "," + String(Temperature) + "," + String(Humidity) + "," + String(distance);
  client.println(Kirim_data);
  //client.stop();
}
