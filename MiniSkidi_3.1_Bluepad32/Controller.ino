void processController()
{
    int drive = -myController->axisY();
    int turn  = myController->axisX();

    if (abs(drive) < DEADZONE)
        drive = 0;

    if (abs(turn) < DEADZONE)
        turn = 0;

    int leftMix  = drive + turn;
    int rightMix = drive - turn;

    int leftCommand = 0;
    int rightCommand = 0;

    if (leftMix > DEADZONE)
        leftCommand = 1;
    else if (leftMix < -DEADZONE)
        leftCommand = -1;

    if (rightMix > DEADZONE)
        rightCommand = 1;
    else if (rightMix < -DEADZONE)
        rightCommand = -1;

    moveTank(leftCommand, rightCommand);

    int arm = myController->axisRY();

    if (abs(arm) < DEADZONE)
        arm = 0;

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