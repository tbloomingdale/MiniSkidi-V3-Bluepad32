struct MOTOR_PINS {
  int pinIN1;
  int pinIN2;
};

MOTOR_PINS motorPins[] = {
  {LEFT_IN1,  LEFT_IN2},   // Left drive motor
  {RIGHT_IN1, RIGHT_IN2},  // Right drive motor
  {ARM_IN1,   ARM_IN2}     // Arm motor
};

constexpr int NUM_MOTORS =
  sizeof(motorPins) / sizeof(motorPins[0]);

void rotateMotor(int motorNumber, int motorDirection) {
  if (motorDirection == FORWARD) {
    digitalWrite(motorPins[motorNumber].pinIN1, HIGH);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);
  }
  else if (motorDirection == BACKWARD) {
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, HIGH);
  }
  else {
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);
  }
}

void moveTank(int leftCommand, int rightCommand) {
  if (leftCommand > 0) {
    rotateMotor(LEFT_MOTOR, FORWARD);
  }
  else if (leftCommand < 0) {
    rotateMotor(LEFT_MOTOR, BACKWARD);
  }
  else {
    rotateMotor(LEFT_MOTOR, STOP);
  }

  if (rightCommand > 0) {
    rotateMotor(RIGHT_MOTOR, FORWARD);
  }
  else if (rightCommand < 0) {
    rotateMotor(RIGHT_MOTOR, BACKWARD);
  }
  else {
    rotateMotor(RIGHT_MOTOR, STOP);
  }
}

void controlArm(int armValue, int deadzone) {
  if (armValue > deadzone) {
    rotateMotor(ARM_MOTOR, FORWARD);
  }
  else if (armValue < -deadzone) {
    rotateMotor(ARM_MOTOR, BACKWARD);
  }
  else {
    rotateMotor(ARM_MOTOR, STOP);
  }
}

void stopArm() {
  rotateMotor(ARM_MOTOR, STOP);
}

void setupMotors() {
  for (int i = 0; i < NUM_MOTORS; i++) {
    pinMode(motorPins[i].pinIN1, OUTPUT);
    pinMode(motorPins[i].pinIN2, OUTPUT);
  }

  moveTank(0, 0);
  stopArm();
}