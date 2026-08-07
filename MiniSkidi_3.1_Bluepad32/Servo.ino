#include <ESP32Servo.h>

Servo bucketServo;
Servo clawServo;

constexpr int BUCKET_SERVO_PIN = 23;
constexpr int CLAW_SERVO_PIN   = 22;

void setupServos() {
  bucketServo.attach(BUCKET_SERVO_PIN);
  clawServo.attach(CLAW_SERVO_PIN);

  bucketServo.write(90);
  clawServo.write(90);
}

void testBucketServo() {
  static int angle = 90;
  static unsigned long lastMove = 0;

  if (millis() - lastMove > 1000) {
    lastMove = millis();

    if (angle == 90)
      angle = 30;
    else
      angle = 150;

    bucketServo.write(angle);

    Serial.print("Bucket angle: ");
    Serial.println(angle);
  }
}