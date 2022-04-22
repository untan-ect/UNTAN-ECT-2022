#include <DHT.h>
#include <analogWrite.h>
#include "BluetoothSerial.h"
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
BluetoothSerial SerialBT;
String MACadd = "3C:61:05:2F:E0:0E";
uint8_t address[6]  = {0x3C, 0x61, 0x05, 0x2F, 0xE0, 0x0E};
String name = "Slave";
char *pin = "1234"; //<- standard pin would be provided by default
bool connected;

void setup() {
  Serial.begin(9600);
  SerialBT.begin("Slave", true);
  pinMode(pinPot, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.println();
  pinMode(DHTPIN, INPUT);
  dht.begin();
  Serial.println("The device started in master mode, make sure remote BT device is on!");
  //connected = SerialBT.connect(name);
  connected = SerialBT.connect(address);

  if (connected) {
    Serial.println("Connected Succesfully!");
  } else {
    while (!SerialBT.connected(10000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app.");
    }
  }
  SerialBT.connect();
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
  sensor();
  Serial.println();
  Serial.println("Nilai Potensio : " + String(nilaiPot));
  Serial.println("Temperature    : " + String(Temperature) + " C");
  Serial.println("Humidity       : " + String(Humidity) + "%");
  Serial.println("Jarak          : " + String(distance) + " CM");
  Kirim_data = String(nilaiPot) + "," + String(Temperature) + "," + String(Humidity) + "," + String(distance);
  SerialBT.println(Kirim_data);
  //delay(500);
}

// 22 april 2022
