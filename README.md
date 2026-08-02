# MiniSkidi V3 Bluepad32

> ## **Operate like a real skid steer, not an RC toy.**

MiniSkidi V3 Bluepad32 is a modernization of the original ProfessorBoots MiniSkidi V3 project.

The project replaces the original PlayStation 3 controller with Bluepad32 and a PlayStation 4 controller while focusing on creating a realistic, intuitive operating experience.

Rather than simply making the robot move, the goal is to make it behave like a compact skid steer loader—with smooth steering, simultaneous machine functions, and controls that are easy for first-time operators to learn.

---

# Project Status

**Current Version:** 4.x Development

## ✅ Working

- PlayStation 4 Controller (Bluepad32)
- Bluetooth Connectivity
- ESP32 Firmware
- Variable-Speed PWM Drive
- Left Stick Drive Control
- Right Stick Arm Control
- Automatic Motor Stop on Controller Disconnect
- Modular Code Structure
- GitHub Version Control

## 🚧 Currently In Development

- New Skid-Steer Drive Algorithm
- Smooth Arc Steering
- Elimination of Joystick Dead Spots
- Bucket Servo Control
- Claw Servo Control
- Simultaneous Machine Operation

---

# Design Philosophy

Every design decision follows one simple principle:

> **Operate like a real skid steer, not an RC toy.**

This project is being developed to:

- Deliver smooth, predictable skid-steer driving.
- Allow driving while simultaneously operating the arm, bucket, and claw.
- Make the controls intuitive for first-time operators.
- Maintain clean, modular firmware that is easy to expand and maintain.
- Create an enjoyable operating experience rather than simply making the robot move.

---

# Project Goals

- Replace the original PS3 controller with Bluepad32.
- Support PlayStation 4 controllers.
- Improve the firmware architecture.
- Implement smooth PWM motor control.
- Develop realistic skid-steer driving characteristics.
- Support simultaneous loader functions.
- Continue expanding features while maintaining code readability.

---

# Hardware

- ESP32 DOIT DEVKIT V1
- Bluepad32
- DRV8833 Dual Motor Drivers
- PlayStation 4 Controller
- MiniSkidi V3 Chassis

---

# Current Features

- Bluetooth Controller Support
- PlayStation 4 Controller Pairing
- Variable-Speed PWM Drive
- Left Stick Drive Control
- Right Stick Arm Control
- Automatic Controller Disconnect Safety Stop
- Modular Multi-File Arduino Project
- GitHub Version Controlled Development

---

# Planned Controller Layout

| Control | Function |
|----------|----------|
| Left Stick | Drive & Steering |
| Right Stick (Up/Down) | Arm Lift |
| L2 / R2 | Bucket Tilt *(planned)* |
| L1 / R1 | Claw Open / Close *(planned)* |
| Options | Beginner / Normal Mode *(planned)* |

---

# Project Structure

```
MiniSkidi_3.1_Bluepad32
│
├── MiniSkidi_3.1_Bluepad32.ino
├── Config.h
├── Controller.ino
├── Drive.ino
├── Servo.ino
├── CHANGELOG.md
└── README.md
```

---

# Development Roadmap

## Version 4.2

- New skid-steer drive system
- Smooth arc steering
- Remove joystick dead spots
- Improve low-speed precision

## Version 4.3

- Bucket Servo
- Claw Servo
- Simultaneous Loader Operation

## Version 4.4

- Beginner Mode
- Lighting
- Controller Refinements

## Version 5.0

- Complete operator experience
- Full documentation
- Public release

---

# Long-Term Vision

The objective is to create a MiniSkidi that behaves like a real compact skid steer loader.

A first-time operator should be able to pick up the controller and begin driving naturally within minutes.

The firmware is being developed so that driving, lifting the arm, operating the bucket, and controlling the claw can all happen simultaneously—just as they would on a real machine.

---

# About This Project

This repository is a substantial modernization of the original MiniSkidi V3 project.

Major development work includes:

- Migration from the original PlayStation 3 controller to Bluepad32.
- PlayStation 4 controller support.
- ESP32 firmware modernization.
- Modular firmware architecture.
- PWM motor control.
- Ongoing development of a realistic skid-steer drive system.
- Documentation and project organization.

Project Maintainer

**Todd Bloomingdale**

---

# Credits

Original MiniSkidi V3

**ProfessorBoots**

Bluepad32 Library

**Ricardo Quesada**

This project builds upon their excellent work while modernizing the controller system and expanding the firmware for a more realistic skid-steer operating experience.

---

# Acknowledgements

Many ideas, testing iterations, firmware architecture discussions, and documentation improvements have been developed through extensive collaboration with OpenAI's ChatGPT.

The design direction and implementation decisions are based on real-world testing and the project goals established by the maintainer.

---

# License

Please respect the licenses of the original MiniSkidi V3 project and all third-party libraries used.

This repository contains original development, documentation, and firmware modernization built upon the original open-source project.