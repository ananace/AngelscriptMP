#pragma once

#include <string>
#include <vector>

#include <cfloat>
#include <cstdint>
#include <cassert>

namespace Network
{

class BitReader
{
public:
	BitReader();
	BitReader(const BitReader&) = default;
	BitReader(BitReader&&) = default;
	~BitReader() = default;

	BitReader& operator=(const BitReader&) = default;

	template<typename T>
	bool readUInt(T& data, uint8_t bits = sizeof(T) * 8);
	template<typename T>
	bool readInt(T& data, uint8_t bits = sizeof(T) * 8);

	bool readFloat(float& data, float min = -FLT_MAX, float max = FLT_MAX, uint8_t bits = 32);

	bool readString(std::string& data, uint8_t lengthBits = 8);
	bool readASCIIString(std::string& data, uint8_t lengthBits = 8);

	size_t size() const;
	size_t bitSize() const;
	size_t tell() const;
	size_t bitTell() const;

	bool atEnd() const;

private:
	std::vector<uint32_t> mData;
	uint64_t mScratch;

	size_t mBitIndex;
	size_t mWordIndex;
	size_t mBitsRead;
	size_t mBitsTotal;
};

}