#pragma once
#include <stdint.h>
#include "averager.h"
#include "common.h"

using namespace Common;

class FanController
{
public:
    FanController();
    ~FanController();

    void update();

    float currentCps() const;
    float averageCps() const;
    float targetCps() const;
    int pwmValue() const;

    void setTargetCps(const float rpm);

    void setPwm(const uint16_t pwmValue);
    void setPwmPercent(const uint16_t percent);

private:
    void init();

    bool m_isManualMode;
    float m_currentCps;
    float m_targetCps;

    int m_pwmValue;
    
    Averager* m_averager;
};