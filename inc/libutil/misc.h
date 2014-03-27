/*
 * misc.h
 * Misc util
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBUTIL_MISC_H_
#define LIBUTIL_MISC_H_

#include <algorithm>

#define SAFE_DELETE(x) do{if (x) {delete x; x = nullptr;}}while(false)
#ifdef DEBUG
#define DEBUG_PRINT(...) printf(__VAR_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

namespace libutil
{

template<typename T>
inline T Clamp(const T &min, const T &x, const T &max)
{
	return std::max(min, std::min(x, max));
}

}

#endif /* LIBUTIL_MISC_H_ */
