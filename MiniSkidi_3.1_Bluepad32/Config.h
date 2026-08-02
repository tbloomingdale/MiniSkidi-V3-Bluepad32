#ifndef CONFIG_H
#define CONFIG_H

// ======================================================
// MiniSkidi V4.2 - Drive Engine 2.0
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
constexpr int MIN_PWM = 95;
constexpr int MAX_PWM = 255;

// Commands smaller than this intentionally stop the track.
// This avoids commanding a motor inside its unusable stall range.
constexpr float TRACK_ZERO_BAND = 0.08f;

// Higher values make ordinary steering gentler.
// Suggested tuning range: 1.2 to 2.2
constexpr float TURN_CURVE = 1.60f;

// ESP32 PWM settings
constexpr int PWM_FREQUENCY  = 20000;
constexpr int PWM_RESOLUTION = 8;

// Drive PWM channels
constexpr int LEFT_IN1_CHANNEL  = 0;
constexpr int LEFT_IN2_CHANNEL  = 1;
constexpr int RIGHT_IN1_CHANNEL = 2;
constexpr int RIGHT_IN2_CHANNEL = 3;

#endif