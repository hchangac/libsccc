/*
 * futaba_s3010.h
 * Futaba S3010 RC servo
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstdint>

#include "libsc/k60/servo.h"

namespace libsc
{
namespace k60
{

class FutabaS3010 : public Servo
{
public:
	explicit FutabaS3010(const uint8_t id);
};

}
}
