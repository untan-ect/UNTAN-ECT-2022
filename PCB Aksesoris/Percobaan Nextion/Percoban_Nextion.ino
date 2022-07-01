#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "time.h"

const char* ssid = "Valdi Honor";                                       
const char* password = "mymommy64";

String openWeatherMapApiKey = "b23308e70faf9521f7bce63d9ecc393e";
String city = "Pontianak";
String countryCode = "ID";

const char* ntpServer = "pool.ntp.org";
const long  WaktuGMT = 3600*7;
const int   PengaturanMusimPanas = 0;

unsigned long WTCuaca, WTWaktu, WTDisplay = 0;
unsigned long JedaCuaca = 630000;                                    
unsigned long JedaWaktu = 500;
unsigned long JedaDisplay = 950;                                                                       

int E, I, P, W, T;
int V = 53;
int v, NxSenKiri, NxSenKanan, NxStatusWifi = 0;
String jsonBuffer, logo, icon, deskripsi;
String Detik, Menit, Jam, Tanggal, Bulan, Tahun;
String Kec, Kel, Tek, TemC;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NextionDisplay(){    
  if (WiFi.status() == WL_CONNECTED) {
      NextionWrite(); 
      String WaktuSekarang = Tanggal + Bulan + Tahun;
      String NxSekarang = "page1.Tanggal.txt=\""+String(WaktuSekarang)+"\"";
      Serial2.print(NxSekarang);
      NextionWrite();
      String WaktuJam = Jam +":"+ Menit +":"+ Detik;
      String NxJam = "page1.Jam.txt=\""+String(WaktuJam)+"\"";
      Serial2.print(NxJam);
      NextionWrite();

      String NxSuhuDepan = "page1.DerajatSuhu.val="+ TemC;
      Serial2.print(NxSuhuDepan);
      NextionWrite();
      String NxSuhuDalam ="page3.TemCu.txt=\""+String(TemC)+"\"";
      Serial2.print(NxSuhuDalam);
      NextionWrite();
      String NxTek = "page3.TekCu.txt=\""+String(Tek)+" mBar"+"\"";
      Serial2.print(NxTek);
      NextionWrite();
      String NxKel = "page3.KelCu.txt=\""+String(Kel)+" %"+"\"";
      Serial2.print(NxKel);
      NextionWrite();
      String NxKec = "page3.KecCu.txt=\""+String(Kec)+" m/s"+"\"";
      Serial2.print(NxKec);
      NextionWrite();
      
      String NxDeskripsiCuaca = "page3.Cuaca.txt=\""+deskripsi+"\"";;
      Serial2.print(NxDeskripsiCuaca);
      NextionWrite();
      String NxLogoCuaca = "page1.n0.val="+String(logo);
      Serial2.print(NxLogoCuaca);
      NextionWrite();
      Serial2.println();
      NextionWrite();
  }
  NextionWrite();
  String NxPersenBaterai = "page1.PersenBaterai.val=" + String(E);
  Serial2.print(NxPersenBaterai);
  NextionWrite();
  String NxTeBat = "page4.TeBat.txt=\"" + String(V) + " V" + "\"";
  Serial2.print(NxTeBat);
  NextionWrite();
  String NxArusBat = "page4.ArusBat.txt=\"" + String(I) + " A" + "\"";
  Serial2.print(NxArusBat);
  NextionWrite();
  String NxDayBat = "page4.DayBat.txt=\"" + String(P) + " W" + "\"";
  Serial2.print(NxDayBat);
  NextionWrite();
  String NxEnBat = "page4.EnBat.txt=\"" + String(W) + " Wh" + "\"";
  Serial2.print(NxEnBat);
  NextionWrite();

  String NxSuhuMesin = "page6.SuhuMesin.val=" + String(T);
  Serial2.print(NxSuhuMesin);
  NextionWrite();

  String NxKecepatan = "page2.SpeedVal.val=" + String(v);
  Serial2.print(NxKecepatan);
  NextionWrite();
  String NxKananSen = "page2.KananSen.val=" + String(NxSenKanan);
  Serial2.print(NxKananSen);
  NextionWrite();
  String NxKiriSen = "page2.KiriSen.val=" + String(NxSenKiri);
  Serial2.print(NxKiriSen);
  NextionWrite();
  String NxWifi = "page2.wifiVal.val=" + String(NxStatusWifi);
  Serial2.print(NxWifi);
  NextionWrite();
  Serial2.println();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CuacaOpenWeather(){
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
      jsonBuffer = httpGETRequest(serverPath.c_str());
      JSONVar myObject = JSON.parse(jsonBuffer);
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      int KonversiKtoC = int(myObject["main"]["temp"]) - 273;
      TemC = String(KonversiKtoC);
      int Kele = int(myObject["main"]["humidity"]);
      Kel = String(Kele);
      int Teka = int(myObject["main"]["pressure"]);
      Tek = "  "+String(Teka);
      int Kece = int(myObject["wind"]["speed"]);
      Kec = String(Kece);

      icon = myObject["weather"][0]["icon"];
       if(icon == "01d"){
          logo = "0";
          deskripsi = "CERAH";
          }
       if(icon == "09d" || icon == "09n" || icon == "02d" || icon == "02n" || icon == "03d" || icon == "03n" || icon == "04d" || icon == "04n"){
          logo = "1";
          deskripsi = "BERAWAN";
          }
       if(icon == "10d" || icon == "10n" || icon == "11d" || icon == "11n" || icon == "13d" || icon == "13n"){
          logo = "2";
          deskripsi = "HUJAN";
          }
       if(icon == "50d" || icon == "50n"){
          logo = "3"; 
          deskripsi = "BERKABUT";
          }
       if(icon == "01n"){
          logo = "4";
          deskripsi = "CERAH";
          }
}
String httpGETRequest(const char* serverName) {
  HTTPClient http;
  http.begin(serverName);                                        
  int httpResponseCode = http.GET();                             
  String payload = "{}"; 
  if (httpResponseCode>0) {
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  return payload;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WaktuNTP(){
      struct tm timeinfo;
      if(!getLocalTime(&timeinfo)){
        Serial.println("Gagal mendapatkan waktu");
        return;
      }
      char CharBulan[15];
      strftime(CharBulan,15, "%B", &timeinfo);
      String Month = String(CharBulan);
      if (Month == "January"){
         Bulan = " Jan ";
      }else if (Month == " February "){
         Bulan = " Feb ";
      }else if (Month == " March "){
         Bulan = " Mar ";
      }else if (Month == "April"){
         Bulan = " Apr ";
      }else if (Month == "May"){
         Bulan = " Mei ";
      }else if (Month == "June"){
         Bulan = " Jun ";
      }else if (Month == "July"){
         Bulan = " Jul ";
      }else if (Month == "August"){
         Bulan = " Agu ";
      }else if (Month == "September"){
         Bulan = " Sep ";
      }else if (Month == "October"){
         Bulan = " Okt ";
      }else if (Month == "November"){
         Bulan = " Nov ";
      }else if (Month == "December"){
         Bulan = " Des ";
      } 
      char CharTanggal[5];
      strftime(CharTanggal,5, "%d", &timeinfo);
      Tanggal = String(CharTanggal);
      char CharTahun[5];
      strftime(CharTahun,5, "%Y", &timeinfo);
      Tahun = String(CharTahun);
      char CharJam[5];
      strftime(CharJam,5, "%H", &timeinfo);
      Jam = String(CharJam);
      char CharMenit[5];
      strftime(CharMenit,5, "%M", &timeinfo);
      Menit = String(CharMenit);
      char CharDetik[5];
      strftime(CharDetik,5, "%S", &timeinfo);
      Detik = String(CharDetik);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NextionWrite(){
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600); 
  Serial2.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan");
  while(WiFi.status()!= WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Terhubung ke WiFi dengan alamat IP: ");
  Serial.println(WiFi.localIP());
  configTime(WaktuGMT, PengaturanMusimPanas, ntpServer);
  WaktuNTP();
  CuacaOpenWeather();
  NextionDisplay();
}

void loop() {
  if ((millis() - WTCuaca) > JedaCuaca) {
      CuacaOpenWeather();
      WTCuaca = millis();
  }
  if ((millis() - WTWaktu) > JedaWaktu) {
      WaktuNTP();
      V--;
      if (V < 0) {
        V = 53;
        }
      I = random(0,10);
      P = V*I;
      W = P/3600;
      E = V*100/53;
      T = random (0,60);
      v++;
      if (v > 50) {
        v = 0;
        }
      NxStatusWifi++;
      NxSenKiri = !NxSenKiri;
      NxSenKanan = !NxSenKanan;

      if (NxStatusWifi == 4) {
        NxStatusWifi = 0;
        }
      Serial.println(String(V) +" "+ String(I) +" "+ String(P) +" "+ String(W) +" "+ String(E) +" "+ String(T) +" "+ String(v) +" "+ String(NxSenKiri) +" "+ String(NxSenKanan) +" "+ String(NxStatusWifi));
      WTWaktu = millis();
  }
  if ((millis() - WTDisplay) > JedaDisplay) {
      NextionDisplay();
      WTDisplay = millis();
  } 
}
