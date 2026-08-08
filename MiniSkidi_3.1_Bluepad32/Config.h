#ifndef CONFIG_H
#define CONFIG_H

// ======================================================
// MiniSkidi V4.3 - Drive Engine 3.0
// "Operate like a real skid steer, not an RC toy."
// ======================================================

// Motor numbers
constexpr int LEFT_MOTOR  = 0;
constexpr int RIGHT_MOTOR = 1;
constexpr int ARM_MOTOR   = 2;

// Motor directions
constexpr int FORWARD  = 1;
constexpr int BACKWARD = -1;
constexpr int STOP     = 0;

// Motor pins
constexpr int RIGHT_IN1 = 25;
constexpr int RIGHT_IN2 = 26;

constexpr int LEFT_IN1 = 33;
constexpr int LEFT_IN2 = 32;

constexpr int ARM_IN1 = 21;
constexpr int ARM_IN2 = 19;

// Controller center dead zone
constexpr int DEADZONE = 100;

// Track PWM settings
constexpr int MIN_PWM = 155;
constexpr int MAX_PWM = 255;

constexpr int TRACK_MIN_PWM = MIN_PWM;
constexpr int TRACK_MAX_PWM = MAX_PWM;

// Below this track command, intentionally stop the track.
// This region should occur only near an intentional pivot.
constexpr float TRACK_STOP_BAND = 0.025f;

// Normal arc steering continues through this portion
// of the turn range.
constexpr float ARC_END = 0.82f;

// Slowest the inside track runs during an ordinary arc.
// 0.35 = 35% of the outside-track command.
constexpr float MIN_ARC_SPEED = 0.35f;

// Higher values produce gentler steering near straight ahead.
constexpr float TURN_RESPONSE = 1.50f;
// Stick magnitude where the outer-ring pivot blend begins.
constexpr float PIVOT_RING_START = 0.88f;

// Minimum sideways direction required for the outer ring
// to request a stronger pivot. 0.75 is roughly 49 degrees
// or more away from straight forward/reverse.
constexpr float PIVOT_DIRECTION_MIN = 0.75f;
// ESP32 PWM settings
constexpr int PWM_FREQUENCY  = 20000;
constexpr int PWM_RESOLUTION = 8;

// Drive PWM channels
constexpr int LEFT_IN1_CHANNEL  = 0;
constexpr int LEFT_IN2_CHANNEL  = 1;
constexpr int RIGHT_IN1_CHANNEL = 2;
constexpr int RIGHT_IN2_CHANNEL = 3;

// ======================================================
// Drive modes
// ======================================================

enum class DriveMode {
  WORK,
  DRIVE,
  TRAVEL
};

// ======================================================
// Work Mode
// ======================================================

constexpr float WORK_SPEED_SCALE        = 0.85f;
constexpr float WORK_RESPONSE_EXPONENT  = 1.25f;

// ======================================================
// Drive Mode
// ======================================================

constexpr float DRIVE_SPEED_SCALE       = 1.00f;

// ======================================================
// Travel Mode
// ======================================================

constexpr float TRAVEL_RESPONSE_EXPONENT = 0.45f;


#endif