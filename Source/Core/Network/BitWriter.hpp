#pragma once

#include <list>
#include <string>

#include <cfloat>
#include <cstdint>

namespace Network
{

class BitWriter
{
public:
	BitWriter();
	BitWriter(const BitWriter&) = default;
	BitWriter(BitWriter&&) = default;
	~BitWriter() = default;

	BitWriter& operator=(const BitWriter&) = default;

	template<typename T>
	bool writeUInt(T data, uint8_t bits = sizeof(T) * 8);
	template<typename T>
	bool writeInt(T data, uint8_t bits = sizeof(T) * 8);

	bool writeFloat(float data, float min = -FLT_MAX, float max = FLT_MAX, uint8_t bits = 32);

	bool writeString(const std::string& data, uint8_t lengthBits = 8);
	bool writeASCIIString(const std::string& data, uint8_t lengthBits = 8);

	size_t tell() const;
	size_t bitTell() const;

private:
	void flush();

	std::list<uint8_t> mData;
	uint64_t mScratch;

	size_t mBitIndex;
	size_t mBitsWritten;
};

}