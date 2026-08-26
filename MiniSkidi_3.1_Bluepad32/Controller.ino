// ======================================================
// MiniSkidi Drive Engine 3.0
// ======================================================

// ======================================================
// Drive Mode Manager
// ======================================================

DriveMode currentDriveMode = DriveMode::DRIVE;
// ======================================================
// Auxiliary Light State
// ======================================================

bool auxLightsOn = false;
// ======================================================
// Auxiliary Light Control
// ======================================================
//
// Original MiniSkidi wiring:
//
// GPIO 18 HIGH + GPIO 5 LOW = lights ON
// GPIO 18 LOW  + GPIO 5 LOW = lights OFF
//
// R3 toggles the lights.
//

void setupAuxLights()
{
  pinMode(AUX_LIGHTS_IN1, OUTPUT);
  pinMode(AUX_LIGHTS_IN2, OUTPUT);

  // Start with lights OFF.
  digitalWrite(AUX_LIGHTS_IN1, LOW);
  digitalWrite(AUX_LIGHTS_IN2, LOW);

  auxLightsOn = false;
}


void updateAuxLights()
{
  static bool previousR3 = false;

  bool currentR3 = myController->thumbR();

  // Act only when R3 changes from
  // released to pressed.
  if (currentR3 && !previousR3) {

    auxLightsOn = !auxLightsOn;

    if (auxLightsOn) {
      digitalWrite(AUX_LIGHTS_IN1, HIGH);
      digitalWrite(AUX_LIGHTS_IN2, LOW);

      Serial.println("Aux lights: ON");
    }
    else {
      digitalWrite(AUX_LIGHTS_IN1, LOW);
      digitalWrite(AUX_LIGHTS_IN2, LOW);

      Serial.println("Aux lights: OFF");
    }
  }

  previousR3 = currentR3;
}

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

