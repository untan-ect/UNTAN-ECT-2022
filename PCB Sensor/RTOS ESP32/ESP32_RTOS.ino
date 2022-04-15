#include <analogWrite.h>
TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t Task3;
// LED pins
const int led1 = 2;
const int led2 = 4;
const int led3 = 0;
const int pot = 34;
const int button = 35;
int nilaipot = 0;
byte statusButton = LOW;
byte statusButtonTerakhir = LOW;
byte statusLED2 = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(pot, INPUT);
  pinMode(button, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
    Task1code,   /* Task function. */
    "Task1",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */
  delay(500);

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
    Task2code,   /* Task function. */
    "Task2",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task2,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */
  delay(500);

  xTaskCreatePinnedToCore(
    Task3code,   /* Task function. */
    "Task3",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task3,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */
  delay(500);
}

//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ) {

  for (;;) {
    Serial.print("Task1 running on core ");
    Serial.println(xPortGetCoreID());
    digitalWrite(led1, HIGH);
    delay(500);
    digitalWrite(led1, LOW);
    delay(500);
  }
}

//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ) {

  for (;;) {
    statusButton = digitalRead(button);
    if (statusButton != statusButtonTerakhir) {
      statusButtonTerakhir = statusButton;
      if (statusButton == LOW) {
        statusLED2 = ! statusLED2;
        digitalWrite(led2, statusLED2);
      }
    }
  }
}

void Task3code( void * pvParameters ) {

  for (;;) {
    nilaipot = analogRead(pot);
    nilaipot = map(nilaipot, 0, 4095, 0, 255);
    analogWrite(led3, nilaipot);
    Serial.print("nilai potensio:");
    Serial.println(nilaipot);
  }
}

void loop() {

}

// ADI GUNAWAN_jumat/15 april 2022
