#include "averager.h"

Averager::Averager(size_t size)
    : m_size(size)
    , m_summa(0.0f)
    , m_avgValue(0.0f)
    , m_deviation(0.0f)
{
    clear();
}

Averager::~Averager()
{
}

void Averager::resize(size_t newSize)
{
    m_size = newSize;
    clear();
}

void Averager::clear()
{
    m_data.clear();
    m_summa = 0.0f;
    m_avgValue = 0.0f;
    m_deviation = 0.0f;
}

float Averager::push(const float value)
{
    m_summa += value;

    if (m_data.size() == m_size) {
        m_summa -= m_data.back();
        m_data.pop_back();
    }    
    m_data.push_front(value);
    const float newAvg = m_summa / m_data.size();
    m_deviation = newAvg - m_avgValue;
    m_avgValue = newAvg;

    return m_avgValue;
}

float Averager::avgValue() const
{
    return m_avgValue;
}

float Averager::deviation() const
{
    return m_deviation;
}
