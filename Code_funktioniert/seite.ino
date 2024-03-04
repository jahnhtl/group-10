const int in1 =4;
const int in2 = 5;
const int in3= 6;
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
//#define HIGH_DIFFERENCE 
  #define MEDIUM_DIFFERENCE 300
//#define SMALL_DIFFERENCE 

// Speeds
#define SLOW_SPEED 130
#define MEDIUM_SPEED  150
#define HIGH_SPEED  255

// mode Values
#define VORWAERTS 1
#define TURN_LEFT 2
#define TURN_RIGHT 3
#define BACKWARDS 4

void drive(int mode, int speed_one, int speed_two);


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
    delay(500);*/ 

  // Schalte den Motorcontroller ein, wenn im Programmzustand
  if (program_state) {
    // Lesen der Infrarotsensoren
    int infraredValueRight = analogRead(irSensorRight);
    int infraredValueCenter = analogRead(irSensorCenter);
    int infraredValueLeft = analogRead(irSensorLeft);  // Lesen des linken Sensors

    if( infraredValueCenter >= FRONT_NO_SPACE ){
      drive(BACKWARDS, SLOW_SPEED, SLOW_SPEED); // Rückwärts fahren, da kein Platz vorne
    } else { 
      int diff = infraredValueLeft - infraredValueRight;

      if(diff >= MEDIUM_DIFFERENCE || diff <= -MEDIUM_DIFFERENCE){
        if(diff > 0){
          drive(TURN_RIGHT, SLOW_SPEED, MEDIUM_SPEED); // Langsam nach rechts fahren
        } else {
          drive(TURN_LEFT, MEDIUM_SPEED, SLOW_SPEED); // Langsam nach links fahren
        }
      } else if(infraredValueCenter > FRONT_MUCH_SPACE) { // Kein großer Unterschied zwischen links/rechts UND Platz vorne -> Vorwärts fahren
        drive(VORWAERTS, HIGH_SPEED, HIGH_SPEED); // Schnell vorwärts fahren
      } else {
        drive(VORWAERTS, SLOW_SPEED, SLOW_SPEED); // Langsam vorwärts fahren, auf Platz rechts/links warten
      }
    }
  }
}

void drive(int mode, int speed_one, int speed_two){
  bool Dir_in1; // Richtung des Eingangs eins
  bool Dir_in2;
  bool Dir_in3;
  bool Dir_in4;
  switch (mode){
    case VORWAERTS:
      Dir_in1 = HIGH;
      Dir_in2 = LOW;
      Dir_in3 = LOW;
      Dir_in4 = HIGH;
      break;
     case TURN_LEFT:
      Dir_in1 = LOW;
      Dir_in2 = HIGH;
      Dir_in3 = HIGH;
      Dir_in4 = LOW;
      break;
    case TURN_RIGHT:
      Dir_in1 = HIGH;
      Dir_in2 = LOW;
      Dir_in3 = LOW;
      Dir_in4 = HIGH;
      break;
    case BACKWARDS:
      Dir_in1 = LOW;
      Dir_in2 = HIGH;
      Dir_in3 = HIGH;
      Dir_in4 = LOW;
      break;
    default:
      Serial.print("Hier sollte er nie rein, error nicht behandelt, mode = ");
      Serial.println(mode);
      exit(-1); // Das Programm stoppen, das sollte NIE passieren.
  } 
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
  digitalWrite(in1, Dir_in1);
  digitalWrite(in2, Dir_in2);   
  digitalWrite(in3, Dir_in3);
  digitalWrite(in4, Dir_in4);
  analogWrite(enA, speed_one);
  analogWrite(enB, speed_two);
}
