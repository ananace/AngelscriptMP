#include "CPUID.hpp"

#include <SFML/Config.hpp>

#include <iostream>
#include <cstring>

#if defined SFML_SYSTEM_LINUX
#    ifdef __GNUC__
#        include <cpuid.h>

#        define CPUID(type, out) do { __get_cpuid(type, out, out+1, out+2, out+3); } while(0)
#        define CPUID_SUB(type, level, out) do { __cpuid_count(type, level, out[0], out[1], out[2], out[3]); } while(0)
#    else
#        define CPUID(type, out) asm("cpuid\n\t" : "=a" (out[0]), "=b" (out[1]), "=c" (out[2]), "=d" (out[3]) : "0" (type))
#        define CPUID_SUB(type, level, out) asm("cpuid\n\t" : "=a" (out[0]), "=b" (out[1]), "=c" (out[2]), "=d" (out[3]) : "0" (type), "2" (level))
#    endif
#elif defined SFML_SYSTEM_WINDOWS
#    ifdef __icc
#        include <ia32intrin.h>

#        define CPUID(type, out) do { __cpuid(out, type); } while(0)
#        define CPUID_SUB(type, level, out) do { __cpuidex((int*)out, type, level); } while(0)
#    elseif _MSC_VER
#        include <intrin.h>

#        define CPUID(type, out) do { __cpuid((int*)out, type); } while(0)
#        define CPUID_SUB(type, level, out) do { __cpuidex((int*)out, type, level); } while(0)
#    endif
#endif

uint64_t CPUFeatures::sFlags[2] = { 0 };
std::size_t CPUFeatures::sCacheLineSize = CPUFeatures::init();

namespace
{
    template<typename T>
    bool compare_mem(const T* a, const T* b, std::size_t n)
    {
        volatile T diff = 0;

        for (std::size_t i = 0; i != n; ++i)
            diff |= (a[i] ^ b[i]);

        return diff == 0;
    }

    template<typename T>
    uint8_t getU8(std::size_t n, T inp)
    {
        return static_cast<uint8_t>(inp >> ((sizeof(T)-1 - (n&(sizeof(T)-1))) << 3));
    }
}

std::size_t CPUFeatures::init()
{
	const uint32_t INTEL[3] = { 0x756E6547, 0x6C65746E, 0x49656E69 };
	const uint32_t AMD[3] = { 0x68747541, 0x444D4163, 0x69746E65 };

	uint32_t cpuid[4] = { 0 };
	CPUID(0, cpuid);

	uint32_t maxLevel = cpuid[0];
	if (maxLevel == 0)
		return 0;

	const bool isIntel = compare_mem(cpuid + 1, INTEL, 3);
	const bool isAMD = compare_mem(cpuid + 1, AMD, 3);

	CPUID(1, cpuid);
	sFlags[0] = (static_cast<uint64_t>(cpuid[2]) << 32) | cpuid[3];

	std::size_t cacheSize = 0;

	if (isIntel)
		cacheSize = 8 * getU8(2, cpuid[1]);

	if (maxLevel >= 7)
	{
		std::memset(cpuid, 0, 4);
		CPUID_SUB(7, 0, cpuid);
		sFlags[1] = (static_cast<uint64_t>(cpuid[2]) << 32) | cpuid[1];
	}

	if (isAMD)
	{
		CPUID(0x80000005, cpuid);
		cacheSize = getU8(3, cpuid[2]);
	}

	return cacheSize;
}

void CPUFeatures::print()
{
#if defined YARN_LINUX
#define CHECKFLAG(flag) << "  "#flag": " << (has##flag() ? "\x1b[32mYes\x1b[0m" : "\x1b[31mNo\x1b[0m") << std::endl
#else
#define CHECKFLAG(flag) << "  "#flag": " << (has##flag() ? "Yes" : "No") << std::endl
#endif

	std::cout << "CPU Features: " << std::endl
	CHECKFLAG(ADX)
	CHECKFLAG(AESNI)
	CHECKFLAG(AVX)
	CHECKFLAG(AVX2)
	CHECKFLAG(AVX512F)
	CHECKFLAG(BMI2)
	CHECKFLAG(CLMUL)
	CHECKFLAG(FMA3)
	CHECKFLAG(MMX)
	CHECKFLAG(RDRAND)
	CHECKFLAG(RDSEED)
	CHECKFLAG(RDTSC)
	CHECKFLAG(SHA)
	CHECKFLAG(SSE)
	CHECKFLAG(SSE2)
	CHECKFLAG(SSE3)
	CHECKFLAG(SSE41)
	CHECKFLAG(SSE42)
	CHECKFLAG(SSSE3);

#undef CHECKFLAG
}
