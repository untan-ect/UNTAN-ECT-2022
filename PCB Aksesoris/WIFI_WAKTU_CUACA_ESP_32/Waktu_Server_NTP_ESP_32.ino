#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char *ssid     = "AMS";
const char *password = "87654321";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

String DaftarHari[7]={"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu"};
String DaftarBulan[12]={"Jan", "Feb", "Mar", "Apr", "Mei", "Jun", "Jul", "Agu", "Sep", "Okt", "Nov", "Des"};

void setup() {
  Serial.begin(115200);
    
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

// Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(25200);    // Set offset time in seconds to adjust for your timezone, for example: WIB = 7 x 3600 = 25200
}

void loop() {
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

  Serial.println("");
  delay(1000);
}
