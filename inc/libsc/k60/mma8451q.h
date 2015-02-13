/*
 * mma8451q.h
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <array>
#include <cstdint>

#include "libbase/k60/soft_i2c_master.h"
#include "libbase/misc_types.h"

namespace libsc
{
namespace k60
{

class Mma8451q
{
public:
	struct Config
	{
		enum struct Sensitivity
		{
			High = 0,
			Mid,
			Low
		};

		enum struct OutputDataRate
		{
			k800Hz = 0,
			k400Hz,
			k200Hz,
			k100Hz,
			k50Hz,
			k12_5Hz,
			k6_25Hz,
			k1_56Hz
		};

		enum struct PowerMode
		{
			Normal = 0,
			LowNoiseLowPower,
			HighResolution,
			LowPower
		};

		uint8_t id;

		/** Maximun acceleration:
		 *  Low - 2g, Mid - 4g, High - 8g */
		Sensitivity sens = Sensitivity::Mid;

		PowerMode power_mode = PowerMode::LowNoiseLowPower;
		OutputDataRate output_data_rate = OutputDataRate::k200Hz;
	};

	explicit Mma8451q(const Mma8451q::Config &config);

	bool IsConnected();

	// 13ms
	bool Update();

	const std::array<float, 3>& GetAccel() const
	{
		return m_lastAccel;
	}

	const std::array<float, 3>& GetAngle() const
	{
		return m_lastAngle;
	}

private:
	// 2ms
	void GetAllAccel();
	// 0ms
	void GetAllAngle();

	void SetActive(const bool flag);

	Byte ReadRegByte(const Byte RegAddr);
	bool WriteRegByte(const Byte RegAddr, const Byte data);
	Byte* ReadRegBytes(const Byte RegAddr, const Byte Length);
	bool WriteRegBytes(const Byte RegAddr, const Byte *data);

	libbase::k60::SoftI2cMaster m_I2cMaster;
	Config::Sensitivity m_Sens;
	float m_ScaleFactor;

	std::array<float, 3> m_lastAccel;
	std::array<float, 3> m_lastAngle;

};

}

}
