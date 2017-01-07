// all functions that involve ultrasound sensors, bottom mounted infrared sensors, and front mounted infrared sensors

//Move forwards untill the next intersection
void moveForward() {
  int SL = LOW;
  int SR = LOW;    //Left and Right Sensor status, HIGH is on and LOW is off

  //Update curRow and curCol based on where we are going
  switch (dir) {
    case NORTH:
      curRow--;
      break;
    case EAST:
      curCol++;
      break;
    case SOUTH:
      curRow++;
      break;
    case WEST:
      curCol--;
      break;
  }

  lcd.clear();
  lcd.home();
  lcd.print("RUNRUNRUN");
  delay(500);
  // move so that the car is ontop of the line
  run();
  delay(75);

  while (SL != HIGH || SR != HIGH) {
    lcd.clear();
    lcd.home();
    lcd.print("Line Following");
    // read in bottom mounted IR for line detection
    SR = digitalRead(SensorRight);
    SL = digitalRead(SensorLeft);

    if (SL == LOW && SR == LOW)
      run();
    // correct if the car is drifting off to the side
    else if (SL == HIGH && SR == LOW)
      left();
    else if (SL == LOW && SR == HIGH)
      right();
  }
  brake(5);
  // run untill the car wheels are on the intersection
  run();
  delay(350);
  //if the car hits a junction, stop
  brake(5);
}

//backtrack to the last avaliable path
void backtrack() {
  int checkRow;
  int checkCol;

  lcd.clear();
  lcd.home();
  lcd.print("Backtracking...");

  for (int i = 0; i < 4; i++) {
    checkRow = curRow;
    checkCol = curCol;
    // get coordinates of cell infront of robot
    switch (dir) {
      case NORTH:
        checkRow--;
        break;
      case EAST:
        checkCol++;
        break;
      case SOUTH:
        checkRow++;
        break;
      case WEST:
        checkCol--;
        break;
    }

    if (Distance_test() > 15 && maze[checkRow][checkCol] == PATH) {
      delay(100);
      moveForward();
      return;
    }

    turn_left();
    delay(100);
  }
  
  // error
  while (!keyscan()) {
    lcd.clear();
    lcd.home();
    lcd.print("ERROR: Can't backtrack");
  }
}

// turn 90 degress left based on line following
void turn_left() {
  // bottom mounted infrared sensor status
  int SL, SR;

  // spin left for at least 2s
  spin_left();
  delay(200);

  // keep turning untill the car just hits the next intersection
  while (SL != HIGH || SR != LOW) {
    SR = digitalRead(SensorRight);
    SL = digitalRead(SensorLeft);
  }

  // brake for accuracy
  brake(1);

  // keep turning untill the car is now over the next intersection
  while (SL != LOW || SR != LOW) {
    SR = digitalRead(SensorRight);
    SL = digitalRead(SensorLeft);

    // spin is car is off to the right
    if (SL == HIGH && SR == LOW)
      spin_left();
    // spin if car is off to the left
    if (SL == LOW && SR == HIGH)
      spin_right();
  }
  brake(1);

  // change dir based on current dir
  dir = DIR((dir + 3) % 4);
}

// turn 90 degress right
void turn_right() {
  // bottom mounted infrared sensor status
  int SL, SR;

  // spin left for at least 1s
  spin_right();
  delay(200);

  // keep turning untill the car just hits the next intersection
  while (SL != LOW || SR != HIGH) {
    SR = digitalRead(SensorRight);
    SL = digitalRead(SensorLeft);
  }

  // brake for accuracy
  brake(1);

  // keep turning untill the car is now over the next intersection
  while (SL != LOW || SR != LOW) {
    SR = digitalRead(SensorRight);
    SL = digitalRead(SensorLeft);

    // spin if car is off to the left
    if (SL == LOW && SR == HIGH)
      spin_right();
    // spin is car is off to the right
    if (SL == HIGH && SR == LOW)
      spin_left();
  }
  brake(1);

  // change dir based on current dir
  dir = DIR((dir + 1) % 4);
}

// gets button input
bool keyscan() {
  int val;
  val = digitalRead(key);

 if (val == HIGH)
    return true;
  
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

float Distance_test() {
  // front distance
  float Fdistance;

  // send pulses
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);

  // get echo
  Fdistance = pulseIn(Echo, HIGH);
  // calculate and display distance
  Fdistance = Fdistance / 58;
  Distance_display(Fdistance);
  return Fdistance;
}

// check if the finish is reached
bool checkFinish() {
  int frontSR;
  int frontSL;
  frontSR = digitalRead(FrontSensorRight);
  frontSL = digitalRead(FrontSensorLeft);
  // maze is complete, flash LCD with victory message
  if (Distance_test() < 10 && frontSL == HIGH && frontSR == HIGH)
    return true;

  return false;
}

