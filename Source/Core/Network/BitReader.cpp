#include "BitReader.hpp"

using Network::BitReader;

BitReader::BitReader()
	: mBitIndex(0)
	, mWordIndex(0)
	, mBitsRead(0)
	, mBitsTotal(0)
{

}

template<>
inline bool BitReader::readUInt(uint32_t& data, uint8_t bits)
{
	assert(bits > 0 && bits <= sizeof(data) * 8);

	if (mBitsRead + bits > mBitsTotal)
	{
		data = 0;

		mBitsRead = mBitsTotal;

		return false;
	}

	mBitsRead += bits;

	if (mBitIndex + bits < 32)
	{
		mScratch <<= bits;
		mBitIndex += bits;
	}
	else
	{
		const auto a = 32 - mBitIndex;
		const auto b = bits - a;

		mScratch <<= a;
		mScratch |= mData[++mWordIndex];
		mScratch <<= b;

		mBitIndex = b;
	}

	data = uint32_t(mScratch >> 32);
	mScratch &= 0xFFFFFFFF;

	return true;
}

template<>
inline bool BitReader::readUInt(uint8_t& data, uint8_t bits)
{
	assert(bits > 0 && bits <= sizeof(data) * 8);

	uint32_t temp;
	if (!readUInt(temp, bits))
		return false;

	data = temp;

	return true;
}
template<>
inline bool BitReader::readUInt(uint16_t& data, uint8_t bits)
{
	assert(bits > 0 && bits <= sizeof(data) * 8);

	uint32_t temp;
	if (!readUInt(temp, bits))
		return false;

	data = temp;

	return true;
}
/*
template<>
inline bool BitReader::readUInt(uint64_t& data, uint8_t bits)
{
	assert(bits > 0 && bits <= sizeof(data) * 8);

	uint32_t temp;
	if (!readUInt(temp, bits / 2))
		return false;

	uint32_t temp2;
	if (!readUInt(temp2, bits / 2))
		return false;

	data = 0;
	data |= (uint64_t(temp) << 32);
	data |= temp;

	return true;
}
*/
template<>
inline bool BitReader::readInt(int8_t& data, uint8_t bits)
{
	assert(bits > 1 && bits <= sizeof(data) * 8);

	bool neg;
	if (!readUInt(neg, 1))
		return false;

	uint8_t temp;
	if (!readUInt(temp, bits - 1))
		return false;

	data = neg ? -1 - temp : temp;
	return true;
}
template<>
inline bool BitReader::readInt(int16_t& data, uint8_t bits)
{
	assert(bits > 1 && bits <= sizeof(data) * 8);

	bool neg;
	if (!readUInt(neg, 1))
		return false;

	uint16_t temp;
	if (!readUInt(temp, bits - 1))
		return false;

	data = neg ? -1 - temp : temp;
	return true;
}
template<>
inline bool BitReader::readInt(int32_t& data, uint8_t bits)
{
	assert(bits > 1 && bits <= sizeof(data) * 8);

	bool neg;
	if (!readUInt(neg, 1))
		return false;

	uint32_t temp;
	if (!readUInt(temp, bits - 1))
		return false;

	data = neg ? -1 - temp : temp;
	return true;
}

bool BitReader::readFloat(float& data, float min, float max, uint8_t bits)
{
	assert(bits > 1 && bits <= sizeof(data) * 8);

	uint32_t temp;
	if (!readUInt(temp, bits))
		return false;

	double perc = double(temp) / double((1 << bits) - 1);
	data = float(min + (max - min) * perc);

	return true;
}

bool BitReader::readString(std::string& data, uint8_t lengthBits)
{
	assert(lengthBits > 1 && lengthBits <= 32);

	uint32_t size;
	if (!readUInt(size, lengthBits))
		return false;

	data.resize(size);
	for (uint32_t i = 0; i < size; ++i)
	{
		if (!readUInt(data[i], 8))
			return false;
	}

	return true;
}
bool BitReader::readASCIIString(std::string& data, uint8_t lengthBits)
{
	assert(lengthBits > 1 && lengthBits <= 32);

	uint32_t size;
	if (!readUInt(size, lengthBits))
		return false;

	data.resize(size);
	for (uint32_t i = 0; i < size; ++i)
	{
		if (!readUInt(data[i], 7))
			return false;
	}

	return true;
}

size_t BitReader::size() const
{
	return mBitsTotal / 8;
}
size_t BitReader::bitSize() const
{
	return mBitsTotal;
}
size_t BitReader::tell() const
{
	return mBitIndex / 8;
}
size_t BitReader::bitTell() const
{
	return mBitIndex;
}

bool BitReader::atEnd() const
{
	return mBitIndex >= mBitsTotal;
}