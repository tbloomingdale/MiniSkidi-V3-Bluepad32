// ======================================================
// MiniSkidi Drive Engine 2.0
// ======================================================


// Reads the left stick as one circular control instead of
// treating X and Y as two unrelated axes.
//
// Outputs:
//   throttle       -1.0 to +1.0
//   steering       -1.0 to +1.0
//   stickMagnitude  0.0 to 1.0
void readDriveStick(
  float& throttle,
  float& steering,
  float& stickMagnitude
) {
  float rawX = static_cast<float>(myController->axisX());
  float rawY = static_cast<float>(-myController->axisY());

  float rawMagnitude =
    sqrtf((rawX * rawX) + (rawY * rawY));

  if (rawMagnitude <= static_cast<float>(DEADZONE)) {
    throttle = 0.0f;
    steering = 0.0f;
    stickMagnitude = 0.0f;
    return;
  }

  // Direction of the stick.
  float directionX = rawX / rawMagnitude;
  float directionY = rawY / rawMagnitude;

  // Remove the radial dead zone and rescale the remaining travel.
  float limitedMagnitude = min(rawMagnitude, 512.0f);

  stickMagnitude =
    (limitedMagnitude - static_cast<float>(DEADZONE)) /
    (512.0f - static_cast<float>(DEADZONE));

  stickMagnitude =
    constrain(stickMagnitude, 0.0f, 1.0f);

  steering = directionX * stickMagnitude;
  throttle = directionY * stickMagnitude;
}


// Converts a normalized track command into usable PWM.
//
// Small values deliberately become zero.
// Values outside that stop band begin at MIN_PWM, avoiding
// the weak motor-stall region.
int trackCommandToPWM(float command)
{
  command = constrain(command, -1.0f, 1.0f);

  float magnitude = abs(command);

  if (magnitude <= TRACK_ZERO_BAND) {
    return 0;
  }

  float usableMagnitude =
    (magnitude - TRACK_ZERO_BAND) /
    (1.0f - TRACK_ZERO_BAND);

  usableMagnitude =
    constrain(usableMagnitude, 0.0f, 1.0f);

  int pwm =
    MIN_PWM +
    static_cast<int>(
      usableMagnitude *
      static_cast<float>(MAX_PWM - MIN_PWM)
    );

  pwm = constrain(pwm, MIN_PWM, MAX_PWM);

  return command > 0.0f ? pwm : -pwm;
}


// Continuous skid-steer mixer.
//
// Straight:
//   Both tracks run together.
//
// Increasing steering:
//   Outside track holds speed.
//   Inside track progressively slows.
//
// More steering:
//   Inside track passes through stop and begins reversing.
//
// Full sideways:
//   Tracks run opposite directions for a pivot.
//
// There is no separate pivot mode and no abrupt transition.
void driveMixer2(
  float throttle,
  float steering,
  float stickMagnitude,
  float& leftTrack,
  float& rightTrack
) {
  leftTrack  = 0.0f;
  rightTrack = 0.0f;

  float throttleMagnitude = abs(throttle);
  float steeringMagnitude = abs(steering);

  if (stickMagnitude <= 0.0f) {
    return;
  }

  float totalIntent =
    throttleMagnitude + steeringMagnitude;

  if (totalIntent <= 0.001f) {
    return;
  }

  // 0.0 = straight travel
  // 1.0 = full sideways pivot
  float turnRatio =
    steeringMagnitude / totalIntent;

  turnRatio = constrain(turnRatio, 0.0f, 1.0f);

  // Makes steering gentle near straight ahead while remaining
  // continuous all the way into a pivot.
  float shapedTurn =
    powf(turnRatio, TURN_CURVE);

  // Progresses continuously:
  // +1.0 = inside track matches outside track
  //  0.0 = inside track stopped
  // -1.0 = inside track fully reversed
  float insideFactor =
    1.0f - (2.0f * shapedTurn);

  float travelDirection;

  if (throttle > 0.001f) {
    travelDirection = 1.0f;
  }
  else if (throttle < -0.001f) {
    travelDirection = -1.0f;
  }
  else {
    // Pure sideways movement performs a forward-oriented pivot.
    travelDirection = 1.0f;
  }

  float outsideTrack =
    travelDirection * stickMagnitude;

  float insideTrack =
    outsideTrack * insideFactor;

  bool steeringRight = steering > 0.0f;
  bool movingForward = travelDirection > 0.0f;

  // This makes reverse steering mirror forward steering.
  bool outsideTrackIsLeft =
    (steeringRight == movingForward);

  if (steeringMagnitude <= 0.001f) {
    leftTrack  = outsideTrack;
    rightTrack = outsideTrack;
  }
  else if (outsideTrackIsLeft) {
    leftTrack  = outsideTrack;
    rightTrack = insideTrack;
  }
  else {
    leftTrack  = insideTrack;
    rightTrack = outsideTrack;
  }

  leftTrack =
    constrain(leftTrack, -1.0f, 1.0f);

  rightTrack =
    constrain(rightTrack, -1.0f, 1.0f);
}

void updateDriveDiagnosticLED(
  int leftPWM,
  int rightPWM,
  float stickMagnitude
) {
  static int previousState = -1;

  int state;

  if (stickMagnitude <= 0.01f) {
    // Blue: stick is centered.
    state = 0;
  }
  else if (leftPWM == 0 && rightPWM == 0) {
    // Red: stick is moved, but software commanded both tracks to stop.
    state = 1;
  }
  else if (leftPWM == 0 || rightPWM == 0) {
    // Yellow: software commanded one track to stop.
    state = 2;
  }
  else {
    // Green: software commanded both tracks to move.
    state = 3;
  }

  if (state == previousState) {
    return;
  }

  previousState = state;

  switch (state) {
    case 0:
      myController->setColorLED(0, 0, 255);       // Blue
      break;

    case 1:
      myController->setColorLED(255, 0, 0);       // Red
      break;

    case 2:
      myController->setColorLED(255, 160, 0);     // Yellow
      break;

    case 3:
      myController->setColorLED(0, 255, 0);       // Green
      break;
  }
}
void processController()
{
  float throttle = 0.0f;
  float steering = 0.0f;
  float stickMagnitude = 0.0f;

  readDriveStick(
    throttle,
    steering,
    stickMagnitude
  );

  float leftTrack  = 0.0f;
  float rightTrack = 0.0f;

  driveMixer2(
    throttle,
    steering,
    stickMagnitude,
    leftTrack,
    rightTrack
  );

  int leftPWM =
    trackCommandToPWM(leftTrack);

  int rightPWM =
    trackCommandToPWM(rightTrack);

  moveTank(leftPWM, rightPWM);
  
  updateDriveDiagnosticLED(leftPWM, rightPWM, stickMagnitude);

  // Right stick arm operation remains unchanged.
  int arm = myController->axisRY();

  if (abs(arm) < DEADZONE) {
    arm = 0;
  }

  controlArm(arm, DEADZONE);

  Serial.printf(
    "Throttle:%6.2f Steering:%6.2f Mag:%5.2f "
    "Left:%4d Right:%4d Arm:%4d\n",
    throttle,
    steering,
    stickMagnitude,
    leftPWM,
    rightPWM,
    arm
  );

  delay(20);
}