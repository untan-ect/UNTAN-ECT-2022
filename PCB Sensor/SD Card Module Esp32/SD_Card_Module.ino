#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define SD_CS 5

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Membuka file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Gagal membuka file");
        return;
    }
    if(file.print(message)){
        Serial.println("Pesan diterima");
    } else {
        Serial.println("Pesan gagal diterima");
    }
    file.close();
}

void setup(){
  Serial.begin(9600);
  SD.begin(SD_CS);
  if (!SD.begin(SD_CS)) {
    Serial.println("Gagal Memuat Kartu SD");
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("Tidak Ada Kartu SD");
    return;
  }
  Serial.println("Menginisialisasi kartu SD...");
  
  appendFile(SD, "/KAPUAS_2.XLS", "Kita akan belajar bersama ");
  
}

void loop(){
}

// Hardianto-4/15/2022
