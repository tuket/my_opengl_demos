#include <string>

#ifndef UTIL_H
#define UTIL_H

std::string loadTextFile(const std::string& fileName);

inline unsigned nextPowerOfTwo(unsigned x)
{
	unsigned res = 1;
	while(res<x) res <<= 1;
	return res;
}

#endif
