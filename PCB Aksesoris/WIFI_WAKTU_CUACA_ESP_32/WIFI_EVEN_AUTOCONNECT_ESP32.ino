#include <WiFi.h>
 
const char* ssid = "***********";
const char* password = "***********";

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Berhasil terhubung ke WiFi");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("WiFi terhubung");
  Serial.println("Alamat IP: ");
  Serial.println(WiFi.localIP());
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.print("WiFi terputus. Alasan: ");
  Serial.println(info.disconnected.reason);
  Serial.println("Menyambungkan kembali..");
  WiFi.begin(ssid, password);
}

void setup(){
  Serial.begin(115200);
  WiFi.disconnect(true);
  delay(1000);

  WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, SYSTEM_EVENT_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);

  /* 
  Remove WiFi event
   Serial.print("WiFi Event ID: ");
   Serial.println(eventID);
   WiFi.removeEvent(eventID);
  */

  WiFi.mode(WIFI_STA);                                          // Berperan sebagai stasiun WiFi                                                                                  
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Alamat IP: ");
  Serial.println(WiFi.localIP());

}

void loop(){
  delay(1000);
}

// Fadli Amman Siwaka - 20 April 2022
