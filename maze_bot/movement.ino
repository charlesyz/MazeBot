// all helper functions to do with controlling motors only

// GO FORWARDS - Sets right motor and left motor to go forwards at RunSpeed
void run() {
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, RunSpeed);
  analogWrite(Right_motor_back, 0);
  digitalWrite(Left_motor_go, HIGH);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, RunSpeed);
  analogWrite(Left_motor_back, 0);
}

// STOPS THE CAR FOR int time SECONDS
void brake(int time) {
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, LOW);
  delay(time * 100);
}

// starts the car turning left
void left() {
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
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, 0);
  analogWrite(Right_motor_back, 0);
  digitalWrite(Left_motor_go, HIGH);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, RunSpeed);
  analogWrite(Left_motor_back, 0);
}

// starts the car going backwards
void back() {
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, HIGH);
  analogWrite(Right_motor_go, 0);
  analogWrite(Right_motor_back, RunSpeed);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, HIGH);
  analogWrite(Left_motor_go, 0);
  analogWrite(Left_motor_back, RunSpeed);
}

// turn left by moving the left motor backwards and the right motor forwards
void spin_left() {
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, RunSpeed);
  analogWrite(Right_motor_back, 0);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, HIGH);
  analogWrite(Left_motor_go, 0);
  analogWrite(Left_motor_back, RunSpeed);
}
// turn right by moving the left motor backwards and the right motor forwards
void spin_right() {
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, HIGH);
  analogWrite(Right_motor_go, 0);
  analogWrite(Right_motor_back, RunSpeed);
  digitalWrite(Left_motor_go, HIGH);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, RunSpeed);
  analogWrite(Left_motor_back, 0);
}
