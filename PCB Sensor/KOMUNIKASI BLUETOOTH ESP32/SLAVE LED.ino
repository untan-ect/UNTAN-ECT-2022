#include <Arduino.h>
#include "BluetoothSerial.h"
#include <analogWrite.h>

int led = 33;
int A;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

String Pembacaan;
char bacaData;

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  SerialBT.begin("Slave"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}
void loop() {
  if (SerialBT.available() > 0) {
    bacaData = SerialBT.read();
    Pembacaan += bacaData ;
    if (bacaData == '\n') {
      A = Pembacaan.toInt();
      analogWrite(led, A);
      Pembacaan = "";
      Serial.println("Data A = " + String (A));
    }
  }
}

// 23-04-2022
