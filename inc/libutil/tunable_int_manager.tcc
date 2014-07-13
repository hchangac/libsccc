/*
 * tunable_int_manager.tcc
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBUTIL_TUNABLE_INT_MANAGER_TCC_
#define LIBUTIL_TUNABLE_INT_MANAGER_TCC_

#include <cassert>
#include <cstdint>

#include <functional>

#include "libbase/k60/misc_utils.h"

#include "libsc/k60/uart_device.h"
#include "libutil/string.h"
#include "libutil/tunable_int_manager.h"

namespace libutil
{

template<uint8_t size>
TunableIntManager<size>* TunableIntManager<size>::GetInstance(
		libsc::k60::UartDevice *uart)
{
	if (!m_instance)
	{
		m_instance = new TunableIntManager(uart);
	}
	return m_instance;
}

template<uint8_t size>
TunableIntManager<size>::TunableIntManager(libsc::k60::UartDevice *uart)
		: m_uart(uart), m_curr_id(0), m_buffer_it(0)
{}

template<uint8_t size>
const TunableInt* TunableIntManager<size>::Register(const char *name,
		const TunableInt::Type type, const uint32_t val)
{
	if (m_curr_id >= size)
	{
		assert(false);
	}

	TunableInt *var = &m_data[m_curr_id];
	var->m_name = name;
	var->m_type = type;
	var->m_id = m_curr_id;
	var->m_val = val;
	++m_curr_id;
	return var;
}

template<uint8_t size>
void TunableIntManager<size>::Start(const bool is_broadcast)
{
	m_uart->EnableRx([this](const Byte *bytes, const size_t count)
			{
				OnUartReceiveChar(bytes, count);
			});

	if (is_broadcast)
	{
		for (int i = 0; i < size && m_data[i].m_name; ++i)
		{
			if (m_data[i].m_type == TunableInt::INTEGER)
			{
				m_uart->SendStr(String::Format("%s,integer,%d,%d\n",
						m_data[i].m_name, m_data[i].m_id,
						htobe32(m_data[i].m_val)));
			}
			else
			{
				m_uart->SendStr(String::Format("%s,real,%d,%.3f\n",
						m_data[i].m_name, m_data[i].m_id,
						TunableInt::AsFloat(htobe32(m_data[i].m_val))));
			}
		}
	}
}

template<uint8_t size>
void TunableIntManager<size>::Stop()
{
	m_uart->DisableRx();
}

template<uint8_t size>
void TunableIntManager<size>::OnUartReceiveChar(const Byte *bytes,
		const size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		m_buffer[m_buffer_it] = bytes[i];
		if (++m_buffer_it >= 5)
		{
			uint32_t val = m_buffer[1] << 24 | m_buffer[2] << 16
					| m_buffer[3] << 8 | m_buffer[4];
			m_data[(Uint)m_buffer[0]].SetValue(be32toh(val));
			m_buffer_it = 0;
		}
	}
}

}

#endif /* LIBUTIL_TUNABLE_INT_MANAGER_TCC_ */
