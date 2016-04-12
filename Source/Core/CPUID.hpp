#pragma once

#include <cstdint>

struct CPUFeatures
{
	static std::size_t guessCacheLineSize() {
		return sCacheLineSize;
	}

#define CHECKFLAG(flag) inline static bool has##flag() { \
	return hasFlag(CPUID_##flag##_BIT); \
}

	CHECKFLAG(RDTSC)
	CHECKFLAG(MMX)
	CHECKFLAG(SSE)
	CHECKFLAG(SSE2)
	CHECKFLAG(SSE3)
	CHECKFLAG(CLMUL)
	CHECKFLAG(SSSE3)
	CHECKFLAG(FMA3)
	CHECKFLAG(SSE41)
	CHECKFLAG(SSE42)
	CHECKFLAG(AESNI)
	CHECKFLAG(AVX)
	CHECKFLAG(RDRAND)
	CHECKFLAG(AVX2)
	CHECKFLAG(BMI2)
	CHECKFLAG(AVX512F)
	CHECKFLAG(RDSEED)
	CHECKFLAG(ADX)
	CHECKFLAG(SHA)

#undef CHECKFLAG

	static void print();

private:
	static std::size_t init();

	enum CPUID_bits {
		CPUID_RDTSC_BIT = 4,
		CPUID_MMX_BIT = 23,
		CPUID_SSE_BIT = 25,
		CPUID_SSE2_BIT = 26,
		CPUID_SSE3_BIT = 32,
		CPUID_CLMUL_BIT = 33,
		CPUID_SSSE3_BIT = 41,
		CPUID_FMA3_BIT = 44,
		CPUID_SSE41_BIT = 51,
		CPUID_SSE42_BIT = 52,
		CPUID_AESNI_BIT = 57,
		CPUID_AVX_BIT = 60,
		CPUID_RDRAND_BIT = 62,

		CPUID_AVX2_BIT = 64 + 5,
		CPUID_BMI2_BIT = 64 + 8,
		CPUID_AVX512F_BIT = 64 + 16,
		CPUID_RDSEED_BIT = 64 + 18,
		CPUID_ADX_BIT = 64 + 19,
		CPUID_SHA_BIT = 64 + 29,
	};

	inline static bool hasFlag(uint64_t flag) {
		return ((sFlags[flag / 64] >> (flag % 64)) & 1);
	}

	static uint64_t sFlags[2];
	static std::size_t sCacheLineSize;
};
