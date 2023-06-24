#include <Arduino.h>
#include <ArduinoJson.h>

#define USE_TIMER_3 true
#define USE_TIMER_5 true
#include <TimerInterrupt.h>

#include "common.h"
#include "sensors.h"
#include "fan_controller.h"
#include "hatch_controller.h"
#include "led_controller.h"
#include "uart_controller.h"

using namespace Common;

auto &fanTimer = ITimer3;
auto &hatchTimer = ITimer5;

Sensors sensors;
FanController fanController;
HatchController hatchController;
LedController ledController;
UartController uartController(&sensors, &ledController, &fanController, &hatchController);

// ------------------------  INTERRUPT HANDLERS ------------------------

void onFanTimerTimeout()
{
    fanController.update();
}

void onHatchTimerTimeout()
{
    //hatchController.update();
}

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

void initTimers()
{
    fanTimer.init();
    if (!fanTimer.attachInterruptInterval(FAN_TIMER_INTERVAL_MS, onFanTimerTimeout))
    {
        Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
    }

    // hatchTimer.init();
    // if (!hatchTimer.setFrequency(4500, onHatchTimerTimeout))
    // {
    //     Serial.println(F("Can't set ITimer3. Select another freq. or timer"));
    // }
}

void setup()
{
    Serial.begin(115200);
    Serial.flush();
    Serial.setTimeout(10);

    initTimers();

    pinMode(STATUS_R_PIN, OUTPUT);
    pinMode(STATUS_G_PIN, OUTPUT);
    pinMode(STATUS_B_PIN, OUTPUT);
    setStatus(READY);

    ledController.setUseShadeMode(true);
    ledController.setLightColor(255, 2, 2);
    fanController.setTargetCps(0);
    hatchController.requestCalibration();
    //hatchController.manualMove(800, 500);

    pinMode(DEBUG_PIN_3, INPUT);
    digitalWrite(DEBUG_PIN_3, HIGH);

    TCCR4B = TCCR4B & B11111000 | B00000001;
}

static uint32_t profiler1 = 0;
uint32_t res = 0;

void loop()
{
    static uint32_t softTimerCounter;
    if (millis() - softTimerCounter >= SENSORS_UPDATE_INTERVAL_MS)
    {
        softTimerCounter = millis();
        sensors.update();
        res = millis() - softTimerCounter;
        //Serial.print("----- Profile:");
        //Serial.println(res);
    }

    //profiler1 = millis();
    hatchController.update();

    uartController.update();

}
