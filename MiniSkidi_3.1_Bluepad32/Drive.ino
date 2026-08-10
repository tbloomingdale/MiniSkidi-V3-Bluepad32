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

// Sets the arm motor to a signed PWM speed:
// -255 = full down
//    0 = stopped
// +255 = full up
void setArmMotorPWM(int speed) {
  speed = constrain(speed, -ARM_MAX_PWM, ARM_MAX_PWM);

  if (speed > 0) {
    ledcWrite(ARM_IN1_CHANNEL, speed);
    ledcWrite(ARM_IN2_CHANNEL, 0);
  }
  else if (speed < 0) {
    ledcWrite(ARM_IN1_CHANNEL, 0);
    ledcWrite(ARM_IN2_CHANNEL, -speed);
  }
  else {
    ledcWrite(ARM_IN1_CHANNEL, 0);
    ledcWrite(ARM_IN2_CHANNEL, 0);
  }
}
void controlArm(int armValue, int deadzone) {
  if (abs(armValue) <= deadzone) {
    setArmMotorPWM(0);
    return;
  }

  int magnitude = abs(armValue);

  int pwm = map(
    magnitude,
    deadzone,
    512,
    ARM_MIN_PWM,
    ARM_MAX_PWM
  );

  pwm = constrain(
    pwm,
    ARM_MIN_PWM,
    ARM_MAX_PWM
  );

  if (armValue > 0) {
    setArmMotorPWM(pwm);
  }
  else {
    setArmMotorPWM(-pwm);
  }
}

void stopArm() {
  setArmMotorPWM(0);
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

  // Configure PWM channels for proportional arm control.
ledcSetup(
  ARM_IN1_CHANNEL,
  PWM_FREQUENCY,
  PWM_RESOLUTION
);

ledcSetup(
  ARM_IN2_CHANNEL,
  PWM_FREQUENCY,
  PWM_RESOLUTION
);

ledcAttachPin(ARM_IN1, ARM_IN1_CHANNEL);
ledcAttachPin(ARM_IN2, ARM_IN2_CHANNEL);
  moveTank(0, 0);
  stopArm();
}