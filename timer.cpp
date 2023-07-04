#include <stdio.h>
#include <cassert>

#include "timer.h"

timer_ticks timer::g_init_ticks;
timer_ticks timer::g_freq;
double timer::g_inv_freq;

inline void query_counter(timer_ticks* pTicks)
{
    QueryPerformanceCounter(pTicks);
}

inline void query_counter_frequency(timer_ticks* pTicks)
{
    QueryPerformanceFrequency(pTicks);
}

timer::timer() :
        m_start_time(),
        m_stop_time(),
        m_started(false),
        m_stopped(false)
{
    if (!g_inv_freq)
        init();
}

timer::timer(timer_ticks start_ticks) :
        m_start_time(start_ticks),
        m_stop_time(),
        m_started(true),
        m_stopped(false)
{
    if (!g_inv_freq)
        init();
}

void timer::start(timer_ticks start_ticks)
{
    m_start_time = start_ticks;
    m_started = true;
    m_stopped = false;
}

void timer::start()
{
    query_counter(&m_start_time);
    m_started = true;
    m_stopped = false;
}

void timer::stop()
{
    assert(m_started);
    query_counter(&m_stop_time);
    m_stopped = true;
}

double timer::get_elapsed_secs() const
{
    assert(m_started);
    if (!m_started)
        return 0;

    timer_ticks stop_time = m_stop_time;
    if (!m_stopped)
        query_counter(&stop_time);

    timer_ticks delta;
    delta.QuadPart = stop_time.QuadPart - m_start_time.QuadPart;
    return delta.QuadPart * g_inv_freq;
}

timer_ticks timer::get_elapsed_us() const
{
    assert(m_started);
    if (!m_started)
        return timer_ticks();

    timer_ticks stop_time = m_stop_time;
    if (!m_stopped)
        query_counter(&stop_time);

    timer_ticks delta;
    delta.QuadPart = stop_time.QuadPart - m_start_time.QuadPart;
    return delta;
}

void timer::init()
{
    if (!g_inv_freq)
    {
        query_counter_frequency(&g_freq);
        g_inv_freq = 1.0 / g_freq.QuadPart;

        query_counter(&g_init_ticks);
    }
}

timer_ticks timer::get_init_ticks()
{
    if (!g_inv_freq)
        init();

    return g_init_ticks;
}

timer_ticks timer::get_ticks()
{
    if (!g_inv_freq)
        init();

    timer_ticks ticks;
    query_counter(&ticks);
    return timer_ticks{static_cast<DWORD>(ticks.QuadPart - g_init_ticks.QuadPart)};
    ;
}

double timer::ticks_to_secs(timer_ticks ticks)
{
    if (!g_inv_freq)
        init();

    return ticks.QuadPart * g_inv_freq;
}