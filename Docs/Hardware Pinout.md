# MiniSkidi V3 Bluepad32

## Hardware Pinout

**Document Version:** 1.0

**Last Updated:** August 7, 2026

---

## Purpose

This document records the confirmed ESP32 hardware connections used by the MiniSkidi V3 Bluepad32 firmware.

Only confirmed pin assignments should be added to this document.

---

## Controller

**Controller:** Sony PlayStation 4 DualShock 4

**Connection:** Bluetooth

**Library:** Bluepad32

No wired controller connection to the ESP32 is required.

---

## Drive Motors

### Left Track Motor

| Function | ESP32 GPIO |
| --- | ---: |
| LEFT_IN1 | 33 |
| LEFT_IN2 | 32 |

### Right Track Motor

| Function | ESP32 GPIO |
| --- | ---: |
| RIGHT_IN1 | 25 |
| RIGHT_IN2 | 26 |

---

## Arm Motor

| Function | ESP32 GPIO |
| --- | ---: |
| ARM_IN1 | 21 |
| ARM_IN2 | 19 |

---

## PWM Configuration

**PWM Frequency:** 20,000 Hz

**PWM Resolution:** 8-bit

### PWM Channels

| Function | Channel |
| --- | ---: |
| LEFT_IN1_CHANNEL | 0 |
| LEFT_IN2_CHANNEL | 1 |
| RIGHT_IN1_CHANNEL | 2 |
| RIGHT_IN2_CHANNEL | 3 |

---

## Motor Assignment

| Motor Number | Function |
| ---: | --- |
| 0 | Left Track |
| 1 | Right Track |
| 2 | Arm |

---

## Servo Connections

Servo pin assignments are not yet documented here.

Confirmed bucket and claw servo connections should be added after their hardware assignments and firmware mappings are verified.

---

## Safety Notes

The drive and arm motors are commanded to stop when the PS4 controller disconnects.

Motor power wiring and ESP32 logic wiring should be verified before testing after hardware changes.

---

## Source of Pin Assignments

The motor GPIO and PWM assignments in this document correspond to the current MiniSkidi Bluepad32 firmware configuration.

Update this document whenever a hardware pin assignment changes.