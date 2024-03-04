const int in1 = 4;
const int in2 = 5;
const int in3 = 6;
const int in4 = 7;
const int enA = 9;
const int enB = 8;

#define switch_red 2
#define switch_black 3
#define irSensorRight A2
#define irSensorCenter A0
#define irSensorLeft A3  // Hinzugefügter Sensor auf der linken Seite
bool program_state = false;

// Values for the front to decide if you move forward/backwards
#define FRONT_NO_SPACE 300
#define FRONT_MUCH_SPACE 150

// Difference for deciding when to turn left/right/do neither
#define HIGH_DIFFERENCE 200
#define MEDIUM_DIFFERENCE 100
//#define SMALL_DIFFERENCE

// Speeds
#define SLOW_SPEED 130
#define SLOW_SPEED_2 100
#define MEDIUM_SPEED  160
#define HIGH_SPEED  255
#define STOP_SPEED 20

// mode Values
#define VORWAERTS 1
#define TURN_LEFT 2
#define TURN_RIGHT 3
#define BACKWARDS 4

void drive(int mode, int speed_one, int speed_two);
void driveDirections(bool dirIn1, bool dirIn2, bool dirIn3, bool dirIn4);

void setup() {
  Serial.begin(9600);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(switch_red, INPUT_PULLUP);
  pinMode(switch_black, INPUT_PULLUP);
}

void loop() {

  /*
    while(true){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enA, HIGH_SPEED);
    analogWrite(enB, HIGH_SPEED);
    delay(5000);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enA, HIGH_SPEED);
    analogWrite(enB, HIGH_SPEED);
    delay(5000);
    }
    //*/
  // Überprüfe den Zustand der Schalter
  if (digitalRead(switch_black) == LOW) {
    program_state = true;
  }

  if (digitalRead(switch_red) == LOW) {
    program_state = false;
    analogWrite(enA, 0);
    analogWrite(enB, 0);
  }

  /*Serial.println("Werte L/M/R");
    Serial.println(analogRead(irSensorLeft));
    Serial.println(analogRead(irSensorCenter));
    Serial.println(analogRead(irSensorRight));
    //delay(500);*/

  // Schalte den Motorcontroller ein, wenn im Programmzustand
  if (program_state) {
    // Lesen der Infrarotsensoren
    int infraredValueRight = analogRead(irSensorRight);
    int infraredValueCenter = analogRead(irSensorCenter);
    int infraredValueLeft = analogRead(irSensorLeft);  // Lesen des linken Sensors

    if ( infraredValueCenter >= FRONT_NO_SPACE ) {
      drive(BACKWARDS, MEDIUM_SPEED, MEDIUM_SPEED); // Rückwärts fahren, da kein Platz vorne
      Serial.println("Modus: Rückwärts"); // Ausgabe des Modus

    } else {
      int diff = infraredValueLeft - infraredValueRight;
      Serial.println("Diff: ");
      Serial.println(diff);

    /*  if (diff >= HIGH_DIFFERENCE || diff <= -HIGH_DIFFERENCE) {
        if (diff > 0) {
          drive(TURN_RIGHT, SLOW_SPEED_2, SLOW_SPEED); // Langsam nach rechts fahren
          Serial.println("Modus: Scharfe Rechtsdrehung"); // Ausgabe des Modus
        } else if (diff < 0) {
          drive(TURN_LEFT, SLOW_SPEED, SLOW_SPEED_2); // Langsam nach links fahren
          Serial.println("Modus: Scharfe Linksdrehung"); // Ausgabe des Modus
        }
      } */if (diff >= MEDIUM_DIFFERENCE || diff <= -MEDIUM_DIFFERENCE) {
        if (diff > 0) {
          drive(TURN_RIGHT, SLOW_SPEED, MEDIUM_SPEED); // Langsam nach rechts fahren
          Serial.println("Modus: Rechtsdrehung"); // Ausgabe des Modus
        } else {  /* if (diff<0) */ //Abfrage ABSOLUT unnoetig.
          drive(TURN_LEFT, MEDIUM_SPEED, SLOW_SPEED); // Langsam nach links fahren
          Serial.println("Modus: Linksdrehung"); // Ausgabe des Modus
        }
      } else {
        drive(VORWAERTS, HIGH_SPEED, HIGH_SPEED); // Langsam vorwärts fahren, auf Platz rechts/links warten
        Serial.println("Modus: Langsam vorwärts"); // Ausgabe des Modus
      }
      //delay(200);


      /*if(infraredValueLeft < 50&& infraredValueCenter > FRONT_MUCH_SPACE)
        {
        drive(TURN_LEFT, SLOW_SPEED, MEDIUM_SPEED); // Langsam nach rechts fahren
        Serial.println("Modus: Scharf links!");
        }
        else if(infraredValueRight < 50 && infraredValueCenter > FRONT_MUCH_SPACE)
        {
        drive(TURN_RIGHT, MEDIUM_SPEED, SLOW_SPEED); // Langsam nach links fahren
        Serial.println("Modus: Scharf rechts!");
        }*/


      /*else if(infraredValueCenter > FRONT_MUCH_SPACE) { // Kein großer Unterschied zwischen links/rechts UND Platz vorne -> Vorwärts fahren
        drive(VORWAERTS, HIGH_SPEED, HIGH_SPEED); // Schnell vorwärts fahren
        Serial.println("Modus: Vorwärts"); // Ausgabe des Modus }*/

    }
  }
}

void drive(int mode, int speed_one, int speed_two) {
  /*driveDirections(HIGH, LOW, LOW, HIGH);
  analogWrite(enA, 255);
  analogWrite(enB, 130);
  return;*/
  //The upper lines are for debugging!
  switch (mode) {
    case TURN_LEFT:
    case TURN_RIGHT:
    case VORWAERTS:
      driveDirections(HIGH, LOW, LOW, HIGH);
      break;
    /*case TURN_LEFT:
      driveDirections(HIGH, LOW, HIGH, LOW);
      break;
    case TURN_RIGHT:
      driveDirections(LOW, HIGH, LOW, HIGH);
      break;*/
    case BACKWARDS:
      driveDirections(LOW, HIGH, HIGH, LOW);
      break;
    /*default:
      Serial.print("Hier sollte er nie rein, error nicht behandelt, mode = ");
      Serial.println(mode);
      exit(-1); // Das Programm stoppen, das sollte NIE passieren.
  */}
  /*
    Serial.print("Mode: ");
    Serial.println(mode);
    Serial.print("Dir_in1:");
    Serial.print(Dir_in1);
    Serial.print("\tDir_in2:");
    Serial.print(Dir_in2);
    Serial.print("\tDir_in3:");
    Serial.print(Dir_in3);
    Serial.print("\tDir_in4:");
    Serial.print(Dir_in4);
    Serial.print("\n");
    //*/
  analogWrite(enA, speed_one);
  analogWrite(enB, speed_two);
}

void driveDirections(bool dirIn1, bool dirIn2, bool dirIn3, bool dirIn4) {
  digitalWrite(in1, dirIn1);
  digitalWrite(in2, dirIn2);
  digitalWrite(in3, dirIn3);
  digitalWrite(in4, dirIn4);
}
