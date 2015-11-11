#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include <cmath>

namespace Math
{

template<>
constexpr float Dot(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}
template<>
constexpr float Dot(const sf::Vector3f& v1, const sf::Vector3f& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
template<>
constexpr float LengthSquared(const sf::Vector2f& v)
{
	return v.x * v.x + v.y * v.y;
}
template<>
constexpr float LengthSquared(const sf::Vector3f& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}
template<>
inline float Length(const sf::Vector2f& v)
{
	return std::sqrt(v.x * v.x + v.y * v.y);
}
template<>
inline float Length(const sf::Vector3f& v)
{
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
template<>
inline void SetLength(sf::Vector2f& v, float len)
{
	float oldLen = Length(v);
	if (oldLen == 0)
		return;

	float newLen = len / oldLen;
	v.x *= newLen;
	v.y *= newLen;
}
template<>
inline void SetLength(sf::Vector3f& v, float len)
{
	float oldLen = Length(v);
	if (oldLen == 0)
		return;

	float newLen = len / oldLen;
	v.x *= newLen;
	v.y *= newLen;
	v.z *= newLen;
}
template<typename T>
constexpr float PolarAngle(const T& v)
{
	return std::atan2(v.y, v.x);
}
template<typename T>
inline void SetPolarAngle(T& v, float ang)
{
	float len = Length(v);

	v.x = len * std::cos(ang);
	v.y = len * std::sin(ang);
}

}
