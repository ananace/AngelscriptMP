#pragma once

#include <SFML/System/Vector3.hpp>

namespace Graphics
{

class Transform3D
{
public:
	enum Axis
	{
		Axis_X,
		Axis_Y,
		Axis_Z
	};

	Transform3D(const float matrix[16]);
	Transform3D(
		float a00, float a01, float a02, float a03,
		float a10, float a11, float a12, float a13,
		float a20, float a21, float a22, float a23,
		float a30, float a31, float a32, float a33
	);

	Transform3D(const Transform3D& copy) = default;
	Transform3D(Transform3D&& move) = default;
	~Transform3D() = default;

	Transform3D& operator=(const Transform3D& assign) = default;

	Transform3D operator*(const Transform3D& combine) const;
	Transform3D& operator*=(const Transform3D& combine);
	sf::Vector3f operator*(const sf::Vector3f& vec) const;

	const float* getMatrix() const;
	Transform3D getInverse() const;

	Transform3D& combine(const Transform3D& other);

	Transform3D& rotate(float x, float y, float z);
	Transform3D& rotate(Axis a, float ang);
	Transform3D& scale(float x, float y, float z);
	Transform3D& scale(const sf::Vector3f& vec);
	Transform3D& translate(float x, float y, float z);
	Transform3D& translate(const sf::Vector3f& vec);

	sf::Vector3f transformPoint(const sf::Vector3f& vec) const;

	static const Transform3D Identity;

private:
	float mMatrix[16];
};

}
