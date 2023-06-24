#pragma once
#include <Arduino.h>
#include <stdint.h>
#include <AccelStepper.h>

class HatchController
{
public:
    enum State
    {
        READY,
        MOVING,
        MANUAL_MOVING,
        CALIBRATE_OPENING,
        CALIBRATE_CLOSING,
        INIT,
        ERROR
    };

    HatchController();
    ~HatchController();

    void update();
    void requestCalibration();
    void requestOpenHatch(const uint16_t percent);
    void manualMove(const long steps, const float speed);
    void setMaxSpeed(const float speed);
    void setAcceleration(const float acc);
    void setSpeed(const float speed);

    State getState() const;

private:
    void init();

    bool m_isCalibrated;
    long m_openedPosition;
    long m_closedPosition;
    long m_moveTo;
    long m_manualPosition;

    float m_speed;
    float m_maxSpeed;
    float m_acceleration;

    State m_state;
    AccelStepper* m_motor;
};