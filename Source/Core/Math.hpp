#pragma once

#include <SFML/System/Vector2.hpp>

class ScriptManager;

namespace Math
{

static constexpr float PI = 3.141592653589793238463f;
static constexpr float PI2 = PI/2;
static constexpr float DEG2RAD = PI / 180;
static constexpr float RAD2DEG = 180 / PI;

extern void registerScriptData(ScriptManager&);

/** Calculate the dot product between two vectors
 */
template<typename T>
constexpr float Dot(const T&, const T&);
/** Calculate the squared length of a vector, the same as the dot product of itself
 */
template<typename T>
constexpr float LengthSquared(const T& v);
/** Calculate the real length of the vector using a sqrt
 */
template<typename T>
inline float Length(const T& v);
/** Set the length of a vector
 */
template<typename T>
void SetLength(T& v, float len);
/** Normalizes a vector
 */
template<typename T>
inline T Normalized(const T& v);
/** Calculate the polar angle of a vector
 * \note The angle is in radians
 */
template<typename T>
constexpr float PolarAngle(const T& v);
/** Set the polar angle of a vector
 * \note The angle is in radians
 */
template<typename T>
void SetPolarAngle(T& v, float ang);

}

namespace sf
{
	extern sf::Vector2f operator*(const sf::Vector2f& a, const sf::Vector2f& b);
	extern sf::Vector2f operator/(const sf::Vector2f& a, const sf::Vector2f& b);
	extern sf::Vector2f& operator*=(sf::Vector2f& a, const sf::Vector2f& b);
	extern sf::Vector2f& operator/=(sf::Vector2f& a, const sf::Vector2f& b);
}

#include "Math.inl"
