#include "BitWriter.hpp"

#include <cassert>
#include <cmath>

using Network::BitWriter;

BitWriter::BitWriter()
	: mScratch(0)
	, mBitIndex(0)
	, mBitsWritten(0)
{

}

namespace Network
{

template<>
inline bool BitWriter::writeUInt(uint32_t data, uint8_t bits)
{
	assert(bits > 0 && bits <= 32);

	data &= (uint32_t(1) << bits) - 1;

	mScratch |= uint64_t(data) << (64 - mBitIndex - bits);
	mBitIndex += bits;

	if (mBitIndex >= 32)
	{
		uint32_t data = mScratch >> 32;
		
		mScratch <<= 32;
		mBitIndex -= 32;

		mData.push_back((data >> 24) & 0xff);
		mData.push_back((data >> 16) & 0xff);
		mData.push_back((data >> 8) & 0xff);
		mData.push_back((data) & 0xff);
	}

	mBitsWritten += bits;

	return true;
}
template<>
bool BitWriter::writeUInt(uint16_t data, uint8_t bits)
{
	return writeUInt(uint32_t(data), bits);
}
template<>
bool BitWriter::writeUInt(uint8_t data, uint8_t bits)
{
	return writeUInt(uint32_t(data), bits);
}

template<>
inline bool BitWriter::writeInt(int32_t data, uint8_t bits)
{
	assert(bits > 1 && bits <= 32);

	if (data < 0)
	{
		writeUInt(1, 1);
		writeUInt(1 - data, bits - 1);
	}
	else
	{
		writeUInt(0, 1);
		writeUInt(data, bits - 1);
	}
}
template<>
bool BitWriter::writeInt(uint16_t data, uint8_t bits)
{
	return writeInt(uint32_t(data), bits);
}
template<>
bool BitWriter::writeInt(uint8_t data, uint8_t bits)
{
	return writeInt(uint32_t(data), bits);
}

}

bool BitWriter::writeFloat(float data, float min, float max, uint8_t bits)
{
	assert(bits > 0 && bits <= 32);
	assert(data >= min && data <= max);

	double perc = double(data - min) / double(max - min);
	uint32_t value = uint32_t(perc * uint32_t(1 << bits));

	return writeUInt(value, bits);
}

bool BitWriter::writeString(const std::string& data, uint8_t lengthBits)
{
	if (!writeUInt(data.length(), lengthBits))
		return false;

	for (auto& c : data)
		if (!writeUInt(c, 8))
			return false;

	return true;
}
bool BitWriter::writeASCIIString(const std::string& data, uint8_t lengthBits)
{
	if (!writeUInt(data.length(), lengthBits))
		return false;

	for (auto& c : data)
		if (!writeUInt(c, 7))
			return false;

	return true;
}

size_t BitWriter::tell() const
{
	return size_t(std::ceil(mBitsWritten / 8.f));
}
size_t BitWriter::bitTell() const
{
	return mBitsWritten;
}

void BitWriter::flush()
{
	if (mBitIndex == 0)
		return;

	uint32_t data = mScratch >> 32;

	if (mBitIndex > 24)
		mData.push_back((data >> 24) & 0xff);
	if (mBitIndex > 16)
		mData.push_back((data >> 16) & 0xff);
	if (mBitIndex > 8)
		mData.push_back((data >> 8) & 0xff);

	mData.push_back((data)& 0xff);

	mBitIndex = 0;
}
