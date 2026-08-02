#include <Arduino.h>
#include <Bluepad32.h>

// Functions located in Drive.ino
void setupMotors();
void moveTank(int leftCommand, int rightCommand);
void controlArm(int armValue, int deadzone);
void stopArm();

ControllerPtr myController = nullptr;

void onConnectedController(ControllerPtr ctl) {
  myController = ctl;

  Serial.println();
  Serial.println("************************");
  Serial.println("PS4 Controller Connected!");
  Serial.println("************************");
}

void onDisconnectedController(ControllerPtr ctl) {
  // Immediately stop all DC motors if Bluetooth disconnects.
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

  BP32.setup(&onConnectedController, &onDisconnectedController);

  Serial.println("Waiting for controller...");
}

void loop() {
  BP32.update();

  if (myController && myController->isConnected()) {
    const int DEADZONE = 100;

    // Left stick controls the tracks.
    int drive = -myController->axisY();
    int turn  = -myController->axisX();

    if (abs(drive) < DEADZONE) {
      drive = 0;
    }

    if (abs(turn) < DEADZONE) {
      turn = 0;
    }

    int leftMix  = drive + turn;
    int rightMix = drive - turn;

    int leftCommand  = 0;
    int rightCommand = 0;

    if (leftMix > DEADZONE) {
      leftCommand = 1;
    } else if (leftMix < -DEADZONE) {
      leftCommand = -1;
    }

    if (rightMix > DEADZONE) {
      rightCommand = 1;
    } else if (rightMix < -DEADZONE) {
      rightCommand = -1;
    }

    moveTank(leftCommand, rightCommand);

    // Right stick up/down controls the arm.
    int arm = myController->axisRY();

    if (abs(arm) < DEADZONE) {
      arm = 0;
    }

    controlArm(arm, DEADZONE);

    Serial.printf(
      "Drive:%5d Turn:%5d Left:%2d Right:%2d Arm:%5d\n",
      drive,
      turn,
      leftCommand,
      rightCommand,
      arm
    );

    delay(20);
  }
}