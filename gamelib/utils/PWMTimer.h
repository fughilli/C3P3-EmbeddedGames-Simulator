#ifndef _PWM_TIMER_H_
#define _PWM_TIMER_H_

class PWMTimer
{
private:
    uint32_t m_period, m_highband, m_counter;
public:
    PWMTimer(uint32_t period, uint32_t highband)
    {
        m_period = period;
        m_highband = highband;
        m_counter = 0;
    }

    bool operator()()
    {
        bool ret = ((m_counter++) < m_highband);
        if(m_counter == m_period)
            m_counter = 0;
        return ret;
    }
};

#endif // _PWM_TIMER_H_

