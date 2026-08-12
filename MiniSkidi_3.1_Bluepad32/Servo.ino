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
// Bucket PWM Setup
// ======================================================
//
// Channel 8 is used for the bucket servo at 50 Hz.
// Duty starts at zero so the servo receives no position
// command until we explicitly send one.

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

  ledcWrite(
    BUCKET_SERVO_CHANNEL,
    0
  );
}


// ======================================================
// Bucket Center Test
// ======================================================

void centerBucketServo()
{
  // 1500 microseconds = approximate servo center.
  //
  // At 50 Hz:
  // one PWM period = 20,000 microseconds
  //
  // 16-bit LEDC range = 0 to 65535.

  uint32_t duty =
    (1500UL * 65535UL) / 20000UL;

  ledcWrite(
    BUCKET_SERVO_CHANNEL,
    duty
  );
}
// ======================================================
// Bucket Position Control
// ======================================================

// Conservative starting limits.
// We will expand these only after physical testing.
constexpr int BUCKET_MIN_US = 700;
constexpr int BUCKET_MAX_US = 2000;

// Starting software position.
// This does NOT move the servo by itself.
int bucketPositionUs = 1500;


// Send a pulse width in microseconds to the bucket servo.
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
// Hydraulic-Style Bucket Control
// ======================================================
//
// Right stick X controls bucket movement RATE.
//
// Small stick movement = slow bucket movement.
// Large stick movement = faster bucket movement.
// Centered stick = hold current bucket position.

void controlBucket(int bucketValue, int deadzone)
{
  if (abs(bucketValue) <= deadzone) {
    return;
  }

  int magnitude = abs(bucketValue);

  // Convert stick travel into movement per controller update.
  //
  // 1 us per update = very fine movement
  // 6 us per update = full-speed movement
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
}