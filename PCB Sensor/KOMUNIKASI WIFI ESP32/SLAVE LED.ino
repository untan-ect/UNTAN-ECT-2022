#include <WiFi.h>
#include <WiFiClient.h>
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

//#define LED_BUILTIN 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

const char* ssid = "ESP32 Kontrol";
const char* pass = "coba_aja";

const uint16_t port = 409500;
const char * host = "192.168.4.1";


WiFiClient client;

void setup() {
  pinMode(led, OUTPUT);
  myservo.attach(13);
  Serial.begin(115200);
  delay(10);
  WiFi.begin(ssid, pass);
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();

  Serial.print("Connecting.");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.print("WiFi connected - IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Connecting to ");
  Serial.println(host);

  if (client.connect(host, port)) {
    Serial.println("Sending data"); // printed on serial monitor
  }
  delay(100);
}

void loop()
{
  String line;
  while (client.connected()) {
    line = client.readString();             // read a byte, the
    String l = getValue(line, ',', 0);
    String m = getValue(line, ',', 1);
    String n = getValue(line, ',', 2);
    String o = getValue(line, ',', 3);
    pot = l.toInt();
    temperature = m.toInt();
    Humidity = n.toInt();
    jarak = o.toInt();
    Serial.println("Line 1 : " + String(pot));
    Serial.println("Line 2 : " + String(temperature));
    Serial.println("Line 3 : " + String(Humidity));
    Serial.println("Line 4 : " + String(jarak));
    Serial.println();
  }

  if (!client.connect(host, port)) {
    Serial.println("Closing connection.");
    client.stop();
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

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

// 23-04-2022
