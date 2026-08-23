// ======================================================
// MiniSkidi Bucket / Attachment Controls
// ======================================================
//
// Servo control uses dedicated ESP32 hardware PWM.
// ESP32Servo is intentionally not used because the
// drive and arm motors already use LEDC PWM resources.
//
// Bucket:
//   Right stick X = proportional manual curl / dump
//   R1 = scoop / level preset
//   L1 = dump preset
//
// Claw:
//   R2 = open
//   L2 = close
//


// ======================================================
// Bucket Position / Memory State
// ======================================================

constexpr int BUCKET_MIN_US = 700;
constexpr int BUCKET_MAX_US = 2000;

// Calibrated bucket presets after mechanical adjustment.
constexpr int BUCKET_SCOOP_US = 2000;
constexpr int BUCKET_DUMP_US  = 700;

// Bucket movement is updated at a fixed interval.
// This prevents servo speed from depending on how fast
// the main ESP32 loop happens to execute.
constexpr unsigned long BUCKET_UPDATE_INTERVAL_MS = 20;

int bucketPositionUs = 1500;

constexpr const char* BUCKET_PREF_NAMESPACE = "bucket";
constexpr const char* BUCKET_PREF_KEY = "position";

unsigned long bucketLastMovedAt = 0;
unsigned long bucketLastUpdateAt = 0;

bool bucketPositionDirty = false;

// True while automatically moving toward a preset.
bool bucketPresetActive = false;

// Current automatic preset target.
int bucketPresetTargetUs = BUCKET_SCOOP_US;


// ======================================================
// Claw Position / Memory State
// ======================================================
//
// Calibrated MG90S claw range:
//
// 1000 us = fully closed
// 1500 us = approximately half open
// 2000 us = fully open
//

constexpr int CLAW_MIN_US = 1000;
constexpr int CLAW_MAX_US = 2000;

// Fixed claw update interval.
// Prevents claw speed from depending on main-loop speed.
constexpr unsigned long CLAW_UPDATE_INTERVAL_MS = 20;

int clawPositionUs = 1500;

constexpr const char* CLAW_PREF_NAMESPACE = "claw";
constexpr const char* CLAW_PREF_KEY = "position";

unsigned long clawLastMovedAt = 0;
unsigned long clawLastUpdateAt = 0;

bool clawPositionDirty = false;


// ======================================================
// Bucket PWM Setup
// ======================================================

void setupBucketPWM()
{
  ledcSetup(
    BUCKET_SERVO_CHANNEL,
    BUCKET_SERVO_FREQUENCY,
    BUCKET_SERVO_RESOLUTION
  );

  ledcAttachPin(
    BUCKET_SERVO_PIN,
    BUCKET_SERVO_CHANNEL
  );

  // No bucket position command at startup.
  ledcWrite(
    BUCKET_SERVO_CHANNEL,
    0
  );
}


// ======================================================
// Claw PWM Setup
// ======================================================

void setupClawPWM()
{
  ledcSetup(
    CLAW_SERVO_CHANNEL,
    CLAW_SERVO_FREQUENCY,
    CLAW_SERVO_RESOLUTION
  );

  ledcAttachPin(
    CLAW_SERVO_PIN,
    CLAW_SERVO_CHANNEL
  );

  // No claw position command at startup.
  ledcWrite(
    CLAW_SERVO_CHANNEL,
    0
  );
}


// ======================================================
// Bucket Output
// ======================================================

void writeBucketMicroseconds(int pulseUs)
{
  pulseUs = constrain(
    pulseUs,
    BUCKET_MIN_US,
    BUCKET_MAX_US
  );

  uint32_t duty =
    ((uint32_t)pulseUs * 65535UL) / 20000UL;

  ledcWrite(
    BUCKET_SERVO_CHANNEL,
    duty
  );
}


// ======================================================
// Claw Output
// ======================================================

