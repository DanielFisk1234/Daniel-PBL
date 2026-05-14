#define LED_PIN 2

// Ultrasonic Sensor 2 (outside)
#define TRIGGER_PIN_2 3
#define ECHO_PIN_2    4

// Ultrasonic Sensor 3 (inside)
#define TRIGGER_PIN_3 5
#define ECHO_PIN_3    6

// 7 Segment Display
#define A_PIN 7
#define B_PIN 8
#define C_PIN 9
#define D_PIN 10
#define E_PIN 11
#define F_PIN 12
#define G_PIN 13

const int DETECTION_DISTANCE = 15;
const unsigned long CONFIRM_TIME = 3000;

int peopleCount = 0;

void clearDisplay() {

  digitalWrite(A_PIN, LOW);
  digitalWrite(B_PIN, LOW);
  digitalWrite(C_PIN, LOW);
  digitalWrite(D_PIN, LOW);
  digitalWrite(E_PIN, LOW);
  digitalWrite(F_PIN, LOW);
  digitalWrite(G_PIN, LOW);
}

void displayDigit(int digit) {

  clearDisplay();

  switch(digit) {

    case 0:
      digitalWrite(A_PIN, HIGH);
      digitalWrite(B_PIN, HIGH);
      digitalWrite(C_PIN, HIGH);
      digitalWrite(D_PIN, HIGH);
      digitalWrite(E_PIN, HIGH);
      digitalWrite(F_PIN, HIGH);
      break;

    case 1:
      digitalWrite(B_PIN, HIGH);
      digitalWrite(C_PIN, HIGH);
      break;

    case 2:
      digitalWrite(A_PIN, HIGH);
      digitalWrite(B_PIN, HIGH);
      digitalWrite(D_PIN, HIGH);
      digitalWrite(E_PIN, HIGH);
      digitalWrite(G_PIN, HIGH);
      break;

    case 3:
      digitalWrite(A_PIN, HIGH);
      digitalWrite(B_PIN, HIGH);
      digitalWrite(C_PIN, HIGH);
      digitalWrite(D_PIN, HIGH);
      digitalWrite(G_PIN, HIGH);
      break;

    case 4:
      digitalWrite(B_PIN, HIGH);
      digitalWrite(C_PIN, HIGH);
      digitalWrite(F_PIN, HIGH);
      digitalWrite(G_PIN, HIGH);
      break;

    case 5:
      digitalWrite(A_PIN, HIGH);
      digitalWrite(C_PIN, HIGH);
      digitalWrite(D_PIN, HIGH);
      digitalWrite(F_PIN, HIGH);
      digitalWrite(G_PIN, HIGH);
      break;

    case 6:
      digitalWrite(A_PIN, HIGH);
      digitalWrite(C_PIN, HIGH);
      digitalWrite(D_PIN, HIGH);
      digitalWrite(E_PIN, HIGH);
      digitalWrite(F_PIN, HIGH);
      digitalWrite(G_PIN, HIGH);
      break;

    case 7:
      digitalWrite(A_PIN, HIGH);
      digitalWrite(B_PIN, HIGH);
      digitalWrite(C_PIN, HIGH);
      break;

    case 8:
      digitalWrite(A_PIN, HIGH);
      digitalWrite(B_PIN, HIGH);
      digitalWrite(C_PIN, HIGH);
      digitalWrite(D_PIN, HIGH);
      digitalWrite(E_PIN, HIGH);
      digitalWrite(F_PIN, HIGH);
      digitalWrite(G_PIN, HIGH);
      break;

    case 9:
      digitalWrite(A_PIN, HIGH);
      digitalWrite(B_PIN, HIGH);
      digitalWrite(C_PIN, HIGH);
      digitalWrite(D_PIN, HIGH);
      digitalWrite(F_PIN, HIGH);
      digitalWrite(G_PIN, HIGH);
      break;
  }
}

float readDistance(int triggerPin, int echoPin) {

  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(triggerPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  float distance = duration * 0.0343 / 2;

  return distance;
}


bool sensorTriggered(int triggerPin, int echoPin) {

  float distance = readDistance(triggerPin, echoPin);

  return (distance > 0 && distance < DETECTION_DISTANCE);
}


void setup() {

  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);

  pinMode(TRIGGER_PIN_2, OUTPUT);
  pinMode(ECHO_PIN_2, INPUT);

  pinMode(TRIGGER_PIN_3, OUTPUT);
  pinMode(ECHO_PIN_3, INPUT);

  pinMode(A_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  pinMode(C_PIN, OUTPUT);
  pinMode(D_PIN, OUTPUT);
  pinMode(E_PIN, OUTPUT);
  pinMode(F_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);

  displayDigit(0);
}

void loop() {

  bool sensor2 = sensorTriggered(TRIGGER_PIN_2, ECHO_PIN_2);
  bool sensor3 = sensorTriggered(TRIGGER_PIN_3, ECHO_PIN_3);


  if(sensor2) {

    Serial.println("Possible ENTRY");

    digitalWrite(LED_PIN, HIGH);

    unsigned long startTime = millis();

    bool confirmed = false;

    while(millis() - startTime < CONFIRM_TIME) {

      if(sensorTriggered(TRIGGER_PIN_3, ECHO_PIN_3)) {

        confirmed = true;
        break;
      }
    }

    digitalWrite(LED_PIN, LOW);

    if(confirmed) {

      peopleCount++;
  

      if(peopleCount > 9)
        peopleCount = 9;

      displayDigit(peopleCount);

      Serial.println("ENTRY CONFIRMED");
      Serial.print("People Count: ");
      Serial.println(peopleCount);

      delay(1000);
    }
  }


  if(sensor3) {

    Serial.println("Possible EXIT");

    digitalWrite(LED_PIN, HIGH);

    unsigned long startTime = millis();

    bool confirmed = false;

    while(millis() - startTime < CONFIRM_TIME) {

      if(sensorTriggered(TRIGGER_PIN_2, ECHO_PIN_2)) {

        confirmed = true;
        break;
      }
    }

    digitalWrite(LED_PIN, LOW);

    if(confirmed) {

      peopleCount--;

      if(peopleCount < 0)
        peopleCount = 0;

      displayDigit(peopleCount);

      Serial.println("EXIT CONFIRMED");
      Serial.print("People Count: ");
      Serial.println(peopleCount);

      delay(1000);
    }
  }

  delay(50);
}
