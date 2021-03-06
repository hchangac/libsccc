/*
 * mkl26z4_lqfp100.cpp
 *
 * Author: Harrison Ng
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstdint>

#include <bitset>

#include "libbase/kl26/pinout/mkl26z4_lqfp100.h"

#include "libbase/kl26/adc.h"
#include "libbase/kl26/pin.h"
#include "libbase/kl26/pin_utils.h"
#include "libbase/kl26/spi.h"
#include "libbase/kl26/tpm.h"
#include "libbase/kl26/uart.h"

#include "libutil/misc.h"

using namespace std;
using namespace libutil;

namespace libbase
{
namespace kl26
{

namespace
{

int GetPinId(const Pin::Name pin)
{
	if ((Uint)pin <= (Uint)Pin::Name::kPta5)
	{
		return (Uint)pin - (Uint)Pin::Name::kPta0 + 33;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPta6
			&& (Uint)pin <= (Uint)Pin::Name::kPta7)
	{
		return (Uint)pin - (Uint)Pin::Name::kPta6 + 39;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPta12
			&& (Uint)pin <= (Uint)Pin::Name::kPta17)
	{
		return (Uint)pin - (Uint)Pin::Name::kPta12 + 41;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPta18
			&& (Uint)pin <= (Uint)Pin::Name::kPta20)
	{
		return (Uint)pin - (Uint)Pin::Name::kPta18 + 49;
	}

	else if ((Uint)pin >= (Uint)Pin::Name::kPtb0
			&& (Uint)pin <= (Uint)Pin::Name::kPtb3)
	{
		return (Uint)pin - (Uint)Pin::Name::kPtb0 + 52;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPtb7
			&& (Uint)pin <= (Uint)Pin::Name::kPtb11)
	{
		return (Uint)pin - (Uint)Pin::Name::kPtb7 + 56;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPtb16
			&& (Uint)pin <= (Uint)Pin::Name::kPtb23)
	{
		return (Uint)pin - (Uint)Pin::Name::kPtb16 + 61;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPtc0
			&& (Uint)pin <= (Uint)Pin::Name::kPtc3)
	{
		return (Uint)pin - (Uint)Pin::Name::kPtc0 + 69;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPtc4
			&& (Uint)pin <= (Uint)Pin::Name::kPtc18)
	{
		return (Uint)pin - (Uint)Pin::Name::kPtc4 + 75;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPtd0
			&& (Uint)pin <= (Uint)Pin::Name::kPtd7)
	{
		return (Uint)pin - (Uint)Pin::Name::kPtd0 + 92;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPte0
			&& (Uint)pin <= (Uint)Pin::Name::kPte6)
	{
		return (Uint)pin - (Uint)Pin::Name::kPte0;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPte16
			&& (Uint)pin <= (Uint)Pin::Name::kPte23)
	{
		return (Uint)pin - (Uint)Pin::Name::kPte16 + 13;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPte29
			&& (Uint)pin <= (Uint)Pin::Name::kPte31)
	{
		return (Uint)pin - (Uint)Pin::Name::kPte29 + 25;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPte24
			&& (Uint)pin <= (Uint)Pin::Name::kPte26)
	{
		return (Uint)pin - (Uint)Pin::Name::kPte24 + 30;
	}
	else
	{
		assert(false);
		return -1;
	}
}

int GetPinId(const Adc::Name pin)
{
	if (pin == Adc::Name::kAdc0DAd1)
	{
		return 13;
	}
	else if (pin == Adc::Name::kAdc0Ad5A)
	{
		return 14;
	}
	else if (pin == Adc::Name::kAdc0DAd2)
	{
		return 15;
	}
	else if (pin == Adc::Name::kAdc0Ad6A)
	{
		return 16;
	}
	else if (pin == Adc::Name::kAdc0DAd0)
	{
		return 17;
	}
	else if (pin == Adc::Name::kAdc0Ad4A)
	{
		return 18;
	}
	else if (pin == Adc::Name::kAdc0DAd3)
	{
		return 19;
	}
	else if (pin == Adc::Name::kAdc0Ad7A)
	{
		return 20;
	}
	else if (pin == Adc::Name::kAdc0Ad4B)
	{
		return 25;
	}
	else if (pin == Adc::Name::kAdc0Ad23)
	{
		return 27;
	}
	else if (pin == Adc::Name::kAdc0Ad8)
	{
		return 52;
	}
	else if (pin == Adc::Name::kAdc0Ad9)
	{
		return 53;
	}
	else if (pin == Adc::Name::kAdc0Ad12)
	{
		return 54;
	}
	else if (pin == Adc::Name::kAdc0Ad13)
	{
		return 55;
	}
	else if (pin == Adc::Name::kAdc0Ad14)
	{
		return 69;
	}
	else if (pin == Adc::Name::kAdc0Ad15)
	{
		return 70;
	}
	else if (pin == Adc::Name::kAdc0Ad11)
	{
		return 71;
	}
	else if (pin == Adc::Name::kAdc0Ad6B)
	{
		return 97;
	}
	else if (pin == Adc::Name::kAdc0Ad7B)
	{
		return 98;
	}
	else
	{
		assert(false);
		return -1;
	}
}

}

Mkl26z4Lqfp100::Mkl26z4Lqfp100()
{}

bool Mkl26z4Lqfp100::RegPin_(const Uint pin)
{
	if (pin >= GetPinCount())
	{
		assert(false);
		return false;
	}

	if (m_is_pins_active[pin])
	{
		return false;
	}
	else
	{
		m_is_pins_active[pin] = true;
		return true;
	}
}

bool Mkl26z4Lqfp100::RegPin_(const Pin::Name pin)
{
	return RegPin_((Uint)GetPinId(pin));
}

bool Mkl26z4Lqfp100::RegPin_(const Adc::Name pin)
{
	return RegPin_((Uint)GetPinId(pin));
}

void Mkl26z4Lqfp100::UnregPin_(const Uint pin)
{
	if (pin >= GetPinCount())
	{
		assert(false);
		return;
	}

	m_is_pins_active[pin] = false;
}

void Mkl26z4Lqfp100::UnregPin_(const Pin::Name pin)
{
	UnregPin_((Uint)GetPinId(pin));
}

void Mkl26z4Lqfp100::UnregPin_(const Adc::Name pin)
{
	UnregPin_((Uint)GetPinId(pin));
}

Adc::Name Mkl26z4Lqfp100::GetAdc(const Pin::Name pin)
{
	switch (pin)
	{
	case Pin::Name::kPtb0:
		return Adc::Name::kAdc0Ad8;

	case Pin::Name::kPtb1:
		return Adc::Name::kAdc0Ad9;

	case Pin::Name::kPtb2:
		return Adc::Name::kAdc0Ad12;

	case Pin::Name::kPtb3:
		return Adc::Name::kAdc0Ad13;

	case Pin::Name::kPtc0:
		return Adc::Name::kAdc0Ad14;

	case Pin::Name::kPtc1:
		return Adc::Name::kAdc0Ad15;

	case Pin::Name::kPtc2:
		return Adc::Name::kAdc0Ad11;

	case Pin::Name::kPtd1:
		return Adc::Name::kAdc0Ad5B;

	case Pin::Name::kPtd5:
		return Adc::Name::kAdc0Ad6B;

	case Pin::Name::kPtd6:
		return Adc::Name::kAdc0Ad7B;

	case Pin::Name::kPte16:
		return Adc::Name::kAdc0DAd1;

	case Pin::Name::kPte17:
		return Adc::Name::kAdc0Ad5A;

	case Pin::Name::kPte18:
		return Adc::Name::kAdc0DAd2;

	case Pin::Name::kPte19:
		return Adc::Name::kAdc0Ad6A;

	case Pin::Name::kPte20:
		return Adc::Name::kAdc0DAd0;

	case Pin::Name::kPte21:
		return Adc::Name::kAdc0Ad4A;

	case Pin::Name::kPte22:
		return Adc::Name::kAdc0DAd3;

	case Pin::Name::kPte23:
		return Adc::Name::kAdc0Ad7A;

	case Pin::Name::kPte29:
		return Adc::Name::kAdc0Ad4B;

	case Pin::Name::kPte30:
		return Adc::Name::kAdc0Ad23;

	default:
		return Adc::Name::kDisable;
	}
}

I2c::Name Mkl26z4Lqfp100::GetI2c(const Pin::Name pin)
{
	switch (pin)
	{
	case Pin::Name::kPtb0:
	case Pin::Name::kPtb2:
	case Pin::Name::kPtc8:
	case Pin::Name::kPte19:
	case Pin::Name::kPte24:
		return I2c::Name::kI2c0Scl;

	case Pin::Name::kPtb1:
	case Pin::Name::kPtb3:
	case Pin::Name::kPtc9:
	case Pin::Name::kPte18:
	case Pin::Name::kPte25:
		return I2c::Name::kI2c0Sda;

	case Pin::Name::kPta3:
	case Pin::Name::kPtc1:
	case Pin::Name::kPtc10:
	case Pin::Name::kPte1:
		return I2c::Name::kI2c1Scl;

	case Pin::Name::kPta4:
	case Pin::Name::kPtc2:
	case Pin::Name::kPtc11:
	case Pin::Name::kPte0:
		return I2c::Name::kI2c1Sda;

	default:
		return I2c::Name::kDisable;
	}
}

Pin::Config::MuxControl Mkl26z4Lqfp100::GetI2cMux(const Pin::Name pin)
{
	if (pin == Pin::Name::kPte0 || pin == Pin::Name::kPte1)
	{
		return Pin::Config::MuxControl::kAlt6;
	}
	else if (pin == Pin::Name::kPte24 || pin == Pin::Name::kPte25){
		return Pin::Config::MuxControl::kAlt5;
	}
	else
	{
		return Pin::Config::MuxControl::kAlt2;
	}
}

Spi::MisoName Mkl26z4Lqfp100::GetSpiMiso(const Pin::Name pin)
{
	switch (pin)
	{
	case Pin::Name::kPta16:
	case Pin::Name::kPta17:
	case Pin::Name::kPtc6:
	case Pin::Name::kPtc7:
	case Pin::Name::kPtd2:
	case Pin::Name::kPtd3:
	case Pin::Name::kPte18:
	case Pin::Name::kPte19:
		return Spi::MisoName::kSpi0Miso;

	case Pin::Name::kPtb16:
	case Pin::Name::kPtb17:
	case Pin::Name::kPtd6:
	case Pin::Name::kPtd7:
	case Pin::Name::kPte0:
	case Pin::Name::kPte1:
	case Pin::Name::kPte3:
		return Spi::MisoName::kSpi1Miso;

	default:
		return Spi::MisoName::kDisable;
	}
}

Pin::Config::MuxControl Mkl26z4Lqfp100::GetSpiMisoMux(const Pin::Name pin)
{
	switch (pin)
	{
	default:
		assert(false);
		// no break

	case Pin::Name::kPta17:
	case Pin::Name::kPtb17:
	case Pin::Name::kPtc7:
	case Pin::Name::kPtd3:
	case Pin::Name::kPtd7:
	case Pin::Name::kPte0:
	case Pin::Name::kPte3:
	case Pin::Name::kPte19:
		return Pin::Config::MuxControl::kAlt2;

	case Pin::Name::kPta16:
	case Pin::Name::kPtb16:
	case Pin::Name::kPtc6:
	case Pin::Name::kPtd2:
	case Pin::Name::kPtd6:
	case Pin::Name::kPte1:
	case Pin::Name::kPte18:
		return Pin::Config::MuxControl::kAlt5;
	}
}

Spi::MosiName Mkl26z4Lqfp100::GetSpiMosi(const Pin::Name pin)
{
	switch (pin)
	{
	case Pin::Name::kPta16:
	case Pin::Name::kPta17:
	case Pin::Name::kPtc6:
	case Pin::Name::kPtc7:
	case Pin::Name::kPtd2:
	case Pin::Name::kPtd3:
	case Pin::Name::kPte18:
	case Pin::Name::kPte19:
		return Spi::MosiName::kSpi0Mosi;

	case Pin::Name::kPtb16:
	case Pin::Name::kPtb17:
	case Pin::Name::kPtd6:
	case Pin::Name::kPtd7:
	case Pin::Name::kPte1:
	case Pin::Name::kPte3:
		return Spi::MosiName::kSpi1Mosi;

	default:
		return Spi::MosiName::kDisable;
	}
}

Pin::Config::MuxControl Mkl26z4Lqfp100::GetSpiMosiMux(const Pin::Name pin)
{
	switch (pin)
	{
	default:
		assert(false);
		// no break

	case Pin::Name::kPta16:
	case Pin::Name::kPtb16:
	case Pin::Name::kPtc6:
	case Pin::Name::kPtd2:
	case Pin::Name::kPtd6:
	case Pin::Name::kPte1:
	case Pin::Name::kPte18:
		return Pin::Config::MuxControl::kAlt2;

	case Pin::Name::kPta17:
	case Pin::Name::kPtb17:
	case Pin::Name::kPtc7:
	case Pin::Name::kPtd3:
	case Pin::Name::kPtd7:
	case Pin::Name::kPte3:
	case Pin::Name::kPte19:
		return Pin::Config::MuxControl::kAlt5;
	}
}

Spi::SckName Mkl26z4Lqfp100::GetSpiSck(const Pin::Name pin)
{
	switch (pin)
	{
	case Pin::Name::kPta15:
	case Pin::Name::kPtc5:
	case Pin::Name::kPtd1:
	case Pin::Name::kPte17:
		return Spi::SckName::kSpi0Sck;

	case Pin::Name::kPtb9:
	case Pin::Name::kPtb11:
	case Pin::Name::kPtd5:
	case Pin::Name::kPte2:
		return Spi::SckName::kSpi1Sck;

	default:
		return Spi::SckName::kDisable;
	}
}

Pin::Config::MuxControl Mkl26z4Lqfp100::GetSpiSckMux(const Pin::Name pin)
{
	return Pin::Config::MuxControl::kAlt2;
}

Spi::PcsName Mkl26z4Lqfp100::GetSpiPcs(const Pin::Name pin)
{
	switch (pin)
	{
	case Pin::Name::kPta14:
	case Pin::Name::kPtc4:
	case Pin::Name::kPtd0:
	case Pin::Name::kPte16:
		return Spi::PcsName::kSpi0Pcs0;

	case Pin::Name::kPtb8:
	case Pin::Name::kPtb10:
	case Pin::Name::kPtd4:
	case Pin::Name::kPte4:
		return Spi::PcsName::kSpi1Pcs0;

	default:
		return Spi::PcsName::kDisable;
	}
}

Pin::Config::MuxControl Mkl26z4Lqfp100::GetSpiPcsMux(const Pin::Name pin)
{
	return Pin::Config::MuxControl::kAlt2;
}

Tpm::Name Mkl26z4Lqfp100::GetTpm(const Pin::Name pin)
{
	switch (pin)
	{
	case Pin::Name::kPta3:
	case Pin::Name::kPtc1:
	case Pin::Name::kPtd0:
	case Pin::Name::kPte24:
		return Tpm::Name::kTpm0Ch0;

	case Pin::Name::kPta4:
	case Pin::Name::kPtc2:
	case Pin::Name::kPtd1:
	case Pin::Name::kPte25:
		return Tpm::Name::kTpm0Ch1;

	case Pin::Name::kPta5:
	case Pin::Name::kPtc3:
	case Pin::Name::kPtd2:
	case Pin::Name::kPte29:
		return Tpm::Name::kTpm0Ch2;

	case Pin::Name::kPta6:
	case Pin::Name::kPtc4:
	case Pin::Name::kPtd3:
	case Pin::Name::kPte30:
		return Tpm::Name::kTpm0Ch3;

	case Pin::Name::kPta7:
	case Pin::Name::kPtc8:
	case Pin::Name::kPtd4:
	case Pin::Name::kPte31:
		return Tpm::Name::kTpm0Ch4;

	case Pin::Name::kPta0:
	case Pin::Name::kPtc9:
	case Pin::Name::kPtd5:
	case Pin::Name::kPte26:
		return Tpm::Name::kTpm0Ch5;

	case Pin::Name::kPta12:
	case Pin::Name::kPtb0:
	case Pin::Name::kPte20:
		return Tpm::Name::kTpm1Ch0;

	case Pin::Name::kPta13:
	case Pin::Name::kPtb1:
	case Pin::Name::kPte21:
		return Tpm::Name::kTpm1Ch1;

	case Pin::Name::kPta10:
	case Pin::Name::kPtb2:
	case Pin::Name::kPtb18:
	case Pin::Name::kPte22:
		return Tpm::Name::kTpm2Ch0;

	case Pin::Name::kPta2:
	case Pin::Name::kPtb3:
	case Pin::Name::kPtb19:
	case Pin::Name::kPte23:
		return Tpm::Name::kTpm2Ch1;

	default:
		return Tpm::Name::kDisable;
	}
}

Pin::Config::MuxControl Mkl26z4Lqfp100::GetTpmMux(const Pin::Name pin)
{
	const Uint pin_int = static_cast<Uint>(pin);
	if (pin_int >= (Uint)Pin::Name::kPtc1 && pin_int <= (Uint)Pin::Name::kPtc4)
	{
		return Pin::Config::MuxControl::kAlt4;
	}
	else if (pin_int >= (Uint)Pin::Name::kPtd0
			&& pin_int <= (Uint)Pin::Name::kPtd5)
	{
		return Pin::Config::MuxControl::kAlt4;
	}
	else
	{
		return Pin::Config::MuxControl::kAlt3;
	}
}

Tpm::ClkinName Mkl26z4Lqfp100::GetTpmClkin(const Pin::Name pin)
{
	switch (pin)
	{
	case Pin::Name::kPta18:
	case Pin::Name::kPtb16:
	case Pin::Name::kPtc12:
	case Pin::Name::kPte16:
	case Pin::Name::kPte29:
		return Tpm::ClkinName::kTpmClkin0;

	case Pin::Name::kPta19:
	case Pin::Name::kPtb17:
	case Pin::Name::kPtc13:
	case Pin::Name::kPte17:
	case Pin::Name::kPte30:
		return Tpm::ClkinName::kTpmClkin1;

	default:
		return Tpm::ClkinName::kDisable;
	}
}

Pin::Config::MuxControl Mkl26z4Lqfp100::GetTpmClkinMux(const Pin::Name)
{
	return Pin::Config::MuxControl::kAlt4;
}

Uart::Name Mkl26z4Lqfp100::GetUart(const Pin::Name pin)
{
	switch (pin)
	{
	case Pin::Name::kPta1:
	case Pin::Name::kPta15:
	case Pin::Name::kPtb16:
	case Pin::Name::kPtd6:
	case Pin::Name::kPte21:
		return Uart::Name::kUart0Rx;

	case Pin::Name::kPta2:
	case Pin::Name::kPta14:
	case Pin::Name::kPtb17:
	case Pin::Name::kPtd7:
	case Pin::Name::kPte20:
		return Uart::Name::kUart0Tx;

	case Pin::Name::kPta18:
	case Pin::Name::kPtc3:
	case Pin::Name::kPte1:
		return Uart::Name::kUart1Rx;

	case Pin::Name::kPta19:
	case Pin::Name::kPtc4:
	case Pin::Name::kPte0:
		return Uart::Name::kUart1Tx;

	case Pin::Name::kPtd2:
	case Pin::Name::kPtd4:
	case Pin::Name::kPte17:
	case Pin::Name::kPte23:
		return Uart::Name::kUart2Rx;

	case Pin::Name::kPtd3:
	case Pin::Name::kPtd5:
	case Pin::Name::kPte16:
	case Pin::Name::kPte22:
		return Uart::Name::kUart2Tx;

	default:
		return Uart::Name::kDisable;
	}
}

Pin::Config::MuxControl Mkl26z4Lqfp100::GetUartMux(const Pin::Name pin)
{
	const Uint pin_int = static_cast<Uint>(pin);
	if (pin_int >= (Uint)Pin::Name::kPte20 && pin_int <= (Uint)Pin::Name::kPte23)
	{
		return Pin::Config::MuxControl::kAlt4;
	}
	else if (pin == Pin::Name::kPta1 || pin == Pin::Name::kPta2)
	{
		return Pin::Config::MuxControl::kAlt2;
	}
	else
	{
		return Pin::Config::MuxControl::kAlt3;
	}
}

}
}
