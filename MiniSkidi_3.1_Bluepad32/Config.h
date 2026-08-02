#ifndef CONFIG_H
#define CONFIG_H

// ======================================================
// MiniSkidi V4 Configuration
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

// Controller tuning
constexpr int DEADZONE = 100;

// Drive PWM tuning
constexpr int MAX_PWM = 255;

// Lowest output that reliably starts a track moving.
// Lower this later if the tracks start too abruptly.
constexpr int MIN_PWM = 95;

// Steering sensitivity
constexpr float TURN_GAIN = 1.0f;

// ESP32 PWM settings
constexpr int PWM_FREQUENCY = 20000;
constexpr int PWM_RESOLUTION = 8;

// PWM channels
constexpr int LEFT_IN1_CHANNEL  = 0;
constexpr int LEFT_IN2_CHANNEL  = 1;
constexpr int RIGHT_IN1_CHANNEL = 2;
constexpr int RIGHT_IN2_CHANNEL = 3;

#endif