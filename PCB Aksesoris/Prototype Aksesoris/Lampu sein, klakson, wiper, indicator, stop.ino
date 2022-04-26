#define indicatorLamp  22
#define StopLamp 15
#define Leftsein 5
#define Rightsein 32
#define Wiper 33
#define Klakson 25
#define buttonwiper 26
#define buttonlftsnt 27
#define buttonrghtlmp 14
#define buttonstplmp 12
#define buttonklakson 13
boolean srsein = false;
boolean slsein = false;

TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t Task3;
TaskHandle_t Task4;
TaskHandle_t Task5;


void Stplmp (void*parameters) {
  for (;;) {
    int nStplmp = digitalRead(buttonstplmp);
    Serial.print("STOPLAMP : ");
    Serial.println(nStplmp);
    if ( nStplmp == 1) {
      digitalWrite( StopLamp, HIGH);
    }
    else {
      digitalWrite (StopLamp, LOW);
    }
  }
}
void Rsein (void*parameters) {
  for (;;) {

    int nrsein = digitalRead(buttonrghtlmp);
    Serial.print("RIGHTSEIN : ");
    Serial.println(nrsein);
    if (nrsein == 1) {
      srsein = !srsein;
      digitalWrite(Rightsein, srsein);
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    else {
      digitalWrite(Rightsein, LOW);
    }
  }
}

void Lsein (void*parameters) {
  for (;;) {

    int nLsein = digitalRead(buttonlftsnt);
    Serial.print("LEFTSEIN : ");
    Serial.println(nLsein);

    if (nLsein == 1) {
      slsein = !slsein;
      digitalWrite(Leftsein, slsein);
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    else {
      digitalWrite(Leftsein, LOW);
    }
  }
}

void Wpr (void*parameters) {
  for (;;) {
    int nWpr = digitalRead(buttonwiper);
    Serial.print("Wiper : ");
    Serial.println(nWpr);
    if (nWpr == 1) {
      digitalWrite(Wiper, HIGH);
    }
    else {
      digitalWrite (Wiper, LOW);
    }
  }
}

void Klksn (void*parameters) {
  for (;;) {
    int nKlksn = digitalRead(buttonklakson);
    Serial.print("Klakson : ");
    Serial.println(nKlksn);
    if ( nKlksn == 1) {
      digitalWrite( Klakson, HIGH);
    }
    else {
      digitalWrite (Klakson, LOW);
    }
  }
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(indicatorLamp , OUTPUT);
  pinMode(StopLamp , OUTPUT);
  pinMode(Leftsein , OUTPUT);
  pinMode(Rightsein , OUTPUT);
  pinMode(StopLamp , OUTPUT);
  pinMode(Klakson , OUTPUT);
  pinMode(Wiper, OUTPUT);
  pinMode(buttonwiper, INPUT);
  pinMode(buttonlftsnt, INPUT);
  pinMode(buttonrghtlmp, INPUT);
  pinMode(buttonstplmp, INPUT);
  pinMode(buttonklakson, INPUT);

  xTaskCreatePinnedToCore(
    Rsein,   /* Task function. */
    "Rightsein",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task2,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */

  xTaskCreatePinnedToCore(
    Lsein,   /* Task function. */
    "Leftsein",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task3,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */
  
    xTaskCreatePinnedToCore(
    Stplmp,   /* Task function. */
    "stoplamp",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    1);          /* pin task to core 0 */

  xTaskCreatePinnedToCore(
    Wpr,   /* Task function. */
    "Wiper",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task4,      /* Task handle to keep track of created task */
    1);          /* pin task to core 0 */

  xTaskCreatePinnedToCore(
    Klksn,   /* Task function. */
    "Klakson",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task5,      /* Task handle to keep track of created task */
    1);          /* pin task to core 0 */

  digitalWrite(indicatorLamp, HIGH);
  delay(3000);
  digitalWrite(indicatorLamp, LOW);



}

void loop() {
  // put your main code here, to run repeatedly:

}

// tanggal 26 April 2022
// oleh : Fadli Amman Siwaka
