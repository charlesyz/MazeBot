// all globals

// =============================== pins ===========================================
const short Echo = A5;            // input echo pin for ultrasound (gets echo)
const short Trig = A4;            // output trigger pin for ultrasound (sends sound)

// motor pins
const short Left_motor_back = 9;  // left motor back
const short Left_motor_go = 5;    // left motor forwards
const short Right_motor_go = 6;   // right motor forwards
const short Right_motor_back = 10;// right motor back

const short key = 2;              // Button

const short SensorRight = A2;     //Right bottom mounted IR sensor (P3.2 OUT1)
const short SensorLeft = A3;      //Left bottom mounted IR sensor(P3.3 OUT2)

const int FrontSensorLeft = A0;   //Left front mounted IR sensor(P3.4 OUT3)
const int FrontSensorRight = A1; // Right front mounted IR sensor(P3.5 OUT4)

// ============================== globals ==========================================
const int RunSpeed = 100;        // analog motor speed

LiquidCrystal lcd(3, 4, 7, 8, 11, 12, 13); // lcd and pins

//Matrix holding the maze info. The cells can be OPEN, CLOSED, or part of the PATH
//We added two to both numRow and numRow because we will frame it with CLOSED cells
// numRows and numCols are defined in maze_algorithm.h
STATUS maze[NUMROWS + 2][NUMCOLS + 2];

//Keep track of which direction our robot is facing
DIR dir = SOUTH;

// The robot's current position in the maze
int curRow = 1;
int curCol = 1;

