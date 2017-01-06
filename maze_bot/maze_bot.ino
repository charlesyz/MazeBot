#include <LiquidCrystal.h>
LiquidCrystal lcd(3, 4, 7, 8, 11, 12, 13); // lcd pins

// pins
const int Echo = A5;            // input echo pin for ultrasound (gets echo)
const int Trig = A4;            // output trigger pin for ultrasound (sends sound)

// motor pins
const int Left_motor_back = 9;  // left motor back
const int Left_motor_go = 5;    // left motor forwards
const int Right_motor_go = 6;   // right motor forwards
const int Right_motor_back = 10;// right motor back

const int key = A0;             // Button
const int beep = A1;            // Speaker

const int SensorRight = A2;     //Right bottom mounted IR sensor (P3.2 OUT1)
const int SensorLeft = A3;      //Left bottom mounted IR sensor(P3.3 OUT2)

const int servopin = 2;         // pin controlling the servo

const unsigned short RunSpeed = 150;        // analog motor speed

void setup() {
  Serial.begin(9600);
  // initialise pins to input/output
  pinMode(Left_motor_go, OUTPUT); // PIN 8 (PWM)
  pinMode(Left_motor_back, OUTPUT); // PIN 9 (PWM)
  pinMode(Right_motor_go, OUTPUT); // PIN 10 (PWM)
  pinMode(Right_motor_back, OUTPUT); // PIN 11 (PWM)
  pinMode(key, INPUT);
  pinMode(beep, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
  lcd.begin(16, 2);         // start lcd
  pinMode(servopin, OUTPUT);
}

// go forwards
void run() {
  // set right motor and left motor to go forwards at RunSpeed
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, RunSpeed);
  analogWrite(Right_motor_back, 0);
  digitalWrite(Left_motor_go, HIGH);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, RunSpeed);
  analogWrite(Left_motor_back, 0);
}

// stops the car for time seconds
void brake(int time) {
  // stop motors for time seconds
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, LOW);
  delay(time * 100);
}

// starts the car turning left
void left() {
  // set right motor to go forwards
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, RunSpeed);
  analogWrite(Right_motor_back, 0);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, 0);
  analogWrite(Left_motor_back, 0);
}

// starts the car turning right
void right() {
  // set left motor to go forwards
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, 0);
  analogWrite(Right_motor_back, 0);
  digitalWrite(Left_motor_go, HIGH);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, RunSpeed);
  analogWrite(Left_motor_back, 0);
}


//--------------------- REMOVE THESE IF NOT USED ---------------------
void spin_right(int time) {
  // set right motor to go backwards and left motor to go forwards for time seconds
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, HIGH);
  analogWrite(Right_motor_go, 0);
  analogWrite(Right_motor_back, 200);
  digitalWrite(Left_motor_go, HIGH);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, 200);
  analogWrite(Left_motor_back, 0);
  delay(time * 100);
}

void spin_left(int time) {
  // set left motor to go backwards and right motor to go forwards for time seconds
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, 200);
  analogWrite(Right_motor_back, 0);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, HIGH);
  analogWrite(Left_motor_go, 0);
  analogWrite(Left_motor_back, 200);
  delay(time * 100);
}

void back(int time) {
  // set both motors to go backwards for time seconds
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, HIGH);
  analogWrite(Right_motor_go, 0);
  analogWrite(Right_motor_back, 200);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, HIGH);
  analogWrite(Left_motor_go, 0);
  analogWrite(Left_motor_back, 200);
  delay(time * 100);
}
//==========================================================

// gets button input and plays a sound
bool keyscan() {
  int val;
  val = digitalRead(key);

  if (val == HIGH) {         // beep if the user presses the key
    digitalWrite(beep, HIGH);
    delay(200);
    digitalWrite(beep, LOW);
    return true;
  }
  return false;
}

// display distance on the LCD
void Distance_display(int Distance) {
  lcd.clear();
  // if within range print the distance
  if (Distance > 2 && Distance < 400) {

    lcd.home();
    lcd.print("    Distance: ");
    lcd.setCursor(6, 2);
    lcd.print(Distance);
    lcd.print("cm");
  }
  // print if out of range
  else {
    lcd.home();
    lcd.print("Out of range!!!     ");
  }
  delay(250);
}


float Distance_test(){
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);  
  float Fdistance = pulseIn(Echo, HIGH); 
  Fdistance= Fdistance / 58;  
  return Fdistance;
}  

void servopulse(int servopin,int myangle){
  int pulsewidth;
  pulsewidth = (myangle * 11) + 500;
  
  digitalWrite(servopin,HIGH);
  delayMicroseconds(pulsewidth);
  digitalWrite(servopin,LOW);
  delay(20 - pulsewidth / 1000);
}

int front_detection(){
  
  for(int i=0;i<=5;i++)
    servopulse(servopin, 90);
  
  return Distance_test();
}

int left_detection(){
  for(int i = 0; i <= 15; i++)
    servopulse(servopin, 180);
  
  return Distance_test();
}

int right_detection(){
  for(int i = 0; i <= 15; i++){
    servopulse(servopin, 0);
  }
  
  return Distance_test();
}

//===========================================================
// main function
void loop() {
  int SL;    //left Sensor status
  int SR;    //Right Sensor status
  
  // ultrasound distances
  int Front_Distance = 0;
  int Left_Distance = 0;
  int Right_Distance = 0;
  bool keyPressed = false;
  
  // check for beginning key input
  while (!keyPressed)
    keyPressed = keyscan();

  // read in bottom mounted IR for line detection
  SR = digitalRead(SensorRight);
  SL = digitalRead(SensorLeft);

  if (SL == LOW && SR == LOW) // if the car is on the line, go forwards
    run();

  // correct if the car is drifting off to the side
  else if (SL == HIGH && SR == LOW)
    left();
  else if (SL == LOW && SR == HIGH)
    right();

  //if the car hits a junction, check surrounding pathways and do appropriate actions 
  else if (SL == HIGH && SR == HIGH) {
    // get distance input
    brake(1);
    delay(100);

    // get distances from ultrasound
    Front_Distance = front_detection();
    Distance_display(Front_Distance);
    delay(100);
    Left_Distance = left_detection();
    Distance_display(Left_Distance);
    delay(100);
    Right_Distance = left_detection();
    Distance_display(Right_Distance);
    delay(100);

    // if there's a wall in front
    if (Front_Distance < 20) {
      
      // if there's a wall on the left and the right, turn around
      if (Left_Distance < 25 && Right_Distance < 20) {
        left();
        // turn 90 degrees twice
        for (int i = 0; i < 2; i++) {
          // turn for at LEAST 1 second
          delay(150);
          // keep turning untill the car is back on the line (this will turn the car 90 degrees)
          while (SR != HIGH)
            SR = digitalRead(SensorRight);
        }
      }

      // start turn left or right if available
      else if (Left_Distance > 20) {
        left();
        delay(150);
        while (SR != HIGH)
          SR = digitalRead(SensorRight);
      }

      else {
        right();
        delay(150);
        while (SL != HIGH)
          SR = digitalRead(SensorLeft);
      }
      brake(1);
    }
    // if it hits an intersection and there's space infront, keep going
    else{
      run();
      delay(150);
    }
  }

}