void writeClawMicroseconds(int pulseUs)
{
  pulseUs = constrain(
    pulseUs,
    CLAW_MIN_US,
    CLAW_MAX_US
  );

  uint32_t duty =
    ((uint32_t)pulseUs * 65535UL) / 20000UL;

  ledcWrite(
    CLAW_SERVO_CHANNEL,
    duty
  );
}


// ======================================================
// Bucket Position Memory
// ======================================================

void loadBucketPosition()
{
  preferences.begin(
    BUCKET_PREF_NAMESPACE,
    true
  );

  int savedPosition =
    preferences.getInt(
      BUCKET_PREF_KEY,
      1500
    );

  preferences.end();

  if (
    savedPosition < BUCKET_MIN_US ||
    savedPosition > BUCKET_MAX_US
  ) {
    savedPosition = 1500;
  }

  bucketPositionUs = savedPosition;

  Serial.printf(
    "Loaded bucket position: %d us\n",
    bucketPositionUs
  );
}


void saveBucketPosition()
{
  preferences.begin(
    BUCKET_PREF_NAMESPACE,
    false
  );

  preferences.putInt(
    BUCKET_PREF_KEY,
    bucketPositionUs
  );

  preferences.end();

  bucketPositionDirty = false;

  Serial.printf(
    "Saved bucket position: %d us\n",
    bucketPositionUs
  );
}


void updateBucketPositionMemory()
{
  if (!bucketPositionDirty) {
    return;
  }

  if (millis() - bucketLastMovedAt < 1000) {
    return;
  }

  saveBucketPosition();
}


// ======================================================
// Claw Position Memory
// ======================================================

void loadClawPosition()
{
  preferences.begin(
    CLAW_PREF_NAMESPACE,
    true
  );

  int savedPosition =
    preferences.getInt(
      CLAW_PREF_KEY,
      1500
    );

  preferences.end();

  if (
    savedPosition < CLAW_MIN_US ||
    savedPosition > CLAW_MAX_US
  ) {
    savedPosition = 1500;
  }

  clawPositionUs = savedPosition;

  Serial.printf(
    "Loaded claw position: %d us\n",
    clawPositionUs
  );
}


void saveClawPosition()
{
  preferences.begin(
    CLAW_PREF_NAMESPACE,
    false
  );

  preferences.putInt(
    CLAW_PREF_KEY,
    clawPositionUs
  );

  preferences.end();

  clawPositionDirty = false;

  Serial.printf(
    "Saved claw position: %d us\n",
    clawPositionUs
  );
}


void updateClawPositionMemory()
{
  if (!clawPositionDirty) {
    return;
  }

  if (millis() - clawLastMovedAt < 1000) {
    return;
  }

  saveClawPosition();
}


// ======================================================
// Hydraulic-Style Bucket Control
// ======================================================
//
// Right stick X controls bucket movement RATE.
//
// Small stick movement = slow movement.
// Large stick movement = faster movement.
//
// Bucket updates are limited to once every 20 ms so
// movement speed does not depend on main-loop speed.
//

void controlBucket(int bucketValue, int deadzone)
{
  if (abs(bucketValue) <= deadzone) {
    return;
  }

  unsigned long now = millis();

  if (
    now - bucketLastUpdateAt <
    BUCKET_UPDATE_INTERVAL_MS
  ) {
    return;
  }

  bucketLastUpdateAt = now;

  int magnitude = abs(bucketValue);

  // Tested hydraulic-style response:
  // 1 us/update near center
  // 42 us/update at full stick.
  int stepUs = map(
    magnitude,
    deadzone,
    512,
    1,
    42
  );

  stepUs = constrain(
    stepUs,
    1,
    42
  );

  if (bucketValue > 0) {
    bucketPositionUs += stepUs;
  }
  else {
    bucketPositionUs -= stepUs;
  }

  bucketPositionUs = constrain(
    bucketPositionUs,
    BUCKET_MIN_US,
    BUCKET_MAX_US
  );

  writeBucketMicroseconds(
    bucketPositionUs
  );

  bucketLastMovedAt = now;
  bucketPositionDirty = true;
}


