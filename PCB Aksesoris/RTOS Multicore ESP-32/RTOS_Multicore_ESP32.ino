#include <analogWrite.h>
#define LED1 22
#define LED2 25
#define LED3 15
#define LED4 5
#define pinpot 32
int nilai = 0;
int baru = 0;
boolean statusled2 = false;
boolean statusled3 = false;
boolean statusled4 = false;


void task1 (void*parameters) {
  for (;;) {
    nilai = analogRead(pinpot);
    nilai = map(nilai, 0, 4095, 0, 255);
    analogWrite(LED1, nilai);
    Serial.print("Nilai : ");
    Serial.println(nilai); 
    vTaskDelay(1/ portTICK_PERIOD_MS);
  }
}

void task2 (void*parameters) {
  for (;;) {
    
    statusled2 =!statusled2;
    digitalWrite(LED2, statusled2);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
void task3 (void*parameters) {
  for (;;) {
    statusled3 = !statusled3;
    digitalWrite(LED3, statusled3);
    vTaskDelay(1000 /  portTICK_PERIOD_MS);
  }
}

void task4 (void*parameters) {
  for (;;) {
    
    statusled4 =!statusled4;
    digitalWrite(LED4, statusled4); 
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pinpot, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  
  xTaskCreate(
    task1, //function name
    "Task 1", //task name
    1000, //stack size
    NULL,// task paramaters
    0, // task priority
    NULL  //task handle
    );
    
    xTaskCreate(
    task2,
    "Task 2",
    1000,
    NULL,
    1,
    NULL
     );
     
     xTaskCreate(
    task3, //function name
    "Task 3", //task name
    1000, //stack size
    NULL,// task paramaters
    1, // task priority
    NULL  //task handle
    );
    
    xTaskCreate(
    task4,
    "Task 4",
    1000,
    NULL,
    1,
    NULL
     );
}

void loop() {
  // put your main code here, to run repeatedly:

}
