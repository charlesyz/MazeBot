#include <LiquidCrystal.h>

LiquidCrystal lcd(3, 4, 7, 8, 11, 12, 13); // lcd pins

struct cell {
  // 4 types: o = open(not explored), x = dead, v = part of the solution, s = start, f = finish.
  char type;

  // ints to hold if there are avaliable pathways in each direction. false = no path, true = path
  // 0 = north, 1 = east, 2 = south, 3 = west
  bool connections[3];
};

// =============================== pins ===========================================
const short Echo = A5;            // input echo pin for ultrasound (gets echo)
const short Trig = A4;            // output trigger pin for ultrasound (sends sound)

// motor pins
const short Left_motor_back = 9;  // left motor back
const short Left_motor_go = 5;    // left motor forwards
const short Right_motor_go = 6;   // right motor forwards
const short Right_motor_back = 10;// right motor back

const short key = A0;             // Button
const short beep = A1;            // Speaker

const short SensorRight = A2;     //Right bottom mounted IR sensor (P3.2 OUT1)
const short SensorLeft = A3;      //Left bottom mounted IR sensor(P3.3 OUT2)

const short servopin = 2;         // pin controlling the servo

// ============================== globals ==========================================
const unsigned short RunSpeed = 150;        // analog motor speed

const int numRows = 4;
const int numCols = 4;

int dir = 0; // current direction the robot is facing. 0 = north, 1 = east, 2 = south, 3 = west

// matrix holding the maze info
cell maze[numRows + 2][numCols + 2];


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

  // initialise maze borders to x
  for (int i = 0; i < numRows + 2; i++) {
    for (int j = 0; j < numCols + 2; j++) {
      maze[i][j].type = 'x';
      // default all connections to false;
      for (int k = 0; i < 4; i++) {
        maze[i][j].connections[k] = false;
      }
    }
  }
  // set maze cell types to open
  for (int i = 1; i < numRows; i++)
    for (int j = 1; j < numCols; j++)
      maze[i][j].type = 'o';

  // initialise starting position
  maze[1][1].type = 's';
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
  static bool keyPressed = false;

  // maze pos
  static int posX = 1;
  static int posY = 1;

  // check for beginning key input
  while (!keyPressed)
    keyPressed = keyscan();

  // read in bottom mounted IR for line detection
  SR = digitalRead(SensorRight);
  SL = digitalRead(SensorLeft);

  //============================ for line following ====================================
  // if the car is on the line, go forwards
  if (SL == LOW && SR == LOW)
    run();
  // correct if the car is drifting off to the side
  else if (SL == HIGH && SR == LOW)
    left();
  else if (SL == LOW && SR == HIGH)
    right();

  //=========================== maze algorithm ========================================
  //if the car hits a junction, check surrounding pathways and do appropriate actions
  else if (SL == HIGH && SR == HIGH) {
    maze[posX][posY].type = 'v';

    // sets connection pointing backwards from current direction to true
    maze[posX][posY].connections[dir + 2 + 4 % 4] = true;

    // get distance input
    brake(1);
    delay(100);

    // check what spaces are avaliable and map to the maze matrix
    // Check forwards
    maze[posX][posY].connections[dir] = front_detection() < 20;
    delay(100);
    // Check left
    maze[posX][posY].connections[(dir + 3) % 4] = left_detection() < 20;
    delay(100);
    // Check right
    maze[posX][posY].connections[(dir + 5) % 4] = right_detection() < 20;
    delay(100);
    
    // check north
    if (maze[posX][posY].connections[0] && maze[posX - 1][posY].type == 'o' ) {
      // start turning left
      if (dir == 1)
        left();

      // start turning right
      else if (dir == 3)
        right();

      dir = 0;
    }
    // east
    else if (maze[posX][posY].connections[1] && maze[posX][posY + 1].type == 'o') {
      // start turning left
      if (dir == 2)
        left();

      // start turning right
      else if (dir == 0)
        right();

      dir = 1;
    }
    // south
    else if (maze[posX][posY].connections[2] && maze[posX + 1][posY].type == 'o') {
      // start turning left
      if (dir == 3)
        left();

      // start turning right
      else if (dir == 1)
        right();

      dir = 2;
    }
    // west
    else if (maze[posX][posY].connections[3] && maze[posX][posY - 1].type == 'o') {
      // start turning left
      if (dir == 0)
        left();

      // start turning right
      else if (dir == 2)
        right();

      dir = 3;
    }
    // if no openings, turn around
    else {
      maze[posX][posY].type = 'x';
      // turn 90 degrees left, then the next time it turns it will turn it 180
      left();
      // turn for at least 2.25 seconds to get past the black line
      delay(150);
      // keep turning untill the car is back on the line (this will turn the car 90 degrees)
      while (SR != HIGH)
        SR = digitalRead(SensorRight);
    }

    // turn 90 degrees
    delay(150);
    while (SR != HIGH)
      SR = digitalRead(SensorRight);

    // modify posX and posY based on direction
    switch (dir) {
      // pointing north
      case 0:
        posX--;
        break;
      // east
      case 1:
        posY++;
        break;
      // south
      case 2:
        posX++;
        break;
      // west
      case 3:
        posY--;
        break;
    }

    delay(150);
    run();
  }
}
