// ======================================================
// MiniSkidi Drive Engine 3.0
// ======================================================

// ======================================================
// Drive Mode Manager
// ======================================================

DriveMode currentDriveMode = DriveMode::DRIVE;


// Returns the name used by the Serial Monitor.
const char* getDriveModeName()
{
  switch (currentDriveMode) {
    case DriveMode::WORK:
      return "WORK";

    case DriveMode::TRAVEL:
      return "TRAVEL";

    case DriveMode::DRIVE:
    default:
      return "DRIVE";
  }
}


// Sets the PS4 light-bar color for the current drive mode.
//
// Blue  = Work / precision
// Green = Drive / normal
// Red   = Travel
void setDriveModeLED()
{
  if (myController == nullptr ||
      !myController->isConnected()) {
    return;
  }

  switch (currentDriveMode) {
    case DriveMode::WORK:
      myController->setColorLED(0, 0, 255);
      break;

    case DriveMode::TRAVEL:
      myController->setColorLED(255, 0, 0);
      break;

    case DriveMode::DRIVE:
    default:
      myController->setColorLED(0, 255, 0);
      break;
  }
}


// Selects drive modes with one D-pad tap.
//
// D-pad Up    = Travel
// D-pad Right = Drive
// D-pad Down  = Work
//
// Edge detection prevents a held button from repeatedly
// selecting the same mode.
void updateDriveModeFromDpad()
{
  static uint8_t previousDpad = 0;

  uint8_t currentDpad = myController->dpad();
  uint8_t newlyPressed =
    currentDpad & static_cast<uint8_t>(~previousDpad);

  DriveMode requestedMode = currentDriveMode;

  if (newlyPressed & DPAD_UP) {
    requestedMode = DriveMode::TRAVEL;
  }
  else if (newlyPressed & DPAD_RIGHT) {
    requestedMode = DriveMode::DRIVE;
  }
  else if (newlyPressed & DPAD_DOWN) {
    requestedMode = DriveMode::WORK;
  }

  previousDpad = currentDpad;

  if (requestedMode == currentDriveMode) {
    return;
  }

  currentDriveMode = requestedMode;

  setDriveModeLED();

  Serial.printf(
    "Drive Mode: %s\n",
    getDriveModeName()
  );
}


// Adjusts stick magnitude without changing steering direction.
//
// Work:
//   Reduces maximum drive output.
//
// Drive:
//   Preserves the current Drive Engine 3.1 response.
//
// Travel:
//   Reaches stronger output earlier while retaining the same
//   full-stick maximum and the same steering direction.
float applyDriveModeToMagnitude(float stickMagnitude)
{
  stickMagnitude =
    constrain(stickMagnitude, 0.0f, 1.0f);

  switch (currentDriveMode) {
    case DriveMode::WORK:
  return
    powf(stickMagnitude, WORK_RESPONSE_EXPONENT) *
    WORK_SPEED_SCALE;

    case DriveMode::TRAVEL:
      return powf(
        stickMagnitude,
        TRAVEL_RESPONSE_EXPONENT
      );

    case DriveMode::DRIVE:
    default:
      return stickMagnitude * DRIVE_SPEED_SCALE;
  }
}

// Reads the complete left-stick position using one circular
// dead zone.
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

  float directionX = rawX / rawMagnitude;
  float directionY = rawY / rawMagnitude;

  float limitedMagnitude = min(rawMagnitude, 512.0f);

  stickMagnitude =
    (limitedMagnitude - static_cast<float>(DEADZONE)) /
    (512.0f - static_cast<float>(DEADZONE));

  stickMagnitude =
    constrain(stickMagnitude, 0.0f, 1.0f);

  steering = directionX * stickMagnitude;
  throttle = directionY * stickMagnitude;
}


// Produces the inside-track speed factor.
//
// At straight ahead:
//   factor = +1.0
//
// Through normal arc turns:
//   factor gradually decreases, but stays above MIN_ARC_SPEED.
//
// Near full sideways:
//   factor passes through zero and continues toward -1.0.
//
// This prevents ordinary diagonal stick positions from
// stopping the inside track.
float calculateInsideTrackFactor(float turnRatio)
{
  turnRatio = constrain(turnRatio, 0.0f, 1.0f);

  // Normal arc-turn region.
  if (turnRatio <= ARC_END) {
    float arcPosition = turnRatio / ARC_END;

    arcPosition =
      constrain(arcPosition, 0.0f, 1.0f);

    float shapedPosition =
      powf(arcPosition, TURN_RESPONSE);

    return
      1.0f -
      ((1.0f - MIN_ARC_SPEED) * shapedPosition);
  }

  // Final transition from tight arc into pivot.
  float pivotPosition =
    (turnRatio - ARC_END) /
    (1.0f - ARC_END);

  pivotPosition =
    constrain(pivotPosition, 0.0f, 1.0f);

  // Smoothstep removes an abrupt change at ARC_END.
  float smoothPivot =
    pivotPosition *
    pivotPosition *
    (3.0f - (2.0f * pivotPosition));

  return
    MIN_ARC_SPEED +
    ((-1.0f - MIN_ARC_SPEED) * smoothPivot);
}


// Converts a normalized track speed into signed PWM.
//
// Any usable nonzero track command begins at TRACK_MIN_PWM,
// avoiding the motor's weak stall region.
int trackSpeedToPWM(float trackSpeed)
{
  trackSpeed =
    constrain(trackSpeed, -1.0f, 1.0f);

  float magnitude = abs(trackSpeed);

  if (magnitude <= TRACK_STOP_BAND) {
    return 0;
  }

  float usableMagnitude =
    (magnitude - TRACK_STOP_BAND) /
    (1.0f - TRACK_STOP_BAND);

  usableMagnitude =
    constrain(usableMagnitude, 0.0f, 1.0f);

  int pwm =
    TRACK_MIN_PWM +
    static_cast<int>(
      usableMagnitude *
      static_cast<float>(TRACK_MAX_PWM - TRACK_MIN_PWM)
    );

  pwm =
    constrain(pwm, TRACK_MIN_PWM, TRACK_MAX_PWM);

  return trackSpeed > 0.0f ? pwm : -pwm;
}


