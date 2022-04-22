#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
const char* ssid = "************";
const char* password = "***********";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

String DaftarHari[7]={"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu"};
String DaftarBulan[12]={"Jan", "Feb", "Mar", "Apr", "Mei", "Jun", "Jul", "Agu", "Sep", "Okt", "Nov", "Des"};

String openWeatherMapApiKey = "b17a0cc7c6807e9691247d3cb72fff62";
String city = "Pontianak";
String countryCode = "ID";
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;
String jsonBuffer;

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Terhubung ke WiFi");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Terhubung ke WiFi");
  Serial.println("Alamat IP: ");
  Serial.println(WiFi.localIP());
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Terputus dari WiFi");
  Serial.print("Alasan: ");
  Serial.println(info.disconnected.reason);
  Serial.println("Mencoba menghubungkan");
  WiFi.begin(ssid, password);
}

void setup(){
  Serial.begin(115200);

  WiFi.disconnect(true);
  delay(1000);

  WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, SYSTEM_EVENT_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);

  /* Remove WiFi event
  Serial.print("WiFi Event ID: ");
  Serial.println(eventID);
  WiFi.removeEvent(eventID);*/

  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println("Wait for WiFi... ");
  timeClient.begin();
  timeClient.setTimeOffset(25200); 
}

void loop(){
  timeClient.update();

  unsigned long epochTime = timeClient.getEpochTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);

  int Jam = timeClient.getHours();
  Serial.print("Jam: ");
  Serial.println(Jam);  

  int Menit = timeClient.getMinutes();
  Serial.print("Menit: ");
  Serial.println(Menit); 
   
  int Detik = timeClient.getSeconds();
  Serial.print("Detik: ");
  Serial.println(Detik);  

  String Hari = DaftarHari[timeClient.getDay()];
  Serial.print("Hari: ");
  Serial.println(Hari);    

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int Tanggal = ptm->tm_mday;
  Serial.print("Tanggal: ");
  Serial.println(Tanggal);

  int Bulan_ke = ptm->tm_mon+1;
  String Bulan = DaftarBulan[Bulan_ke-1];
  Serial.print("Bulan: ");
  Serial.println(Bulan);

  int Tahun = ptm->tm_year+1900;
  Serial.print("Tahun: ");
  Serial.println(Tahun);

  String SekarangJam = timeClient.getFormattedTime();
  Serial.print("Sekarang Jam: ");
  Serial.println(SekarangJam);  

  String SekarangTanggal = String(Tanggal) + " " + String(Bulan) + " " + String(Tahun);
  Serial.print("Sekarang tanggal: ");
  Serial.println(SekarangTanggal);

if ((millis() - lastTime) > timerDelay) {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;

      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(jsonBuffer);
      JSONVar myObject = JSON.parse(jsonBuffer);

      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }

      Serial.print("JSON object = ");
      Serial.println(myObject);
      Serial.print("Temperature: ");
      Serial.println(myObject["main"]["temp"]);
      Serial.print("Pressure: ");
      Serial.println(myObject["main"]["pressure"]);
      Serial.print("Humidity: ");
      Serial.println(myObject["main"]["humidity"]);
      Serial.print("Wind Speed: ");
      Serial.println(myObject["wind"]["speed"]);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  Serial.println("");
  delay(1000);
}


String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
