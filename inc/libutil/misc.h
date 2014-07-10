/*
 * misc.h
 * Misc util
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBUTIL_MISC_H_
#define LIBUTIL_MISC_H_

#include <cstdint>
#include <algorithm>

namespace libsc
{
namespace k60
{

class UartDevice;

}
}

#define SAFE_DELETE(x) do{if (x) {delete x; x = nullptr;}}while(false)

#define UTIL_JOIN(x, y) x ## y

namespace libutil
{

template<typename T>
inline T Clamp(const T &min, const T &x, const T &max)
{
	return std::max(min, std::min(x, max));
}

template<typename T>
inline T ClampVal(const T min, const T x, const T max)
{
	return (x > min) ? ((x > max) ? max : x) : min;
}

inline uint16_t GetRgb565(const uint8_t r, const uint8_t g, const uint8_t b)
{
	return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

void InitDefaultFwriteHandler(libsc::k60::UartDevice *uart);
void UninitDefaultFwriteHandler();

// host to big-endian encoding
uint32_t htobe32(const uint32_t host_32bits);
// host to lil-endian encoding
uint32_t htole32(const uint32_t host_32bits);
// big-endian to host encoding
uint32_t be32toh(const uint32_t big_endian_32bits);
// lil-endian to host encoding
uint32_t le32toh(const uint32_t little_endian_32bits);

}

#endif /* LIBUTIL_MISC_H_ */
