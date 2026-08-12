// ======================================================
// MiniSkidi Bucket / Attachment Controls
// ======================================================
//
// Servo control uses dedicated ESP32 hardware PWM.
// ESP32Servo is intentionally not used because the
// drive and arm motors already use LEDC PWM resources.
//
// Bucket:
//   Right stick X = curl / dump
//
// Claw:
//   Reserved for future implementation.
//


// ======================================================
// Bucket Position / Memory State
// ======================================================

constexpr int BUCKET_MIN_US = 700;
constexpr int BUCKET_MAX_US = 2000;

int bucketPositionUs = 1500;

constexpr const char* BUCKET_PREF_NAMESPACE = "bucket";
constexpr const char* BUCKET_PREF_KEY = "position";

unsigned long bucketLastMovedAt = 0;
bool bucketPositionDirty = false;


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

  // No servo command at startup.
  ledcWrite(
    BUCKET_SERVO_CHANNEL,
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
// Bucket Position Memory
// ======================================================

// Load the last saved bucket position.
// Falls back to 1500 if no valid saved value exists.
//
// Loading this value does NOT command the servo.
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


// Save the current commanded bucket position.
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


// Save the bucket position only after movement has
// stopped for 1 second.
//
// This prevents continuous flash writes while the
// operator is moving the bucket.
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
// Hydraulic-Style Bucket Control
// ======================================================
//
// Right stick X controls bucket movement RATE.
//
// Small stick movement = slow movement.
// Large stick movement = faster movement.
// Centered stick = hold current bucket position.

void controlBucket(int bucketValue, int deadzone)
{
  if (abs(bucketValue) <= deadzone) {
    return;
  }

  int magnitude = abs(bucketValue);

  // Current tested bucket response:
  // 1 us/update near center
  // 42 us/update at full stick
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

  bucketLastMovedAt = millis();
  bucketPositionDirty = true;
}