// ======================================================
// Bucket Preset Control
// ======================================================
//
// R1 = SCOOP / LEVEL = 2000 us
// L1 = DUMP          = 700 us
//
// Preset movement uses the same fixed 20 ms timing.
// Manual right-stick input cancels an active preset.
//

void startBucketScoopPreset()
{
  bucketPresetTargetUs = BUCKET_SCOOP_US;
  bucketPresetActive = true;

  // Allow the first preset step immediately.
  bucketLastUpdateAt = 0;

  Serial.printf(
    "Bucket SCOOP preset started: %d us\n",
    bucketPresetTargetUs
  );
}


void startBucketDumpPreset()
{
  bucketPresetTargetUs = BUCKET_DUMP_US;
  bucketPresetActive = true;

  // Allow the first preset step immediately.
  bucketLastUpdateAt = 0;

  Serial.printf(
    "Bucket DUMP preset started: %d us\n",
    bucketPresetTargetUs
  );
}


void cancelBucketPreset()
{
  if (!bucketPresetActive) {
    return;
  }

  bucketPresetActive = false;

  Serial.println(
    "Bucket preset cancelled"
  );
}


void updateBucketPreset()
{
  if (!bucketPresetActive) {
    return;
  }

  unsigned long now = millis();

  if (
    now - bucketLastUpdateAt <
    BUCKET_UPDATE_INTERVAL_MS
  ) {
    return;
  }

  bucketLastUpdateAt = now;

  // Tested preferred preset speed.
  constexpr int PRESET_STEP_US = 15;

  if (bucketPositionUs < bucketPresetTargetUs) {

    bucketPositionUs += PRESET_STEP_US;

    if (bucketPositionUs >= bucketPresetTargetUs) {
      bucketPositionUs = bucketPresetTargetUs;
    }
  }
  else if (bucketPositionUs > bucketPresetTargetUs) {

    bucketPositionUs -= PRESET_STEP_US;

    if (bucketPositionUs <= bucketPresetTargetUs) {
      bucketPositionUs = bucketPresetTargetUs;
    }
  }
  else {

    bucketPresetActive = false;
    return;
  }

  writeBucketMicroseconds(
    bucketPositionUs
  );

  bucketLastMovedAt = now;
  bucketPositionDirty = true;

  if (bucketPositionUs == bucketPresetTargetUs) {

    bucketPresetActive = false;

    Serial.printf(
      "Bucket preset reached: %d us\n",
      bucketPositionUs
    );
  }
}


// ======================================================
// Hydraulic-Style Claw Control
// ======================================================
//
// R2 = OPEN
// L2 = CLOSE
//
// Hold trigger = move.
// Release trigger = hold.
// Both triggers pressed = no movement.
//
// Claw updates are limited to once every 20 ms so
// movement speed does not depend on main-loop speed.
//

void controlClaw(bool openClaw, bool closeClaw)
{
  if (openClaw == closeClaw) {
    return;
  }

  unsigned long now = millis();

  if (
    now - clawLastUpdateAt <
    CLAW_UPDATE_INTERVAL_MS
  ) {
    return;
  }

  clawLastUpdateAt = now;

  // Previously tested preferred claw step.
  constexpr int CLAW_STEP_US = 20;

  if (openClaw) {
    clawPositionUs += CLAW_STEP_US;
  }
  else {
    clawPositionUs -= CLAW_STEP_US;
  }

  clawPositionUs = constrain(
    clawPositionUs,
    CLAW_MIN_US,
    CLAW_MAX_US
  );

  writeClawMicroseconds(
    clawPositionUs
  );

  clawLastMovedAt = now;
  clawPositionDirty = true;
}