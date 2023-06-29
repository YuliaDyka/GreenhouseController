#pragma once
#include <OneWire.h>
#include <DS18B20.h>
#include <ArduinoSTL.h>
#include <vector>

class Sensors
{
public:
    enum State
    {
        READY,
        MEASURING,
        INITIALIZING,
        IDLE
    };

    Sensors();
    ~Sensors();

    void update();

    float temperature(const uint8_t sensorIndex) const;
    const std::vector<float> &temperature() const;

    int light(const uint8_t sensorIndex) const;
    const std::vector<int> &light() const;

private:
    void init();

    std::vector<float> m_temperatures{};
    std::vector<int> m_lights{};

    std::vector<State> m_tSensorStates{};
    std::vector<OneWire *> m_oneWireBuses{};
    std::vector<DS18B20 *> m_tSensors{};
};