// Outside-track-priority skid-steer mixer.
//
// Small steering:
//   Both tracks continue moving.
//
// Increasing steering:
//   Inside track slows while outside track holds speed.
//
// Near full sideways:
//   Inside track smoothly crosses zero and reverses.
//
// Full sideways:
//   True pivot.
void driveMixer3(
  float throttle,
  float steering,
  float stickMagnitude,
  float& leftTrack,
  float& rightTrack
) {
  leftTrack  = 0.0f;
  rightTrack = 0.0f;

  if (stickMagnitude <= 0.0f) {
    return;
  }

  float throttleMagnitude = abs(throttle);
  float steeringMagnitude = abs(steering);

  float totalIntent =
    throttleMagnitude + steeringMagnitude;

  if (totalIntent <= 0.001f) {
    return;
  }

// 0.0 = straight
// 1.0 = full sideways
float turnRatio =
  steeringMagnitude / stickMagnitude;

turnRatio =
  constrain(turnRatio, 0.0f, 1.0f);


// Outer-ring pivot assistance.
//
// Normal stick movement keeps the existing smooth arc behavior.
//
// When the operator pushes the stick close to its outer gate
// and points it mostly sideways, the drive engine blends more
// confidently toward a pivot. This makes pivoting discoverable
// by feel without requiring a perfectly horizontal stick.
if (stickMagnitude >= PIVOT_RING_START &&
    turnRatio >= PIVOT_DIRECTION_MIN) {

  float ringPosition =
    (stickMagnitude - PIVOT_RING_START) /
    (1.0f - PIVOT_RING_START);

  ringPosition =
    constrain(ringPosition, 0.0f, 1.0f);

  float directionPosition =
    (turnRatio - PIVOT_DIRECTION_MIN) /
    (1.0f - PIVOT_DIRECTION_MIN);

  directionPosition =
    constrain(directionPosition, 0.0f, 1.0f);

  float pivotIntent =
    ringPosition * directionPosition;

  // Blend the existing turn ratio toward full pivot.
  turnRatio =
    turnRatio +
    ((1.0f - turnRatio) * pivotIntent);

  turnRatio =
    constrain(turnRatio, 0.0f, 1.0f);
}

float insideFactor =
  calculateInsideTrackFactor(turnRatio);

  float travelDirection;

  if (throttle > 0.001f) {
    travelDirection = 1.0f;
  }
  else if (throttle < -0.001f) {
    travelDirection = -1.0f;
  }
  else {
    // With no forward/reverse input, full sideways movement
    // produces a normal pivot.
    travelDirection = 1.0f;
  }

  float outsideTrack =
    travelDirection * stickMagnitude;

  float insideTrack =
    outsideTrack * insideFactor;

  bool steeringRight = steering > 0.0f;
  bool movingForward = travelDirection > 0.0f;

  // Reverse steering mirrors forward steering.
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


// Temporary PS4 light-bar diagnostics.
//
// Blue   = stick centered
// Green  = both tracks commanded to move
// Yellow = one track commanded to stop
// Red    = both tracks stopped while stick is displaced
void updateDriveDiagnosticLED(
  int leftPWM,
  int rightPWM,
  float stickMagnitude
) {
  static int previousState = -1;

  int state;

  if (stickMagnitude <= 0.01f) {
    state = 0;
  }
  else if (leftPWM == 0 && rightPWM == 0) {
    state = 1;
  }
  else if (leftPWM == 0 || rightPWM == 0) {
    state = 2;
  }
  else {
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
  updateDriveModeFromDpad();

  float throttle = 0.0f;
  float steering = 0.0f;
  float stickMagnitude = 0.0f;

  readDriveStick(
    throttle,
    steering,
    stickMagnitude
  );

float profiledMagnitude =
  applyDriveModeToMagnitude(stickMagnitude);

float profileScale = 0.0f;

if (stickMagnitude > 0.001f) {
  profileScale =
    profiledMagnitude / stickMagnitude;
}

float profiledThrottle =
  throttle * profileScale;

float profiledSteering =
  steering * profileScale;

float leftTrack  = 0.0f;
float rightTrack = 0.0f;

driveMixer3(
  profiledThrottle,
  profiledSteering,
  profiledMagnitude,
  leftTrack,
  rightTrack
);

  int leftPWM =
    trackSpeedToPWM(leftTrack);

  int rightPWM =
    trackSpeedToPWM(rightTrack);

  moveTank(leftPWM, rightPWM);

 // updateDriveDiagnosticLED(
//   leftPWM,
//   rightPWM,
//   stickMagnitude
// );

  // Right stick arm operation remains unchanged.
  int arm = myController->axisRY();

  if (abs(arm) < DEADZONE) {
    arm = 0;
  }

  controlArm(arm, DEADZONE);

  Serial.printf(
  "Mode:%-6s Throttle:%6.2f Steering:%6.2f "
  "Mag:%5.2f Profile:%5.2f "
  "Left:%4d Right:%4d Arm:%4d\n",
  getDriveModeName(),
  throttle,
  steering,
  stickMagnitude,
  profiledMagnitude,
  leftPWM,
  rightPWM,
  arm
);

  delay(20);
}