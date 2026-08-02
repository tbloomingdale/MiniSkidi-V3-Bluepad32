\# MiniSkidi V3 Bluepad32



\# Drive Engine 2.0 Design Specification



\---



\## Mission Statement



> \*\*Operate like a real skid steer, not an RC toy.\*\*



Every firmware decision should improve the operator experience rather than simply making the robot move.



\---



\# Primary Goal



A first-time operator should be able to pick up the controller and comfortably drive the MiniSkidi within one minute.



\---



\# Drive Philosophy



The MiniSkidi should behave like a real compact skid steer loader.



Normal steering should produce smooth arcs.



Small steering inputs should not stop the machine.



The operator should never need to stop before making a turn.



Pivot turns should occur only when intentionally commanded.



Forward and reverse should behave consistently.



\---



\# Steering Requirements



\- No joystick dead spots

\- Smooth arc steering

\- Outside track maintains travel speed

\- Inside track slows progressively

\- Pivot begins at approximately 75% steering input

\- Reverse behaves the same as forward

\- No motor stall region during steering



\---



\# Loader Philosophy



The loader should behave like a real machine.



The operator should be able to:



\- Drive

\- Lift the arm

\- Tilt the bucket

\- Operate the claw



all at the same time.



No subsystem should block another.



\---



\# Controller Layout



Left Stick



\- Drive

\- Steering



Right Stick



\- Arm Up

\- Arm Down



L2 / R2 (Planned)



\- Bucket Tilt



L1 / R1 (Planned)



\- Claw Open / Close



OPTIONS (Planned)



\- Beginner Mode

\- Normal Mode



\---



\# Software Architecture



Controller



↓



Read Controller Inputs



↓



Drive Engine



↓



Loader Engine



↓



Motor / Servo Outputs



Each subsystem operates independently.



\---



\# Design Rules



1\. Operator experience comes first.



2\. Smooth operation is more important than mathematical perfection.



3\. Hardware should never be commanded into known stall regions.



4\. The code should remain modular and easy to maintain.



5\. Every feature should support the mission statement.



\---



\# Acceptance Tests



Drive Test



✓ Straight forward



✓ Straight reverse



✓ Smooth left arc



✓ Smooth right arc



✓ Pivot left



✓ Pivot right



✓ Reverse steering



✓ No dead spots



Operator Test



A new operator should be able to drive confidently within one minute.



Loader Test



The operator should eventually be able to:



Drive



\+



Raise Arm



\+



Tilt Bucket



\+



Operate Claw



simultaneously.



\---



\# Long-Term Vision



Create the most realistic open-source MiniSkidi firmware possible while remaining simple, reliable, and enjoyable to operate.



The goal is to make the MiniSkidi feel like a miniature compact skid steer rather than a tracked RC vehicle.