// Provides tactile confirmation when changing drive modes.
//
// Work   = 1 pulse
// Drive  = 2 pulses
// Travel = 3 pulses
void rumbleDriveMode()
{
  if (myController == nullptr ||
      !myController->isConnected()) {
    return;
  }

  switch (currentDriveMode) {

    case DriveMode::WORK:
      myController->playDualRumble(
        0,
        180,
        120,
        200
      );
      break;

    case DriveMode::DRIVE:
      myController->playDualRumble(
        0,
        180,
        120,
        200
      );

      myController->playDualRumble(
        300,
        180,
        120,
        200
      );
      break;

    case DriveMode::TRAVEL:
      myController->playDualRumble(
        0,
        180,
        120,
        200
      );

      myController->playDualRumble(
        300,
        180,
        120,
        200
      );

      myController->playDualRumble(
        600,
        180,
        120,
        200
      );
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
  rumbleDriveMode();

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

// ======================================================
// Drive Engine 4 - Track Differential Mixer
// ======================================================
//
// Normal turns:
//   Outside track holds commanded speed.
//   Inside track slows but never stops.
//
// Pivot:
//   Only begins when the stick is very close to full sideways.
//   Inside track reverses for a true skid-steer pivot.
//
// This avoids the old arc-to-pivot zero-speed transition.
void driveMixer3(
  float throttle,
  float steering,
  float stickMagnitude,
  float& leftTrack,
  float& rightTrack
) {
  leftTrack  = 0.0f;
  rightTrack = 0.0f;

  if (stickMagnitude <= 0.001f) {
    return;
  }

  float steeringMagnitude = abs(steering);

  // 0.0 = straight ahead / reverse
  // 1.0 = completely sideways
  float turnRatio =
    steeringMagnitude / stickMagnitude;

  turnRatio =
    constrain(turnRatio, 0.0f, 1.0f);

  // Drive Engine 4 pivot boundary.
  //
  // Ordinary diagonal stick positions remain arc turns.
  // Pivot begins only very close to full sideways.
  constexpr float DE4_PIVOT_START = 0.96f;

  // ------------------------------------------------------
  // NORMAL ARC STEERING
  // ------------------------------------------------------
  if (turnRatio < DE4_PIVOT_START) {

    float arcPosition =
      turnRatio / DE4_PIVOT_START;

    arcPosition =
      constrain(arcPosition, 0.0f, 1.0f);

    float shapedTurn =
      powf(arcPosition, TURN_RESPONSE);

    float insideFactor =
      1.0f -
      ((1.0f - MIN_ARC_SPEED) * shapedTurn);

    // The inside track is never allowed below
    // MIN_ARC_SPEED during a normal turn.
    insideFactor =
      constrain(
        insideFactor,
        MIN_ARC_SPEED,
        1.0f
      );

    float travelDirection = 1.0f;

    if (throttle < -0.001f) {
      travelDirection = -1.0f;
    }

    float outsideTrack =
      travelDirection * stickMagnitude;

    float insideTrack =
      outsideTrack * insideFactor;

    bool steeringRight =
      steering > 0.0f;

    bool movingForward =
      travelDirection > 0.0f;

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
  }

  // ------------------------------------------------------
  // PIVOT STEERING
  // ------------------------------------------------------
  else {

    float pivotPosition =
      (turnRatio - DE4_PIVOT_START) /
      (1.0f - DE4_PIVOT_START);

    pivotPosition =
      constrain(pivotPosition, 0.0f, 1.0f);

    float reverseFactor =
      MIN_ARC_SPEED +
      ((1.0f - MIN_ARC_SPEED) * pivotPosition);

    float outsideTrack =
      stickMagnitude;

    float insideTrack =
      -stickMagnitude * reverseFactor;

    if (steering > 0.0f) {
      // Right pivot
      leftTrack  = outsideTrack;
      rightTrack = insideTrack;
    }
    else {
      // Left pivot
      leftTrack  = insideTrack;
      rightTrack = outsideTrack;
    }
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
  updateAuxLights();

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
// ======================================================
// Bucket Control
// Right stick X = manual curl / dump
//
// Manual bucket input immediately cancels
// any active automatic bucket preset.
// ======================================================

int bucketValue = myController->axisRX();

if (abs(bucketValue) > DEADZONE) {

  // Operator takes immediate manual control.
  cancelBucketPreset();

  controlBucket(
    bucketValue,
    DEADZONE
  );
}
else {

  // No manual input:
  // allow an active preset to continue moving.
  updateBucketPreset();
}

// ======================================================
// Bucket Preset Controls
//
// R1 tap = SCOOP / LEVEL preset
// L1 tap = DUMP preset
// ======================================================

uint16_t buttons = myController->buttons();

static bool previousR1 = false;
static bool previousL1 = false;

bool currentR1 =
  buttons & BUTTON_SHOULDER_R;

bool currentL1 =
  buttons & BUTTON_SHOULDER_L;


// R1 pressed once:
// start automatic SCOOP / LEVEL movement.
if (currentR1 && !previousR1) {

  startBucketScoopPreset();

  Serial.println(
    "R1: Scoop preset requested"
  );
}


// L1 pressed once:
// start automatic DUMP movement.
if (currentL1 && !previousL1) {

  startBucketDumpPreset();

  Serial.println(
    "L1: Dump preset requested"
  );
}


previousR1 = currentR1;
previousL1 = currentL1;
// ======================================================
// Claw Control
//
// R2 = OPEN
// L2 = CLOSE
//
// Triggers are treated as on/off controls for now.
// Claw movement speed remains unchanged.
// ======================================================

int r2Value = myController->throttle();
int l2Value = myController->brake();

constexpr int CLAW_TRIGGER_THRESHOLD = 100;

bool openClaw =
  r2Value > CLAW_TRIGGER_THRESHOLD;

bool closeClaw =
  l2Value > CLAW_TRIGGER_THRESHOLD;

controlClaw(
  openClaw,
  closeClaw
);
}