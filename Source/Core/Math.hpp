#pragma once

namespace Math
{

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

#include "Math.inl"
