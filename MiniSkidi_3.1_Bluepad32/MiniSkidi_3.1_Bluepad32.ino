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

ControllerPtr myController = nullptr;

void onConnectedController(ControllerPtr ctl) {
  myController = ctl;

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
    processController();
  }
}