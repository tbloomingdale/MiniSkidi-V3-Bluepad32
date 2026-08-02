#define RIGHT_MOTOR 1
#define LEFT_MOTOR 0
#define ARM_MOTOR 2

#define FORWARD 1
#define BACKWARD -1
#define STOP 0

struct MOTOR_PINS {
    int pinIN1;
    int pinIN2;
};

MOTOR_PINS motorPins[] = {
    {25,26},   // Right motor
    {33,32},   // Left motor
    {21,19}    // Arm motor
};

const int NUM_MOTORS = sizeof(motorPins) / sizeof(motorPins[0]);

void rotateMotor(int motorNumber, int motorDirection) {

    if (motorDirection == FORWARD) {
        digitalWrite(motorPins[motorNumber].pinIN1, HIGH);
        digitalWrite(motorPins[motorNumber].pinIN2, LOW);
    }
    else if (motorDirection == BACKWARD) {
        digitalWrite(motorPins[motorNumber].pinIN1, LOW);
        digitalWrite(motorPins[motorNumber].pinIN2, HIGH);
    }
    else {
        digitalWrite(motorPins[motorNumber].pinIN1, LOW);
        digitalWrite(motorPins[motorNumber].pinIN2, LOW);
    }
}

void moveTank(int left, int right) {

    // LEFT TRACK
    if (left > 0)
        rotateMotor(LEFT_MOTOR, FORWARD);
    else if (left < 0)
        rotateMotor(LEFT_MOTOR, BACKWARD);
    else
        rotateMotor(LEFT_MOTOR, STOP);

    // RIGHT TRACK
    if (right > 0)
        rotateMotor(RIGHT_MOTOR, FORWARD);
    else if (right < 0)
        rotateMotor(RIGHT_MOTOR, BACKWARD);
    else
        rotateMotor(RIGHT_MOTOR, STOP);
}

void setupMotors() {

    for (int i = 0; i < NUM_MOTORS; i++) {

        pinMode(motorPins[i].pinIN1, OUTPUT);
        pinMode(motorPins[i].pinIN2, OUTPUT);
    }

    moveTank(0,0);
}