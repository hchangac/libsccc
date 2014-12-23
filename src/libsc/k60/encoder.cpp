/*
 * encoder.cpp
 * Phase A/B encoder
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cassert>
#include <cstdio>
#include <cstdint>

#include "libbase/log.h"
#include "libbase/k60/ftm_quad_decoder.h"
#include "libbase/k60/gpio.h"
#include "libbase/k60/soft_quad_decoder.h"
#include "libbase/k60/vectors.h"

#include "libsc/config.h"
#include "libsc/k60/encoder.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_ENCODER

namespace
{

#if LIBSC_USE_ENCODER == 1
inline Pin::Name GetQda(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_ENCODER0_QDA;
}

inline Pin::Name GetQdb(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_ENCODER0_QDB;
}

#else
inline Pin::Name GetQda(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break

	case 0:
		return LIBSC_ENCODER0_QDA;

	case 1:
		return LIBSC_ENCODER1_QDA;
	}
}

inline Pin::Name GetQdb(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break

	case 0:
		return LIBSC_ENCODER0_QDB;

	case 1:
		return LIBSC_ENCODER1_QDB;
	}
}

#endif // LIBSC_USE_ENCODER

#ifdef LIBSC_USE_SOFT_ENCODER
SoftQuadDecoder::Config GetQuadDecoderConfig(const uint8_t id)
{
	SoftQuadDecoder::Config config;
	config.a_pin = GetQda(id);
	config.b_pin = GetQdb(id);
	return config;
}

#else
FtmQuadDecoder::Config GetQuadDecoderConfig(const uint8_t id)
{
	FtmQuadDecoder::Config config;
	config.a_pin = GetQda(id);
	config.b_pin = GetQdb(id);
	config.encoding_mode = FtmQuadDecoder::Config::EncodingMode::kPhaseAB;
	config.a_filter_length = 1;
	config.b_filter_length = 1;
	return config;
}

#endif

}

Encoder::Encoder(const Config &config)
		: m_count(0),
		  m_quad_decoder(GetQuadDecoderConfig(config.id))
{}

void Encoder::Update()
{
	m_count = m_quad_decoder.GetCount();
	m_quad_decoder.ResetCount();
	return;
}

#else
Encoder::Encoder(const Config&)
		: m_count(0)
{
	LOG_DL("Configured not to use Encoder");
}
void Encoder::Update() {}

#endif /* LIBSC_USE_ENCODER */

}
}
