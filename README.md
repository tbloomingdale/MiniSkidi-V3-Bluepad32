# MiniSkidi V3 Bluepad32

PS4 controller conversion and control-system update for the ProfessorBoots MiniSkidi V3 RC skid steer.

This project replaces the original PS3 controller implementation with **Bluepad32** and a **PlayStation 4 controller**, while retaining the original MiniSkidi hardware and expanding the control system with proportional drive, multiple drive modes, safety features, bucket and claw position control, automatic bucket presets, and auxiliary lighting.

## Current Stable Version

**v4.4-stable**

This version has completed a full hardware shakedown on the MiniSkidi and is the current known-good recovery point.

## Features

- Bluepad32 PS4 controller support
- Proportional skid-steer drive
- Drive Engine 4 steering
- Work, Drive, and Travel modes
- Proportional loader arm control
- Proportional hydraulic-style bucket control
- Automatic scoop / level bucket preset
- Automatic dump bucket preset
- Manual bucket override of automatic presets
- Bucket position memory across power cycles
- Smooth R2/L2 claw control
- Claw position memory across power cycles
- Auxiliary light control
- Controller connection rumble
- Drive-mode rumble feedback
- PS4 light-bar drive-mode indication
- Neutral-stick startup safety interlock
- Automatic drive and arm stop on controller disconnect

## PS4 Controller Mapping

| Control | Function |
|---|---|
| Left Stick Up / Down | Drive forward / reverse |
| Left Stick Left / Right | Steering / pivot |
| Right Stick Up / Down | Loader arm up / down |
| Right Stick Left / Right | Manual proportional bucket curl / dump |
| R1 | Bucket scoop / level preset |
| L1 | Bucket dump preset |
| R2 | Open claw |
| L2 | Close claw |
| R3 | Toggle auxiliary lights |
| D-pad Up | Travel mode |
| D-pad Right | Drive mode |
| D-pad Down | Work mode |

Currently unused controls are available for future features.

## Drive Modes

### Work Mode

Precision-oriented operation with reduced maximum drive output and smoother response.

**Select:** D-pad Down

### Drive Mode

Normal operating mode and the default mode when the controller connects.

**Select:** D-pad Right

### Travel Mode

More aggressive throttle response for moving the machine between work areas.

**Select:** D-pad Up

## PS4 Light-Bar Indication

The PS4 controller light bar indicates the selected drive mode:

| Drive Mode | Light-Bar Color |
|---|---|
| Work | Blue |
| Drive | Green |
| Travel | Red |

Drive-mode changes also provide rumble feedback:

- Work: 1 pulse
- Drive: 2 pulses
- Travel: 3 pulses

## Safety Features

### Neutral-Stick Startup Interlock

After the PS4 controller connects, MiniSkidi will not enable normal operation until the drive and arm controls have remained centered.

This helps prevent unexpected movement if a stick is displaced during controller connection.

### Controller Disconnect

If the controller disconnects, the drive tracks and loader arm are commanded to stop.

## Bucket Control

The bucket can be operated manually with the **right stick left/right axis** or automatically using the R1/L1 bucket presets.

### Manual Bucket Control

The right stick provides proportional hydraulic-style control:

- Small stick movement = slow bucket movement
- Large stick movement = faster bucket movement
- Centered stick = hold current position

Bucket movement uses a fixed 20 ms update interval so movement speed remains consistent regardless of ESP32 main-loop speed.

Manual right-stick bucket input immediately cancels an active automatic preset and returns control to the operator.

### Bucket Presets

Two calibrated automatic bucket positions are available:

- **R1:** Scoop / level position - 2000 us
- **L1:** Dump position - 700 us

Tapping R1 or L1 smoothly moves the bucket to the selected position.

Preset movement uses a fixed 20 ms update interval and a tested 15 us movement step for smooth operation.

## Bucket Position Memory

The bucket position is retained in ESP32 nonvolatile memory.

After bucket movement stops, the current position is saved. This allows the firmware to retain the calibrated bucket position across power cycles.

## Claw Control

The claw is controlled with the PS4 analog triggers:

- **R2:** Open claw
- **L2:** Close claw

Holding a trigger moves the claw. Releasing the trigger holds the current position.

If R2 and L2 are pressed together, the claw does not move.

Claw movement uses a fixed 20 ms update interval so movement speed remains consistent regardless of ESP32 main-loop speed.

## Claw Position Memory

The claw position is retained in ESP32 nonvolatile memory.

After claw movement stops, the current position is saved so the firmware can retain the calibrated claw position across power cycles.

## Auxiliary Lights

Press **R3** (right-stick click) to toggle the MiniSkidi auxiliary lights on or off.

The lights default to OFF when the ESP32 starts.

## Hardware

Controller:

- Sony PlayStation 4 / DualShock 4
- Bluepad32 Bluetooth controller library

Microcontroller:

- ESP32
- Tested board selection: **DOIT ESP32 DEVKIT V1**

### Pin Assignment

| Function | ESP32 GPIO |
|---|---:|
| Right Motor IN1 | 25 |
| Right Motor IN2 | 26 |
| Left Motor IN1 | 33 |
| Left Motor IN2 | 32 |
| Arm Motor IN1 | 21 |
| Arm Motor IN2 | 19 |
| Bucket Servo | 23 |
| Claw Servo | 22 |
| Auxiliary Lights IN1 | 18 |
| Auxiliary Lights IN2 | 5 |

## Development Environment

Current tested environment:

- Arduino IDE 2.x
- ESP32 board package 2.0.17
- Bluepad32 by Ricardo Quesada
- Board: DOIT ESP32 DEVKIT V1

The Bluepad32 conversion intentionally does not use the original PS3 controller library.

## Project Structure

The active Bluepad32 firmware is located in:

`MiniSkidi_3.1_Bluepad32/`

Important files include:

- `MiniSkidi_3.1_Bluepad32.ino` - main setup, controller connection, and safety logic
- `Config.h` - hardware pins and tuning constants
- `Controller.ino` - PS4 mapping, drive modes, and drive mixer
- `Drive.ino` - track and loader arm motor output
- `Servo.ino` - bucket and claw control, automatic bucket presets, servo timing, and position memory

Additional project documentation is available in the `Docs/` directory.

## Stable Development Policy

The `main` branch is intended to contain tested MiniSkidi firmware.

Known-good development checkpoints are preserved with Git tags.

Current full-machine stable checkpoint:

`v4.4-stable`

New features should be developed on separate feature branches and tested on the machine before being merged into `main`.

## Original MiniSkidi V3 Project

This project is based on the ProfessorBoots MiniSkidi V3.

Original project resources:

- Video: [MiniSkidi V3 YouTube video](https://www.youtube.com/watch?v=dDfVdGGrf1k)
- Kits: [ProfessorBoots RC Skidsteer Kit V3.0](https://professorboots.com/products/rc-skidsteer-kit-v3-0)
- Tutorial: [ProfessorBoots members tutorial](https://professorboots.com/a/members)  
  A membership may be required to access the course when self-sourcing parts.
- 3D files: [3D Printed RC SkidSteer V3.0 on Printables](https://www.printables.com/de/model/721244-3d-printed-rc-skidsteer-v30)

## Project Goal

The goal of this fork is to retain the character and hardware of the original MiniSkidi while providing a modern PS4/Bluepad32 control system that behaves more like a small skid steer than a traditional RC vehicle.