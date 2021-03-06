/*
 * remote_var_manager.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include "libbase/log.h"

#if MK60D10 || MK60DZ10 || MK60F15
#include "libbase/k60/misc_utils.h"
#include "libsc/k60/uart_device.h"
using namespace libsc::k60;

#elif MKL26Z4
#include "libbase/kl26/misc_utils.h"
#include "libsc/kl26/uart_device.h"
using namespace libsc::kl26;

#endif

#include "libutil/endian_utils.h"
#include "libutil/remote_var_manager.h"
#include "libutil/string.h"


using namespace std;

namespace libutil
{

namespace
{

float AsFloat(const uint32_t val)
{
	return *reinterpret_cast<const float*>(&val);
}

}

RemoteVarManager::Var::Var()
		: m_type(Type::kInt),
		  m_id(0),
		  m_val(0)
{}

RemoteVarManager::Var::Var(Var &&rhs)
		: Var()
{
	*this = std::move(rhs);
}

RemoteVarManager::Var& RemoteVarManager::Var::operator=(Var &&rhs)
{
	if (this != &rhs)
	{
		m_name = std::move(rhs.m_name);
		m_type = rhs.m_type;
		m_id = rhs.m_id;
		m_val = rhs.m_val;
	}
	return *this;
}

RemoteVarManager::RemoteVarManager(const size_t var_count)
		: m_buffer_it(0)
{
	m_vars.reserve(var_count);
}

RemoteVarManager::~RemoteVarManager()
{}

RemoteVarManager::Var* RemoteVarManager::Register(const string &name,
		const Var::Type type)
{
	if (m_vars.size() >= m_vars.capacity())
	{
		LOG_WL("RemoteVarManager expanding");
		assert(false);
		return nullptr;
	}

	Var var;
	var.m_name = name;
	var.m_type = type;
	var.m_id = m_vars.size();
	m_vars.push_back(std::move(var));
	return &m_vars.back();
}

RemoteVarManager::Var* RemoteVarManager::Register(string &&name,
		const Var::Type type)
{
	if (m_vars.size() >= m_vars.capacity())
	{
		LOG_WL("RemoteVarManager expanding");
		assert(false);
		return nullptr;
	}

	Var var;
	var.m_name = std::move(name);
	var.m_type = type;
	var.m_id = m_vars.size();
	m_vars.push_back(std::move(var));
	return &m_vars.back();
}

void RemoteVarManager::Broadcast(UartDevice *uart)
{
	for (size_t i = 0; i < m_vars.size(); ++i)
	{
		if (m_vars[i].m_type == Var::Type::kInt)
		{
			uart->SendStr(String::Format("%s,int,%d,%d\n",
					m_vars[i].m_name.c_str(), m_vars[i].m_id,
					EndianUtils::HostToBe(m_vars[i].m_val)));
		}
		else
		{
			uart->SendStr(String::Format("%s,real,%d,%.3f\n",
					m_vars[i].m_name.c_str(), m_vars[i].m_id,
					AsFloat(EndianUtils::HostToBe(m_vars[i].m_val))));
		}
	}
}

bool RemoteVarManager::OnUartReceiveChar(const Byte *data, const size_t size)
{
	for (size_t i = 0; i < size; ++i)
	{
		OnUartReceiveSingleChar(data[i]);
	}
	return true;
}

bool RemoteVarManager::OnUartReceiveSingleChar(const Byte data)
{
	m_buffer[m_buffer_it] = data;
	if (++m_buffer_it >= 5)
	{
		const uint32_t val = m_buffer[1] << 24 | m_buffer[2] << 16
				| m_buffer[3] << 8 | m_buffer[4];
		m_vars[(size_t)m_buffer[0]].m_val = val;
		m_buffer_it = 0;
	}
	return true;
}

}
