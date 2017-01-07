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
  delay(200);
  //check for Finish
  if (checkFinish()) {
    // congratulatory message
    lcd.home();
    lcd.clear();
    lcd.print("MAZE COMPLETE!");
    lcd.setCursor(0, 2);
    lcd.print("PRESS BUTTON");
    
    // set start and finish points
    maze[1][1] = START;
    maze[curRow][curCol] = FINISH;
    
    for (;;) {
      // print maze if the user presses the button);
      if (keyscan()) {
        printMaze();
        delay(10000);

        // if the user presses the button again, restart the program
        lcd.home();
        lcd.clear();
        lcd.print("PRESS BUTTON");
        lcd.setCursor(0, 2);
        lcd.print("TO RESTART");
        while (!keyscan());
        return true;
      }
    }
  }
  //Check if forwards is not physically blocked and is not a part of the solution/dead
  else if (Distance_test() > 15 && maze[checkRow][checkCol] == OPEN) {
    delay(100);
    rotations = 0;  //Reset rotations
    moveForward();
  }
  //If the robot hasnt checked every cell, check the next one
  else if (rotations < 3) {
    // turn 90 degrees left
    turn_left();
    rotations++;
  }
  //If dead end,mark so and return false
  else {
    lcd.clear();
    lcd.home();
    lcd.print("DEAD END");
    maze[curRow][curCol] = CLOSED;

    rotations = 0;
    return false;
  }

  // call recursive function and check if dead end. this ensures that any dead ends arent on the recursive stack
  if (findPath() == false) {
    backtrack();
    return findPath();
  }
}

// print maze to serial monitor on computer
void printMaze() {
  lcd.clear();
  lcd.home();
  lcd.print("PRINTING MAZE...");
  for (int i = 1; i < 5; i++) {
    for (int j = 1; j < 5; j++) {
      switch (maze[i][j]) {
        case OPEN:
          Serial.print(".\t");
          break;
        case CLOSED:
          Serial.print("x\t");
          break;
        case PATH:
          Serial.print("+\t");
          break;
        case START:
          Serial.print("S\t");
          break;
        case FINISH:
          Serial.print("F\t");
      }
    }
    Serial.print("\n");
  }
  Serial.print(". is an open space\n x is a closed space\n + is the path of the robot\n S is the start\n F is the finish");
}

