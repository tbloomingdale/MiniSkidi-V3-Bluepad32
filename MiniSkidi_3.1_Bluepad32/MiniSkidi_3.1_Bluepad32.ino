#include <Arduino.h>
#include <Bluepad32.h>

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

  if (myController == ctl)
    myController = nullptr;

  Serial.println("Controller Disconnected");
}

void setup() {

  Serial.begin(115200);

  setupMotors();        // <-- Add this line

  BP32.setup(&onConnectedController, &onDisconnectedController);

  Serial.println("Waiting for controller...");
}

void loop() {

  BP32.update();

  if (myController && myController->isConnected()) {

           int drive = -myController->axisY();  // Left stick up/down
    int turn  =  -myController->axisX();  // Left stick left/right

    const int DEADZONE = 100;

    if (abs(drive) < DEADZONE) {
      drive = 0;
    }

    if (abs(turn) < DEADZONE) {
      turn = 0;
    }

    int leftMix  = drive + turn;
    int rightMix = drive - turn;

    int leftCommand = 0;
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

    // Send both track commands together, once per loop.
    moveTank(leftCommand, rightCommand);

    Serial.printf(
      "Drive:%5d Turn:%5d Left:%2d Right:%2d\n",
      drive,
      turn,
      leftCommand,
      rightCommand
    );

    delay(20);
  }

  setupMotors();
}
  