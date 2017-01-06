// functions for the main algorithm

// main recursive algorithm
bool findPath() {
  static int rotations = 0;
  int checkRow = curRow;
  int checkCol = curCol;

  maze[curRow][curCol] = PATH;

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

  lcd.home();
  lcd.clear();
  lcd.print(dir);
  lcd.setCursor(0, 2);
  lcd.print(curRow);
  lcd.setCursor(2, 2);
  lcd.print(curCol);
  lcd.setCursor(5, 2);
  lcd.print(checkRow);
  lcd.setCursor(7, 2);
  lcd.print(checkCol);
  delay(500);

  //check for Finish
  if (checkFinish()) {
    // set start and finish points
    maze[1][1] = START;
    maze[curRow][curCol] = FINISH;
    for (;;) {
      // congratulatory message
      lcd.print("MAZE COMPLETE!");
      delay(500);
      lcd.clear();
      delay(500);
      
      // print maze if the user presses the button);
      if (keyscan()) {
        printMaze();
        // if the user presses the button again, restart the program
        while (!keyscan());
        return true();
      }
    }
  }
  //Check if forwards is not physically blocked and is not a part of the solution/dead
  else if (Distance_test() > 10 && maze[checkRow][checkCol] == OPEN) {
    lcd.clear();
    lcd.home();
    lcd.print("front is free");
    delay(100);

    rotations = 0;  //Reset rotations
    moveForward();
  }
  else if (rotations < 3) { //If I haven't checked every single cell
    // turn 90 degrees left
    turn_left();
    rotations++;
  }
  else { //If dead end
    lcd.clear();
    lcd.home();
    lcd.print("dead end");
    maze[curRow][curCol] = CLOSED;

    rotations = 0;
    return false;
  }
  
  if (findPath() == false) {
    backtrack();
    return findPath();
  }
}

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
  delay(100);

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

  for (int i = 0; i < 3; i++) {
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

    if (Distance_test() > 10 && maze[checkRow][checkCol] == PATH) {
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

// print maze to serial monitor on computer
void printMaze() {
  for (int i = 1; i < 5; i++) {
    for (int j = 1; j < 5; j++) {
      switch (maze[i][j]) {
        case OPEN:
          Serial.print(". ");
          break;
        case CLOSED:
          Serial.print("x ");
          break;
        case PATH:
          Serial.print("+ ");
        case START:
          Serial.print("S ");
        case FINISH:
          Serial.print("F ");
      }
    }
    Serial.print("\n");
  }
}

