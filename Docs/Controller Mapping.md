# MiniSkidi V3 Bluepad32

## Controller Mapping

**Document Version:** 1.0

**Last Updated:** August 7, 2026

---

## Purpose

This document describes the PlayStation 4 controller functions used by the MiniSkidi Bluepad32 firmware.

The MiniSkidi firmware is designed for a PlayStation 4 controller using the Bluepad32 library.

---

## Controller

**Controller:** Sony PlayStation 4 DualShock 4

**Controller Library:** Bluepad32

---

## Left Stick - Drive Control

The left analog stick controls both tracks.

### Forward

Push the left stick forward.

Both tracks drive forward.

### Reverse

Pull the left stick backward.

Both tracks drive in reverse.

### Right Turn

Move the left stick toward the right while moving forward or reverse.

The outside track runs faster while the inside track slows to produce an arc turn.

### Left Turn

Move the left stick toward the left while moving forward or reverse.

The outside track runs faster while the inside track slows to produce an arc turn.

### Right Pivot

Move the left stick fully to the right.

The tracks run in opposite directions to pivot the MiniSkidi to the right.

### Left Pivot

Move the left stick fully to the left.

The tracks run in opposite directions to pivot the MiniSkidi to the left.

---

## Right Stick - Arm Control

The vertical axis of the right analog stick controls the loader arm.

### Raise Arm

Move the right stick upward.

### Lower Arm

Move the right stick downward.

### Horizontal Axis

Currently unused.

---

## D-Pad - Drive Mode Selection

The D-pad selects the operating mode.

### D-Pad Up - Travel Mode

**Controller LED:** Red

Travel Mode provides the fastest travel response.

### D-Pad Right - Drive Mode

**Controller LED:** Green

Drive Mode provides the normal general-purpose driving response.

### D-Pad Down - Work Mode

**Controller LED:** Blue

Work Mode provides slower, more controllable response for precision operation.

### D-Pad Left

Reserved for future use.

---

## Drive Modes

### Work Mode

**LED Color:** Blue

Current speed scale:

`0.85`

Current response exponent:

`1.25`

Work Mode is intended for controlled operation while working with the bucket, claw, or other attachments.

The response curve allows the operator to feather the tracks while still providing enough motor power to keep the tracks moving.

---

### Drive Mode

**LED Color:** Green

Current speed scale:

`1.00`

Drive Mode is the normal general-purpose operating mode.

---

### Travel Mode

**LED Color:** Red

Current response exponent:

`0.45`

Travel Mode provides quicker response for moving the MiniSkidi between work areas.

---

## Controller Feedback

The PS4 controller light bar indicates the currently selected drive mode.

| Mode | LED Color |
| --- | --- |
| Work | Blue |
| Drive | Green |
| Travel | Red |

---

## Face Buttons

### Square

Reserved for future use.

### Triangle

Reserved for future use.

### Circle

Reserved for future use.

### Cross (X)

Reserved for future use.

---

## Shoulder Controls

### L1

Reserved for future use.

### L2

Reserved for future use.

### R1

Reserved for future use.

### R2

Reserved for future use.

---

## Center Controls

### Share

Reserved for future use.

### Options

Reserved for future use.

### Touchpad

Reserved for future use.

### PS Button

Standard controller function.

---

## Controller Disconnect Safety

If the PS4 controller disconnects while the MiniSkidi is operating:

- Both drive tracks are commanded to stop.
- The arm motor is commanded to stop.

This safety behavior has been tested successfully.

---

## Future Controller Functions

Future controller mappings may include:

- Bucket servo
- Claw servo
- Lights
- Horn
- Additional accessories

These functions should be documented here when they are implemented.

---

## Drive Engine 4 Development Note

Drive Engine 4 is currently under development.

The primary steering goal is smooth proportional arc steering without unexpected inside-track stopping.

Desired joystick behavior:

| Stick Position | Desired Behavior |
| --- | --- |
| 12 o'clock | Straight forward |
| 1 o'clock | Forward-right arc |
| 2 o'clock | Tighter forward-right arc; both tracks moving |
| 3 o'clock | Right pivot |
| 4 o'clock | Reverse-right arc; both tracks moving |
| 5 o'clock | Reverse-right arc; both tracks moving |
| 6 o'clock | Straight reverse |
| 7 o'clock | Reverse-left arc; both tracks moving |
| 8 o'clock | Tighter reverse-left arc; both tracks moving |
| 9 o'clock | Left pivot |
| 10 o'clock | Forward-left arc; both tracks moving |
| 11 o'clock | Forward-left arc; both tracks moving |

The inside track should remain moving during normal arc steering.

A stopped or reversing inside track should occur only when transitioning into an intentional pivot.

---

## Notes

This document describes the current controller design and intended Drive Engine 4 behavior.

Update this document whenever controller mappings or drive-mode behavior change.