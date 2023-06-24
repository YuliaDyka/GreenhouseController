#include "led_controller.h"
#include "common.h"

using namespace Common;

LedController::LedController()
    : m_ledsPWM{0, 0, 0}
    , m_lightColor{0, 0, 0}
    , m_brightness(0)
    , m_isUseShadeMode(false)
{
    init();
}

void LedController::setLedsPWM(uint8_t red, uint8_t green, uint8_t blue)
{
    m_ledsPWM[LedColor::RED] = red;
    m_ledsPWM[LedColor::GREEN] = green;
    m_ledsPWM[LedColor::BLUE] = blue;
    updateLedsPWM();
}

void LedController::setLedPWM(LedColor ledColor, uint8_t pwmValue)
{
    m_ledsPWM[ledColor] = pwmValue;
    updateLedsPWM();
}

void LedController::setLightColor(uint8_t red, uint8_t green, uint8_t blue)
{
    m_lightColor[LedColor::RED] = red;
    m_lightColor[LedColor::GREEN] = green;
    m_lightColor[LedColor::BLUE] = blue;
    applyLightColor();
}

void LedController::setLightR(uint8_t value)
{
    m_lightColor[LedColor::RED] = value;
}

void LedController::setLightG(uint8_t value)
{
    m_lightColor[LedColor::GREEN] = value;
}

void LedController::setLightB(uint8_t value)
{
    m_lightColor[LedColor::BLUE] = value;
}

void LedController::setWhiteLight()
{
    m_lightColor[LedColor::RED] = 255;
    m_lightColor[LedColor::GREEN] = 255;
    m_lightColor[LedColor::BLUE] = 255;
    applyLightColor();
}

void LedController::setLightBrightness(uint8_t brightness)
{
    m_brightness = brightness;
    applyLightColor();
}

void LedController::applyLightColor()
{
    if (m_isUseShadeMode) {
        const float brightness = m_brightness / 100.0;
        m_ledsPWM[LedColor::RED] = m_lightColor[LedColor::RED] * brightness;
        m_ledsPWM[LedColor::GREEN] = m_lightColor[LedColor::GREEN] * brightness;
        m_ledsPWM[LedColor::BLUE] = m_lightColor[LedColor::BLUE] * brightness;
        updateLedsPWM();
    }
}

void LedController::setUseShadeMode(bool isUseShadeMode)
{
    m_isUseShadeMode = isUseShadeMode;
    applyLightColor();
}

void LedController::init()
{
    pinMode(LED_R_PWM_PIN, OUTPUT);
    pinMode(LED_G_PWM_PIN, OUTPUT);
    pinMode(LED_B_PWM_PIN, OUTPUT);

    setWhiteLight();
}

void LedController::updateLedsPWM()
{
    analogWrite(LED_R_PWM_PIN, m_ledsPWM[LedColor::RED]);
    analogWrite(LED_G_PWM_PIN, m_ledsPWM[LedColor::GREEN]);
    analogWrite(LED_B_PWM_PIN, m_ledsPWM[LedColor::BLUE]);
}
