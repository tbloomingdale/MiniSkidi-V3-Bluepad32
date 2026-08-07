struct MOTOR_PINS {
  int pinIN1;
  int pinIN2;
};

MOTOR_PINS motorPins[] = {
  {LEFT_IN1,  LEFT_IN2},
  {RIGHT_IN1, RIGHT_IN2},
  {ARM_IN1,   ARM_IN2}
};

constexpr int NUM_MOTORS =
  sizeof(motorPins) / sizeof(motorPins[0]);

// Sets one drive motor to a signed PWM speed:
// -255 = full reverse
//    0 = stopped
// +255 = full forward
void setDriveMotorPWM(
  int speed,
  int channelIN1,
  int channelIN2
) {
  speed = constrain(speed, -MAX_PWM, MAX_PWM);

  if (speed > 0) {
    ledcWrite(channelIN1, speed);
    ledcWrite(channelIN2, 0);
  }
  else if (speed < 0) {
    ledcWrite(channelIN1, 0);
    ledcWrite(channelIN2, -speed);
  }
  else {
    ledcWrite(channelIN1, 0);
    ledcWrite(channelIN2, 0);
  }
}

// Drives both tracks using signed PWM speeds.
void moveTank(int leftSpeed, int rightSpeed) {
  setDriveMotorPWM(
    leftSpeed,
    LEFT_IN1_CHANNEL,
    LEFT_IN2_CHANNEL
  );

  setDriveMotorPWM(
    rightSpeed,
    RIGHT_IN1_CHANNEL,
    RIGHT_IN2_CHANNEL
  );
}

// The arm remains simple full-speed forward/reverse/stop.
void rotateArmMotor(int motorDirection) {
  if (motorDirection == FORWARD) {
    digitalWrite(ARM_IN1, HIGH);
    digitalWrite(ARM_IN2, LOW);
  }
  else if (motorDirection == BACKWARD) {
    digitalWrite(ARM_IN1, LOW);
    digitalWrite(ARM_IN2, HIGH);
  }
  else {
    digitalWrite(ARM_IN1, LOW);
    digitalWrite(ARM_IN2, LOW);
  }
}

void controlArm(int armValue, int deadzone) {
  if (armValue > deadzone) {
    rotateArmMotor(FORWARD);
  }
  else if (armValue < -deadzone) {
    rotateArmMotor(BACKWARD);
  }
  else {
    rotateArmMotor(STOP);
  }
}

void stopArm() {
  rotateArmMotor(STOP);
}

void setupMotors() {
  // Configure PWM channels for the drive tracks.
  ledcSetup(
    LEFT_IN1_CHANNEL,
    PWM_FREQUENCY,
    PWM_RESOLUTION
  );

  ledcSetup(
    LEFT_IN2_CHANNEL,
    PWM_FREQUENCY,
    PWM_RESOLUTION
  );

  ledcSetup(
    RIGHT_IN1_CHANNEL,
    PWM_FREQUENCY,
    PWM_RESOLUTION
  );

  ledcSetup(
    RIGHT_IN2_CHANNEL,
    PWM_FREQUENCY,
    PWM_RESOLUTION
  );

  ledcAttachPin(LEFT_IN1, LEFT_IN1_CHANNEL);
  ledcAttachPin(LEFT_IN2, LEFT_IN2_CHANNEL);
  ledcAttachPin(RIGHT_IN1, RIGHT_IN1_CHANNEL);
  ledcAttachPin(RIGHT_IN2, RIGHT_IN2_CHANNEL);

  // Arm motor pins remain normal digital outputs.
  pinMode(ARM_IN1, OUTPUT);
  pinMode(ARM_IN2, OUTPUT);

  moveTank(0, 0);
  stopArm();
}