#pragma once

#include <Arduino.h>
#include <ArduinoSTL.h>
#include <vector>

class LedController
{
public:
    enum LedColor
    {
        RED = 0,
        GREEN,
        BLUE
    };

    LedController();
    
    // --------------- Directly PWM control mode ------------------
    // Shade and brightness values will be ignored
    // 8 bit PWM values: 0 - 255
    void setLedsPWM(uint8_t red, uint8_t green, uint8_t blue);
    void setLedPWM(LedColor ledColor, uint8_t pwmValue);

    uint8_t ledPWM(LedColor ledColor) const { return m_ledsPWM[ledColor]; }
    const std::vector<uint8_t>& ledsPWM() const { return m_ledsPWM; };

    // -------------- Shade of light and brightness mode ------------------
    // Warning: stored pwm values will be overwritten 
    // Set and apply RGB color 8-bit
    void setLightColor(uint8_t red, uint8_t green, uint8_t blue);

    // Set and apply white light
    void setWhiteLight();

    // Set colors separately without apply(needs to call applyLightColor() to take effect)
    void setLightR(uint8_t value);
    void setLightG(uint8_t value);
    void setLightB(uint8_t value);

    // Set and apply brightness percentage: 0 - 100%
    void setLightBrightness(uint8_t brightness);

    uint8_t lightBrightness() const { return m_brightness; }
    const std::vector<uint8_t>& lightColor() const { return m_lightColor; }

    void setUseShadeMode(bool isUseShadeMode);

    void applyLightColor();

private:
    void init();
    void updateLedsPWM();

    std::vector<uint8_t> m_ledsPWM;
    std::vector<uint8_t> m_lightColor;

    uint8_t m_brightness;
    bool m_isUseShadeMode;
};

