#include <LiquidCrystal.h>
#include "maze_bot.h"

void setup() {
  Serial.begin(9600);

  // initialise pins to input/output
  pinMode(Left_motor_go, OUTPUT); 
  pinMode(Left_motor_back, OUTPUT);
  pinMode(Right_motor_go, OUTPUT); 
  pinMode(Right_motor_back, OUTPUT);
  pinMode(key, INPUT);
  pinMode(beep, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
  pinMode(SensorRight, INPUT);
  pinMode(SensorLeft, INPUT);
  pinMode(SensorFront, INPUT);
  lcd.begin(16, 2);         // start lcd

  // Since want the maze borders to x, we will make the entire maze x then make the inner maze open again
  for (int i = 0; i < numRows + 2; i++)
    for (int j = 0; j < numCols + 2; j++)
      maze[i][j] = CLOSED;

  // set inner maze cell types to open (all but the othermost frame)
  for (int i = 1; i < numRows + 1; i++)
    for (int j = 1; j < numCols + 1; j++)
      maze[i][j] = OPEN;
}

// main function
void loop() {
  // check for beginning key input
  while (!keyscan());
  
  // run main algorithm
  findPath();
}

