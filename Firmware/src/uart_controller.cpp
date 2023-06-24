#include "uart_controller.h"
#include "fan_controller.h"
#include "hatch_controller.h"
#include "led_controller.h"
#include "sensors.h"

UartController::UartController(Sensors *sensors,
                               LedController *ledController,
                               FanController *fanController,
                               HatchController *hatchController)
                               : m_sensors(sensors)
                               , m_ledController(ledController)
                               , m_fanController(fanController)
                               , m_hatchController(hatchController)
{
}

void UartController::update()
{
    if (Serial.available() < 10) {
        return;
    }

    const auto rxBuffer = Serial.readString();

    m_jsonBuffer.clear();
    DeserializationError error = deserializeJson(m_jsonBuffer, rxBuffer.c_str());
    if (error) {
        sendError(JSON_PARSE_ERROR, error.c_str());
        return;
    } else if (!m_jsonBuffer.containsKey(MESSAGE_TYPE_PROP)) {
        sendError(NO_MESSAGE_TYPE);
        return;
    } else if (m_jsonBuffer[MESSAGE_TYPE_PROP].as<int>() != REQUEST) {
        sendError(WRONG_MESSAGE_TYPE, "Only requests is allowed!");
        return;
    } else if (!m_jsonBuffer.containsKey(MESSAGE_ID_PROP)) {
        sendError(NO_MESSAGE_ID);
        return;
    }

    const auto id = static_cast<CommandID>(m_jsonBuffer[MESSAGE_ID_PROP].as<int>());
    switch (id)
    {
    case GET_SENSORS:
        sendResponse(id, DONE);
        break;    
    case SET_FAN_CPS:
    {
        if (m_jsonBuffer.containsKey(CPS_PROP)) {
            const float cps = m_jsonBuffer[CPS_PROP].as<float>();
            m_fanController->setTargetCps(cps);
            sendResponse(id, PENDING);
        } else {
            sendError(UNCOMPLETED_REQUEST, "No target CPS value");
        }
        break;
    }
    case SET_FAN_PWM:
        break;
    case SET_FAN_PWM_PERCENT:
        break;
    case GET_FAN_INFO:
        sendResponse(id, DONE);
        break;
    case SET_LEDS_PWM:
        break;                                                                                            
    case SET_LEDS_SHADE:
    {
        auto isValid = false;
        if (m_jsonBuffer.containsKey(RED_COLOR_PROP)) {
            isValid = true;
            m_ledController->setLightR(m_jsonBuffer[RED_COLOR_PROP].as<int>());
        }

        if (m_jsonBuffer.containsKey(GREEN_COLOR_PROP)) {
            isValid = true;
            m_ledController->setLightG(m_jsonBuffer[GREEN_COLOR_PROP].as<int>());
        }

        if (m_jsonBuffer.containsKey(BLUE_COLOR_PROP)) {
            isValid = true;
            m_ledController->setLightB(m_jsonBuffer[BLUE_COLOR_PROP].as<int>());
        }

        if (m_jsonBuffer.containsKey(BRIGHTNESS_PROP)) {
            isValid = true;
            m_ledController->setLightBrightness(m_jsonBuffer[BRIGHTNESS_PROP].as<int>());
        }

        if (isValid) {
            m_ledController->setUseShadeMode(true);
            m_ledController->applyLightColor();
            sendResponse(id, DONE);
        } else {
            sendError(UNCOMPLETED_REQUEST, "At least one value is needed");
        }
        break;
    }
    case GET_LEDS_PWM:
        break;
    case GET_LEDS_SHADE:
        sendResponse(id, DONE);
        break;
    case REQ_HATCH_RECALIBRATE:
        break;                                                                        
    case SET_HATCH_POS:
    {
        if (m_jsonBuffer.containsKey(HATCH_POS_PROP)) {
            if (m_hatchController->getState() == HatchController::READY) {
                m_hatchController->requestOpenHatch(m_jsonBuffer[HATCH_POS_PROP].as<int>());
                sendResponse(id, PENDING);
            } else {
                sendResponse(id, BUSY_IGNORED);
            }
        } else {
            sendError(UNCOMPLETED_REQUEST, "No hatch open value");
        }
        break;
    }
    case GET_HATCH_POS:
        sendResponse(id, DONE);
        break;
    default:
        sendError(WRONG_COMMAND_ID);
        break;
    }
}

void UartController::sendResponse(CommandID id, ResponseResult result)
{
    m_jsonBuffer.clear();
    m_jsonBuffer[MESSAGE_TYPE_PROP]     = RESPONSE;
    m_jsonBuffer[MESSAGE_ID_PROP]       = id;
    m_jsonBuffer[RESPONSE_RESULT_PROP]  = result;
    switch (id)
    {
    case GET_SENSORS:
        if (result == DONE) {
            m_jsonBuffer[TEMPERATURE1_PROP] = m_sensors->temperature(0);
            m_jsonBuffer[TEMPERATURE2_PROP] = m_sensors->temperature(1);
            m_jsonBuffer[LIGHT1_PROP]       = m_sensors->light(0);
            m_jsonBuffer[LIGHT2_PROP]       = m_sensors->light(1);
        }
        break;
    case GET_FAN_INFO:
        if (result == DONE) {
            m_jsonBuffer[CPS_PROP]          = m_fanController->averageCps();
        }
        break;
    case GET_LEDS_PWM:
        break;
    case GET_LEDS_SHADE:
        if (result == DONE) {
            const auto colors = m_ledController->lightColor();
            m_jsonBuffer[RED_COLOR_PROP]    = colors[LedController::RED];
            m_jsonBuffer[GREEN_COLOR_PROP]  = colors[LedController::GREEN];
            m_jsonBuffer[BLUE_COLOR_PROP]   = colors[LedController::BLUE];
            m_jsonBuffer[BRIGHTNESS_PROP]   = m_ledController->lightBrightness();
        }
        break;
    case GET_HATCH_POS:
        if (result == DONE) {
            m_jsonBuffer[HATCH_POS_PROP] = m_hatchController->currentPosition();
        }
        break;                    
    default:
        break;
    }

    serializeJson(m_jsonBuffer, Serial);
    Serial.println();
}

void UartController::sendError(ErrorID id, const char* info)
{
    m_jsonBuffer.clear();
    m_jsonBuffer[MESSAGE_TYPE_PROP] = ERROR;
    m_jsonBuffer[MESSAGE_ID_PROP]   = id;
    if (info != nullptr) {
        m_jsonBuffer[INFO_PROP]     = info;
    }

    serializeJson(m_jsonBuffer, Serial);
    Serial.println();
}
