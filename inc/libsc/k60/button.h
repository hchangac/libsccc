/*
 * button.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstdint>

#include "libbase/k60/gpio.h"

namespace libsc
{
namespace k60
{

class Button
{
public:
	explicit Button(const uint8_t id);

	bool IsDown() const;
	bool IsUp() const
	{
		return !IsDown();
	}

private:
	libbase::k60::Gpi m_pin;
};

}
}
