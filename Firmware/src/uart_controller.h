#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "common.h"

using namespace Common;

class LedController;
class FanController;
class HatchController;
class Sensors;

class UartController
{
public:
    UartController(Sensors* sensors,
                   LedController* ledController,
                   FanController* fanController,
                   HatchController* hatchController);

    void update();

private:
    void sendResponse(CommandID id, ResponseResult result);
    void sendError(ErrorID id, const char* info = nullptr);

    Sensors* m_sensors;
    LedController* m_ledController;
    FanController* m_fanController;
    HatchController* m_hatchController;

    StaticJsonDocument<512> m_jsonBuffer;
};