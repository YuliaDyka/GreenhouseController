#pragma once

#include <ArduinoSTL.h>
#include <list>

class Averager
{
public:
    Averager(size_t size);
    ~Averager();

    void resize(size_t newSize);
    void clear();
    float push(const float value);

    float avgValue() const;
    float deviation() const;

private:
    size_t m_size;
    float m_summa;
    float m_avgValue;
    float m_deviation;

    std::list<float> m_data{};
};


