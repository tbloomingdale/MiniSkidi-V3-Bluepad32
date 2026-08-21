# MiniSkidi V3 Bluepad32

## Controller Rumble and Safety

---

## Overview

The PS4 controller provides rumble feedback for controller connection,
drive mode selection, and controller arming.

A neutral-stick safety interlock prevents MiniSkidi from moving
unexpectedly when the controller connects or reconnects.

---

## Controller Connection Rumble

When the PS4 controller connects, it provides a strong confirmation
rumble.

Connection rumble settings:

- Start Delay: 0 ms
- Duration: 600 ms
- Weak Motor: 180
- Strong Motor: 255

This confirms that the controller has successfully connected to
MiniSkidi.

---

## Drive Mode Rumble

Each drive mode has a unique rumble pattern.

- WORK: 1 pulse
- DRIVE: 2 pulses
- TRAVEL: 3 pulses

The rumble pattern allows the operator to identify the selected
drive mode without looking at the controller light bar.

The controller light bar continues to provide visual drive mode
feedback.

---

## Neutral-Stick Safety Interlock

Whenever the PS4 controller connects or reconnects, MiniSkidi starts
in a disarmed state.

Drive and arm controls remain disabled until the required controller
sticks return to neutral.

The safety interlock checks:

- Left stick X axis
- Left stick Y axis
- Right stick Y axis used for arm control

MiniSkidi waits 700 ms after controller connection before evaluating
the stick positions.

After the settling period, the required controls must remain inside
the configured dead zone continuously for 300 ms.

Only then is the controller armed.

A short rumble confirms that the controller has armed and normal
operation is enabled.

---

## Controller Disconnect Safety

If the controller disconnects:

- Left track stops
- Right track stops
- Arm motor stops
- Controller armed state is cleared

When the controller reconnects, it must pass the neutral-stick safety
interlock again before movement is allowed.

---

## Hardware Test Results

The controller safety system was tested on the MiniSkidi V3 hardware
with a PlayStation 4 controller.

The following tests passed:

- Normal connection with sticks centered
- Left drive stick held during connection
- Right arm stick held during connection
- Controller disconnect while armed
- Reconnection with left drive stick held
- Reconnection with right arm stick held
- Drive remains locked until the stick returns to neutral
- Arm remains locked until the stick returns to neutral
- Arming confirmation rumble operates correctly
- WORK mode rumble operates correctly
- DRIVE mode rumble operates correctly
- TRAVEL mode rumble operates correctly

---

## Current Status

Controller rumble feedback: WORKING

Drive mode rumble feedback: WORKING

Controller disconnect stop: WORKING

Neutral-stick connection interlock: WORKING

Neutral-stick reconnection interlock: WORKING

Hardware testing: PASSED

---

## Development Branch

feature/controller-rumble