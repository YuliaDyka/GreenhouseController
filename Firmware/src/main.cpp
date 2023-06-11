#include <Arduino.h>
#include <AccelStepper.h>

enum Status
{
  READY,
  HATCH_MOVING,
  HATCH_MOVE_DENY
};

constexpr auto STATUS_R_PIN   = 22;
constexpr auto STATUS_G_PIN   = 23;
constexpr auto STATUS_B_PIN   = 24;

constexpr auto FAN_PWM_PIN    = 6;
constexpr auto FAN_SPEED_PIN  = 33;

constexpr auto LED_R_PWM_PIN  = 7;
constexpr auto LED_G_PWM_PIN  = 8;
constexpr auto LED_B_PWM_PIN  = 9;

constexpr auto H_MOTOR_PIN1   = 25;
constexpr auto H_MOTOR_PIN2   = 26;
constexpr auto H_MOTOR_PIN3   = 27;
constexpr auto H_MOTOR_PIN4   = 28;

constexpr auto H_OPEN_PIN     = 35;
constexpr auto H_CLOSE_PIN    = 34;

constexpr auto DEBUG_PIN_1    = A0;
constexpr auto DEBUG_PIN_2    = A1;

constexpr auto STEPS          = 100;

AccelStepper motor(AccelStepper::FULL4WIRE, H_MOTOR_PIN1, H_MOTOR_PIN3, H_MOTOR_PIN2, H_MOTOR_PIN4);

void setStatus(Status status) 
{
  switch (status)
  {
  case READY:
    digitalWrite(STATUS_R_PIN, LOW);
    digitalWrite(STATUS_B_PIN, LOW);
    digitalWrite(STATUS_G_PIN, HIGH);
    break;
  case HATCH_MOVING:
    digitalWrite(STATUS_R_PIN, LOW);
    digitalWrite(STATUS_G_PIN, LOW);
    digitalWrite(STATUS_B_PIN, HIGH);
    break;
  case HATCH_MOVE_DENY:
    digitalWrite(STATUS_G_PIN, LOW);
    digitalWrite(STATUS_B_PIN, LOW);
    digitalWrite(STATUS_R_PIN, HIGH);
    break;   
  default:
    break;
  }
}

void setup() 
{
  pinMode(STATUS_R_PIN, OUTPUT);
  pinMode(STATUS_G_PIN, OUTPUT);
  pinMode(STATUS_B_PIN, OUTPUT);

  pinMode(FAN_PWM_PIN, OUTPUT);
  pinMode(FAN_SPEED_PIN, INPUT);

  pinMode(LED_R_PWM_PIN, OUTPUT);
  pinMode(LED_G_PWM_PIN, OUTPUT);
  pinMode(LED_B_PWM_PIN, OUTPUT);

  pinMode(H_MOTOR_PIN1, OUTPUT);
  pinMode(H_MOTOR_PIN2, OUTPUT);
  pinMode(H_MOTOR_PIN3, OUTPUT);
  pinMode(H_MOTOR_PIN4, OUTPUT);

  pinMode(H_OPEN_PIN, INPUT);
  pinMode(H_CLOSE_PIN, INPUT);

  digitalWrite(FAN_PWM_PIN, LOW);
  digitalWrite(LED_R_PWM_PIN, LOW);
  digitalWrite(LED_G_PWM_PIN, LOW);
  digitalWrite(LED_B_PWM_PIN, LOW);

  motor.setMaxSpeed(1000);
  
  setStatus(READY);
}

void loop() 
{
  const auto currPos = motor.currentPosition();
  int moveSpeed = 0;
  int nextPosition = currPos;

  const auto speedValue = analogRead(DEBUG_PIN_1);
  const auto pwmDebugValue = analogRead(DEBUG_PIN_2);
  const bool isClosed = digitalRead(H_CLOSE_PIN) == 0;
  const bool isOpened = digitalRead(H_OPEN_PIN) == 0;

  if (speedValue < 480) { // Opening
    if (isOpened) {
      setStatus(HATCH_MOVE_DENY);
    } else {
      setStatus(HATCH_MOVING);
      moveSpeed = 480 - speedValue;
      nextPosition = currPos - STEPS;
    }
  } else if (speedValue > 544) { // Closing
    if (isClosed) {
      setStatus(HATCH_MOVE_DENY);
    } else {
      setStatus(HATCH_MOVING);
      moveSpeed = speedValue - 544;
      nextPosition = currPos + STEPS;
    }
  } else {
    setStatus(READY);
  }

  if (moveSpeed != 0) {
    motor.enableOutputs();
    motor.moveTo(nextPosition);
    motor.setSpeed(moveSpeed * 1.3);
    motor.runSpeedToPosition();
  } else {
    motor.stop();
    motor.disableOutputs();
  }

  if (pwmDebugValue < 480) {
    analogWrite(FAN_PWM_PIN, (480 - pwmDebugValue) / 2);
  } else if (pwmDebugValue > 544) {
    analogWrite(LED_G_PWM_PIN, (pwmDebugValue - 544) / 2);
  } else {
    analogWrite(FAN_PWM_PIN, 0);
    analogWrite(LED_G_PWM_PIN, 0);
  }

  delay(1);

}
