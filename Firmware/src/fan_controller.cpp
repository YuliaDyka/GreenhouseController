#include "fan_controller.h"

namespace {

volatile uint16_t cpsCounter = 0;

void onCpsPinInterrupt()
{
    cpsCounter++;
}

}

FanController::FanController()
    : m_isManualMode(false)
    , m_currentCps(0)
    , m_targetCps(0)
    , m_pwmValue(0)
    , m_averager(nullptr)
{
    init();
}

FanController::~FanController()
{
    if (m_averager != nullptr) {
        delete m_averager;
        m_averager = nullptr;
    }
}

void FanController::update()
{
    if (m_targetCps <= 0 || m_isManualMode) {
        return;
    }

    m_currentCps = cpsCounter * 4.0;
    m_averager->push(cpsCounter * 4.0);
    cpsCounter = 0;

    const auto avgValue = m_averager->avgValue();

    const auto diff = abs(m_targetCps - avgValue);
    const auto sign = m_targetCps > m_currentCps ? 1 : -1;
    if (diff > 3) {
        if (diff > 100.0) {
            m_pwmValue += 6 * sign;
        } else if (diff > 75.0) {
            m_pwmValue += 5 * sign;
        } else if (diff > 50.0) {
            m_pwmValue += 4 * sign;
        } else if (diff > 25.0) {
            m_pwmValue += 3 * sign;
        } else if (diff > 10.0) {
            m_pwmValue += 2 * sign;
        } else {
            m_pwmValue += sign;
        }

        m_pwmValue = m_pwmValue < 0 ? 0 : m_pwmValue > 255 ? 255 : m_pwmValue;
        analogWrite(FAN_PWM_PIN, m_pwmValue);
    }
    
    // Serial.print("-- Current cps is: ");
    // Serial.println(m_currentCps);
    // Serial.print("-- AVG cps is:     ");
    // Serial.println(avgValue);
    // Serial.print("-- Target cps is:  ");
    // Serial.println(m_targetCps);
    // Serial.print("-- Diff:           ");
    // Serial.println(diff * sign);
    // Serial.print("-- Avg deviation:  ");
    // Serial.println(m_averager->deviation());
    // Serial.print("-- Current PWM is: ");
    // Serial.println(m_pwmValue);
    // Serial.println("--------------");
}

float FanController::currentCps() const
{
    return m_currentCps;
}

float FanController::averageCps() const
{
    return m_averager->avgValue();
}

float FanController::targetCps() const
{
    return m_targetCps;
}

int FanController::pwmValue() const
{
    return m_pwmValue;
}

void FanController::setTargetCps(const float cps)
{
    m_isManualMode = false;   
    if (cps < 5) {
        m_targetCps = 0;
        m_pwmValue = 0;
        analogWrite(FAN_PWM_PIN, m_pwmValue);
        m_averager->clear();
        return;
    } else if (cps > 270) {
        m_targetCps = 270;
    } else {
        m_targetCps = cps;
    }

    if (abs(m_targetCps - m_currentCps) > 40) {
        m_pwmValue = m_targetCps == 0 ? 0 : (m_targetCps - 20) / 3.0 + 40;
        analogWrite(FAN_PWM_PIN, m_pwmValue);
    }

}

void FanController::setPwm(const uint16_t pwmValue)
{
    m_isManualMode = true;
    m_pwmValue = pwmValue < 0 ? 0 : pwmValue > 255 ? 255 : pwmValue;
    analogWrite(FAN_PWM_PIN, m_pwmValue);
}

void FanController::setPwmPercent(const uint16_t percent)
{
    const auto restrictedValue = percent > 100 ? 100 : percent;
    setPwm(restrictedValue * 255 / 100);
}

void FanController::init()
{
    pinMode(FAN_PWM_PIN, OUTPUT);
    pinMode(FAN_SPEED_PIN, INPUT);
    
    attachInterrupt(digitalPinToInterrupt(FAN_SPEED_PIN), onCpsPinInterrupt, CHANGE);

    m_averager = new Averager(20);
}
