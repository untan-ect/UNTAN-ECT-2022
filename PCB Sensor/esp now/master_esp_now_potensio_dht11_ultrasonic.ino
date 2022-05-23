#include <DHT.h>
#include <esp_now.h>
#include <WiFi.h>

#define DHTTYPE DHT11
#define DHTPIN 4
#define trigPin 26
#define echoPin 25
const int Pot = 35;
int nilaiPot = 0;

DHT dht(DHTPIN, DHTTYPE);
float Temperature;
float Humidity;
long duration;
long distance;
// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x78, 0xE3, 0x6D, 0x18, 0x2D, 0xAC};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int potensio;
  long ultra;
  float Temperature;
  float Humidity;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  pinMode(Pot, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(DHTPIN, INPUT);
  dht.begin();
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6); // untuk menambah penerima cth; 2 penerima jadi broadcastAddress harus 2 misal broadcastAddress dan broadcastAddress2
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Set values to send
  nilaiPot = analogRead(Pot);
  nilaiPot = map(nilaiPot, 0, 4095, 0, 255);
  myData.potensio = nilaiPot;

  digitalWrite(trigPin, HIGH);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  myData.ultra = distance;
  
  Temperature = dht.readTemperature();
  myData.Temperature = Temperature;
  Humidity = dht.readHumidity();
  myData.Humidity = Humidity;

  Serial.println("Nilai Potensio : " + String(nilaiPot));
  Serial.println("Temperature    : " + String(Temperature) + " C");
  Serial.println("Humidity       : " + String(Humidity) + "%");
  Serial.println("Jarak          : " + String(distance) + " CM");

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}
