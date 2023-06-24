#include "sensors.h"

#include <Arduino.h>

#include "common.h"

using namespace Common;

Sensors::Sensors()
{
    init();
}

Sensors::~Sensors() 
{
    for (auto oneWireBus : m_oneWireBuses) {
        if (oneWireBus != nullptr) {
            delete oneWireBus;
            oneWireBus = nullptr;
        }
        m_oneWireBuses.clear();
    }

    for (auto tSensor : m_tSensors) {
        if (tSensor != nullptr) {
            delete tSensor;
            tSensor = nullptr;
        }
        m_tSensors.clear();
    }
}

void Sensors::update()
{
    for (auto i = 0; i < m_tSensors.size(); i++) {
        switch (m_tSensorStates[i])
        {
        case INITIALIZING:
            m_tSensorStates[i] = m_tSensors[i]->begin() ? READY : INITIALIZING;
            break;
        case READY:
            m_tSensors[i]->requestTemperatures();
            m_tSensorStates[i] = MEASURING;       
            break;
        case MEASURING:
            if (m_tSensors[i]->isConversionComplete()) {
                m_temperatures[i] = m_tSensors[i]->getTempC();
                m_tSensorStates[i] = READY;
                // Serial.print("T sensor #");
                // Serial.print(i);
                // Serial.print(" got temperature: ");
                // Serial.println(m_temperatures[i]);
            }
            break;
        default:
            m_temperatures[i] = T_READ_ERROR_VALUE;
            break;
        }
    }

    m_lights[0] = analogRead(L1_SENSOR_PIN);
    m_lights[1] = analogRead(L2_SENSOR_PIN);

    // for (auto i = 0; i < m_lights.size(); i++) {
    //     Serial.print("L sensor #");
    //     Serial.print(i);
    //     Serial.print(" read value: ");
    //     Serial.println(m_lights[i]);
    // }

    // Serial.println("---------------------------");
}

float Sensors::temperature(const uint8_t sensorIndex) const
{
    return (sensorIndex >= 0 && sensorIndex < m_temperatures.size()) ?
             m_temperatures[sensorIndex] : T_WRONG_INDEX_VALUE;
}

const std::vector<float>& Sensors::temperature() const
{
    return m_temperatures;
}

int Sensors::light(const uint8_t sensorIndex) const
{
    return (sensorIndex >= 0 && sensorIndex < m_lights.size()) ?
             m_lights[sensorIndex] : L_WRONG_INDEX_VALUE;
}

const std::vector<int>& Sensors::light() const
{
    return m_lights;
}

void Sensors::init()
{
    m_temperatures.push_back(T_INIT_VALUE);
    m_temperatures.push_back(T_INIT_VALUE);

    m_tSensorStates.push_back(IDLE);
    m_tSensorStates.push_back(IDLE);

    m_oneWireBuses.push_back(new OneWire(T1_SENSOR_PIN));
    m_oneWireBuses.push_back(new OneWire(T2_SENSOR_PIN));

    m_tSensors.push_back(new DS18B20(m_oneWireBuses[0]));
    m_tSensors.push_back(new DS18B20(m_oneWireBuses[1]));
    

    m_tSensorStates[0] = m_tSensors[0]->begin() ? READY : INITIALIZING;
    m_tSensorStates[1] = m_tSensors[1]->begin() ? READY : INITIALIZING;

    m_lights.push_back(L_INIT_VALUE);
    m_lights.push_back(L_INIT_VALUE);
}