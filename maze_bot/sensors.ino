// all functions that involve ultrasound sensors, bottom mounted infrared sensors, and front mounted infrared sensors

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
  int SF;
  SF = digitalRead(SensorFront);

  // maze is complete, flash LCD with victory message
  if (Distance_test() < 10 && SF == HIGH)
    return true;

  return false;
}

