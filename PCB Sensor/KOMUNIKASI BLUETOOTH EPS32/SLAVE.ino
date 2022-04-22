#include "BluetoothSerial.h"
#include <analogWrite.h>
#include <Adafruit_GFX.h>
#include <ESP32_Servo.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

Servo myservo;

const int led = 33;
int pot, jarak;
float temperature, Humidity;
const int lebar = 128;
const int tinggi = 64;
const int reset  = 4;
Adafruit_SSD1306 oled(lebar, tinggi, &Wire, reset);

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

String Pembacaan;
char bacaData;

void setup() {
  pinMode(led, OUTPUT);
  myservo.attach(13);
  Serial.begin(9600);
  SerialBT.begin("Slave"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();
}
void loop() {
  if (SerialBT.available() > 0) {
    bacaData = SerialBT.read();
    Pembacaan = Pembacaan + bacaData ;
    if (bacaData == '\n') {
      Serial.println("Pembacaan =" + Pembacaan);
      String l = getValue(Pembacaan, ',', 0);
      String m = getValue(Pembacaan, ',', 1);
      String n = getValue(Pembacaan, ',', 2);
      String o = getValue(Pembacaan, ',', 3);

      pot = l.toInt();
      temperature = m.toInt();
      Humidity = n.toInt();
      jarak = o.toInt();

      Pembacaan = "";

    }
  }
  myservo.write(pot);              // tell servo to go to position in variable 'pos'

  // jarak

  if (jarak >= 6) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }

  // Oled
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.setCursor(0, 0);
  oled.print ("Potensio : ");
  oled.println(pot);
  oled.setCursor(0, 10);
  oled.print ("Temperature : ");
  oled.println(temperature);
  oled.setCursor(0, 20);
  oled.print ("Humidity : ");
  oled.println(Humidity);
  oled.setCursor(0, 30);
  oled.print ("jarak : ");
  oled.println(jarak);
  oled.display();

}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring (strIndex[0],strIndex[1]) : "";
}

// 22-04-2022
