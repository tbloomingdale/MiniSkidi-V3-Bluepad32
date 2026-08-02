\# MiniSkidi V3 Bluepad32



A modernized version of the ProfessorBoots MiniSkidi V3 project using an ESP32 and Bluepad32 for PlayStation 4 controller support.



\## Project Goals



\- Remove the original PS3 controller dependency.

\- Replace it with Bluepad32.

\- Improve the code structure.

\- Add smooth PWM motor control.

\- Prepare for bucket, claw, lights, and future features.



\## Hardware



\- ESP32 DOIT DEVKIT V1

\- Bluepad32

\- DRV8833 Motor Drivers

\- PlayStation 4 Controller

\- MiniSkidi V3 Chassis



\## Current Features



\- PS4 controller support

\- Bluetooth connection

\- Variable-speed PWM drive

\- Left stick controls driving

\- Right stick controls arm

\- Automatic motor stop on controller disconnect

\- Modular code structure



\## Project Structure



```

Config.h

Controller.ino

Drive.ino

Servo.ino

MiniSkidi\_3.1\_Bluepad32.ino

```



\## Roadmap



\- Improve low-speed drive tuning

\- Bucket servo

\- Claw servo

\- Lights

\- Additional controller features



\## Credits



Original MiniSkidi V3 by ProfessorBoots.



Bluepad32 by Ricardo Quesada.



This repository contains my personal modernization and controller conversion.

