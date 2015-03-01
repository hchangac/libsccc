/*
 * system.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/kl26/watchdog.h"

#include "libsc/kl26/lptmr_timer.h"
#include "libsc/kl26/sys_tick_delay.h"

namespace libsc
{
namespace kl26
{

class System
{
public:
	static void Init()
	{
		if (!m_instance)
		{
			m_instance = new System;
		}
	}

	static void DelayUs(const uint16_t us)
	{
		m_instance->m_delay.DelayUs(us);
	}

	static void DelayMs(const uint16_t ms)
	{
		m_instance->m_delay.DelayMs(ms);
	}

	static void DelayS(const uint16_t s)
	{
		m_instance->m_delay.DelayS(s);
	}

	/**
	 * Return the time elapsed, in ms, since Init()
	 *
	 * @return
	 */
	static Timer::TimerInt Time()
	{
		return m_instance->m_timer.Time();
	}

	static void FeedDog()
	{
		libbase::kl26::Watchdog::Refresh();
	}

private:
	System();

	SysTickDelay m_delay;
	LptmrTimer m_timer;

	static System *m_instance;
};

}
}
