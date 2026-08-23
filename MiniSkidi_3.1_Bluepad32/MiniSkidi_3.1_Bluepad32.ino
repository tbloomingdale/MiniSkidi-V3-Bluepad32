#include <Arduino.h>
#include <Bluepad32.h>
#include "Config.h"
#include <Preferences.h>

// ======================================================
// Functions located in other tabs
// ======================================================

void setupMotors();
void setupAuxLights();
void moveTank(int leftCommand, int rightCommand);
void controlArm(int armValue, int deadzone);
void stopArm();
void processController();
void setDriveModeLED();
const char* getDriveModeName();
void setupBucketPWM();
void setupClawPWM();
void controlBucket(int bucketValue, int deadzone);
void controlClaw(bool openClaw, bool closeClaw);
void loadBucketPosition();
void updateBucketPositionMemory();
void loadClawPosition();
void updateClawPositionMemory();
void startBucketScoopPreset();
void startBucketDumpPreset();
void updateBucketPreset();
void cancelBucketPreset();
Preferences preferences;


// ======================================================
// Controller State
// ======================================================

ControllerPtr myController = nullptr;

bool controllerArmed = false;

unsigned long controllerConnectedAt = 0;
unsigned long controllerNeutralSince = 0;


// ======================================================
// Controller Connected
// ======================================================

void onConnectedController(ControllerPtr ctl)
{
  myController = ctl;

  controllerArmed = false;
  controllerConnectedAt = millis();
  controllerNeutralSince = 0;

  setDriveModeLED();

  ctl->playDualRumble(
    0,      // start immediately
    600,    // duration: 600 ms
    180,    // weak motor
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


// ======================================================
// Controller Disconnected
// ======================================================

void onDisconnectedController(ControllerPtr ctl)
{
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


// ======================================================
// Setup
// ======================================================

void setup()
{
  Serial.begin(115200);

  setupMotors();
setupAuxLights();

setupBucketPWM();
setupClawPWM();

  // Load remembered attachment positions.
  // These functions update the software position only.
  // They do NOT move the servos at startup.
  loadBucketPosition();
  loadClawPosition();

  BP32.setup(
    &onConnectedController,
    &onDisconnectedController
  );

  Serial.println("Waiting for controller...");
}


// ======================================================
// Main Loop
// ======================================================

void loop()
{
  BP32.update();

  if (myController && myController->isConnected()) {

    // --------------------------------------------------
    // Neutral-stick safety interlock
    // --------------------------------------------------

    if (!controllerArmed) {
      moveTank(0, 0);
      stopArm();

      // Ignore initial controller reports after connection.
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

        // Require centered controls continuously
        // before enabling operation.
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


    // --------------------------------------------------
    // Normal controller operation
    // --------------------------------------------------

    processController();

    // Save attachment positions after movement stops.
    updateBucketPositionMemory();
    updateClawPositionMemory();
  }
}