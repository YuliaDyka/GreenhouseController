#include "hatch_controller.h"
#include "common.h"

using namespace Common;

HatchController::HatchController()
    : m_isCalibrated(false)
    , m_openedPosition(0)
    , m_closedPosition(0)
    , m_moveTo(0)
    , m_manualPosition(0)
    , m_speed(400)
    , m_maxSpeed(500)
    , m_acceleration(250)
    , m_state(State::INIT)
    , m_motor(nullptr)
{
    init();
}

HatchController::~HatchController()
{
    if (m_motor != nullptr) {
        delete m_motor;
        m_motor = nullptr;
    }
}

void HatchController::update()
{
    const bool isClosed = digitalRead(H_CLOSE_PIN) == 0;
    const bool isOpened = digitalRead(H_OPEN_PIN) == 0;

    switch (m_state)
    {
    case State::CALIBRATE_OPENING:
        if (!isOpened) {
            m_motor->moveTo(m_motor->currentPosition() - 1);
            m_motor->setSpeed(m_speed);
            m_motor->runSpeedToPosition();
        } else {
            m_openedPosition = m_motor->currentPosition();
            m_state = State::CALIBRATE_CLOSING;
            // Serial.print("--- Opened position: ");
            // Serial.println(m_openedPosition);
        }       
        break;
    case State::CALIBRATE_CLOSING:
        if (!isClosed) {
            m_motor->moveTo(m_motor->currentPosition() + 1);
            m_motor->setSpeed(m_speed);
            m_motor->runSpeedToPosition();
        } else {
            m_closedPosition = m_motor->currentPosition();
            m_state = State::READY;
            m_isCalibrated = true;
            // Serial.print("--- Closet position: ");
            // Serial.println(m_closedPosition);
        }              
        break;
    case State::READY:
        m_motor->stop();
        m_motor->disableOutputs();     
        break;
    case State::MOVING:
    {
        const auto isOpening = m_moveTo < m_motor->currentPosition();
        if ((isOpening && isOpened) || (!isOpening && isClosed)) {
            m_state = State::READY;
            Serial.println("---!!! ALARM !!!---");
            return;
        }

        if (m_motor->distanceToGo() != 0) {
            m_motor->run();
        } else {
            m_state = State::READY;
        }    
        break;
    }
    case State::MANUAL_MOVING:
    {
        const auto isOpening = m_manualPosition < m_motor->currentPosition();
        if ((isOpening && isOpened) || (!isOpening && isClosed)) {
            m_state = State::READY;
            Serial.println("---!!! ALARM !!!---");
            return;
        }

        if (m_motor->distanceToGo() != 0) {
            m_motor->moveTo(m_manualPosition);
            m_motor->setSpeed(m_speed);
            m_motor->runSpeedToPosition();
        } else {
            m_state = State::READY;
        }
        break;
    }
    case State::ERROR:
        m_motor->stop();
        m_motor->disableOutputs();
        break;    
    default:
        break;
    }

}

void HatchController::requestCalibration()
{
    m_speed = 600;
    m_isCalibrated = false;
    m_state = State::CALIBRATE_OPENING;
    m_motor->enableOutputs();
}

void HatchController::requestOpenHatch(const uint16_t percent)
{
    if (m_state == State::READY) {
        const auto validPercent = percent > 100 ? 100 : percent;
        const long fullDistance = m_openedPosition - m_closedPosition;
        m_moveTo = m_closedPosition + fullDistance * validPercent / 100.0;
        m_motor->enableOutputs();
        m_motor->setMaxSpeed(m_maxSpeed);
        m_motor->setAcceleration(m_acceleration);
        m_motor->moveTo(m_moveTo);
        
        m_state = State::MOVING;
        Serial.print("-- Move to: ");
        Serial.println(m_moveTo);
        Serial.print("-- Curr pos: ");
        Serial.println(m_motor->currentPosition());
    }
}

void HatchController::manualMove(const long steps, const float speed)
{
    m_manualPosition = m_motor->currentPosition() + steps;
    m_speed = speed;
    m_state = MANUAL_MOVING;
    m_motor->enableOutputs();
    m_motor->moveTo(m_manualPosition);
}

void HatchController::setMaxSpeed(const float speed)
{
    m_maxSpeed = speed;
    m_motor->setMaxSpeed(m_maxSpeed);
}

void HatchController::setAcceleration(const float acc)
{
    m_acceleration = acc;
    m_motor->setAcceleration(m_acceleration);
}

void HatchController::setSpeed(const float speed)
{
    m_speed = speed;
}

HatchController::State HatchController::getState() const
{
    return m_state;
}

void HatchController::init()
{
    pinMode(H_MOTOR_PIN1, OUTPUT);
    pinMode(H_MOTOR_PIN2, OUTPUT);
    pinMode(H_MOTOR_PIN3, OUTPUT);
    pinMode(H_MOTOR_PIN4, OUTPUT);

    pinMode(H_OPEN_PIN, INPUT);
    pinMode(H_CLOSE_PIN, INPUT);

    m_motor = new AccelStepper(AccelStepper::FULL4WIRE, H_MOTOR_PIN1, H_MOTOR_PIN3, H_MOTOR_PIN2, H_MOTOR_PIN4);
    m_motor->setMaxSpeed(1000);
    m_motor->disableOutputs();
}
