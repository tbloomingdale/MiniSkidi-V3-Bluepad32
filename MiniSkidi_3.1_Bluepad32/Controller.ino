int axisToPWM(int axisValue)
{
  if (abs(axisValue) < DEADZONE) {
    return 0;
  }

  int magnitude = abs(axisValue);

  magnitude = map(
    magnitude,
    DEADZONE,
    512,
    MIN_PWM,
    MAX_PWM
  );

  magnitude = constrain(magnitude, MIN_PWM, MAX_PWM);

  return axisValue > 0 ? magnitude : -magnitude;
}

void processController()
{
  // Left stick:
  // Up/down = forward/reverse
  // Left/right = steering
  int driveAxis = -myController->axisY();
  int turnAxis  =  myController->axisX();

  int drivePWM = axisToPWM(driveAxis);
  int turnPWM  = axisToPWM(turnAxis);

  turnPWM = static_cast<int>(turnPWM * TURN_GAIN);

  int leftSpeed  = drivePWM + turnPWM;
  int rightSpeed = drivePWM - turnPWM;

  // Normalize the mixed values so neither exceeds MAX_PWM.
  int largestMagnitude = max(abs(leftSpeed), abs(rightSpeed));

  if (largestMagnitude > MAX_PWM) {
    leftSpeed =
      static_cast<long>(leftSpeed) * MAX_PWM /
      largestMagnitude;

    rightSpeed =
      static_cast<long>(rightSpeed) * MAX_PWM /
      largestMagnitude;
  }

  moveTank(leftSpeed, rightSpeed);

  // Right stick controls the arm.
  int arm = myController->axisRY();

  if (abs(arm) < DEADZONE) {
    arm = 0;
  }

  controlArm(arm, DEADZONE);

  Serial.printf(
    "Drive:%4d Turn:%4d LeftPWM:%4d RightPWM:%4d Arm:%4d\n",
    drivePWM,
    turnPWM,
    leftSpeed,
    rightSpeed,
    arm
  );

  delay(20);
}