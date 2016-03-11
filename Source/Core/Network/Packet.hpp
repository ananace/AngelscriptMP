#include <SFML/Network/Packet.hpp>

namespace Network
{

struct BitSize
{
	BitSize(uint8_t size) :
		Size(size)
	{

	}

	uint8_t Size;
};

class Packet : sf::Packet
{
public:
	Packet& operator>>(BitSize size);
	Packet& operator>>(bool&         data);
	Packet& operator>>(int8_t&       data);
	Packet& operator>>(uint8_t&      data);
	Packet& operator>>(int16_t&      data);
	Packet& operator>>(uint16_t&     data);
	Packet& operator>>(int32_t&      data);
	Packet& operator>>(uint32_t&     data);
	Packet& operator>>(int64_t&      data);
	Packet& operator>>(uint64_t&     data);
	Packet& operator>>(float&        data);
	Packet& operator>>(double&       data);
	Packet& operator>>(char*         data);
	Packet& operator>>(std::string&  data);
	Packet& operator>>(wchar_t*      data);
	Packet& operator>>(std::wstring& data);
	Packet& operator>>(sf::String&   data);

	Packet& operator<<(BitSize size);
	Packet& operator<<(bool                data);
	Packet& operator<<(int8_t              data);
	Packet& operator<<(uint8_t             data);
	Packet& operator<<(int16_t             data);
	Packet& operator<<(uint16_t            data);
	Packet& operator<<(int32_t             data);
	Packet& operator<<(uint32_t            data);
	Packet& operator<<(int64_t             data);
	Packet& operator<<(uint64_t            data);
	Packet& operator<<(float               data);
	Packet& operator<<(double              data);
	Packet& operator<<(const char*         data);
	Packet& operator<<(const std::string&  data);
	Packet& operator<<(const wchar_t*      data);
	Packet& operator<<(const std::wstring& data);
	Packet& operator<<(const sf::String&   data);

private:
	const BitSize* m_BitSizeOverride;
};

}