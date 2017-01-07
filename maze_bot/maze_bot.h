#ifndef MAZE_BOT_H_
#define MAZE_BOT_H_

#define NUMROWS 4
#define NUMCOLS 4

enum STATUS {OPEN, CLOSED, PATH, START, FINISH};
enum DIR {NORTH, EAST, SOUTH, WEST};


// ========================== function prototypes =================================
//  main_algorithm
bool findPath();        // main recursive algorithm
void printMaze();       // print maze to serial monitor on computer

// movement
void run();             // start the car going forwards
void brake(int time);   // stop the car for TIME seconds
void left();            // set the right motor to go (turn left)
void right();           // set the left motor to go (turn right)
void spin_left();       // set the car to spin left (left motor backwards, right motor forwards)
void spin_right();       // set the car to spin right (right motor backwards, left motor forwards)

// sensors
bool keyscan();                      // get button input
void Distance_display(int Distance); // display distance on the LCD
float Distance_test();               // test for front distance
bool checkFinish();                  // check if the finish is reached
void turn_left();                    // turn 90 degress left at an intersection based on line following
void turn_right();                   // turn 90 degress right
void moveForward();                  // Move forwards untill the next intersection
void backtrack();                    // backtrack to the last avaliable path

// =============================== pins ===========================================
extern const short Echo;            // input echo pin for ultrasound (gets echo)
extern const short Trig;            // output trigger pin for ultrasound (sends sound)

// motor pins
extern const short Left_motor_back;  // left motor back
extern const short Left_motor_go;    // left motor forwards
extern const short Right_motor_go;   // right motor forwards
extern const short Right_motor_back;// right motor back

extern const short key;              // Button
extern const short beep;             // Speaker

extern const short SensorRight;     //Right bottom mounted IR sensor (P3.2 OUT1)
extern const short SensorLeft;      //Left bottom mounted IR sensor(P3.3 OUT2)

extern const int FrontSensorRight;   //Left front mounted IR sensor(P3.4 OUT3)
extern const int FrontSensorLeft;

// ============================== globals ==========================================
extern const int RunSpeed;        // analog motor speed

extern LiquidCrystal lcd; // lcd and pins

//Matrix holding the maze info. The cells can be OPEN, CLOSED, or part of the PATH
//We added two to both numRow and numRow because we will frame it with CLOSED cells
extern STATUS maze[NUMROWS + 2][NUMROWS + 2];

//Keep track of which direction our robot is facing
extern DIR dir;

// The robot's current position in the maze
extern int curRow;
extern int curCol;
#endif
