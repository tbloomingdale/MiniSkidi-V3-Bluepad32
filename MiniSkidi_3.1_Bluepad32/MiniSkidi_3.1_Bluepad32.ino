#include <Arduino.h>
#include <Bluepad32.h>
#include "Config.h"

// Functions located in other tabs
void setupMotors();
void moveTank(int leftCommand, int rightCommand);
void controlArm(int armValue, int deadzone);
void stopArm();
void processController();
void setupServos();
void testBucketServo();
void setDriveModeLED();
const char* getDriveModeName();

ControllerPtr myController = nullptr;
bool controllerArmed = false;
unsigned long controllerConnectedAt = 0;
unsigned long controllerNeutralSince = 0;

void onConnectedController(ControllerPtr ctl) {
myController = ctl;
controllerArmed = false;
controllerConnectedAt = millis();
controllerNeutralSince = 0;
setDriveModeLED();

ctl->playDualRumble(
  0,      // start immediately
  600,    // duration: 600 ms
  180,     // weak motor
  255     // strong motor
);

Serial.printf(
    "Default Drive Mode: %s\n",
    getDriveModeName()
);

  Serial.println();
  Serial.println("************************");
  Serial.println("PS4 Controller Connected!");
  Serial.println("************************");
}

void onDisconnectedController(ControllerPtr ctl) {
  moveTank(0, 0);
  stopArm();

  if (myController == ctl) {
  myController = nullptr;
  controllerArmed = false;
  controllerConnectedAt = 0;
  controllerNeutralSince = 0;
}

  Serial.println("Controller Disconnected");
}

void setup() {
  Serial.begin(115200);

  setupMotors();
  // setupServos();

  BP32.setup(
    &onConnectedController,
    &onDisconnectedController
  );

  Serial.println("Waiting for controller...");
}

void loop() {
  BP32.update();

  if (myController && myController->isConnected()) {

    if (!controllerArmed) {
      moveTank(0, 0);
      stopArm();

      // Ignore the first controller reports after connection.
      // Bluepad32 may initially report centered axes before
      // the actual stick positions arrive.
      if (millis() - controllerConnectedAt < 700) {
        controllerNeutralSince = 0;
        delay(20);
        return;
      }

      bool driveStickCentered =
        abs(myController->axisX()) < DEADZONE &&
        abs(myController->axisY()) < DEADZONE;

      bool armStickCentered =
        abs(myController->axisRY()) < DEADZONE;

      if (driveStickCentered && armStickCentered) {

        if (controllerNeutralSince == 0) {
          controllerNeutralSince = millis();
        }

        // Require both controls to remain centered continuously
        // before enabling the machine.
        if (millis() - controllerNeutralSince >= 300) {
          controllerArmed = true;

          Serial.println(
            "Controller armed - sticks centered."
          );

          myController->playDualRumble(
            0,
            250,
            100,
            180
          );
        }
      }
      else {
        // Any stick movement restarts the neutral timer.
        controllerNeutralSince = 0;
      }

      delay(20);
      return;
    }

    processController();
  }
}