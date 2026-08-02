#ifndef CONFIG_H
#define CONFIG_H

// ======================================================
// MiniSkidi V4 Configuration
// ======================================================

// ------------------------------
// Motor Numbers
// ------------------------------
constexpr int LEFT_MOTOR  = 0;
constexpr int RIGHT_MOTOR = 1;
constexpr int ARM_MOTOR   = 2;

// ------------------------------
// Motor Directions
// ------------------------------
constexpr int FORWARD  = 1;
constexpr int BACKWARD = -1;
constexpr int STOP     = 0;

// ------------------------------
// Motor Pins
// ------------------------------
constexpr int RIGHT_IN1 = 25;
constexpr int RIGHT_IN2 = 26;

constexpr int LEFT_IN1 = 33;
constexpr int LEFT_IN2 = 32;

constexpr int ARM_IN1 = 21;
constexpr int ARM_IN2 = 19;

// ------------------------------
// Controller Settings
// ------------------------------
constexpr int DEADZONE = 100;

// Future PWM settings
constexpr int MAX_PWM = 255;
constexpr int MIN_PWM = 60;

// Future steering adjustment
constexpr float TURN_GAIN = 1.0f;

#endif