#include "Transform3D.hpp"

#include <memory>
#include <cmath>

using namespace Graphics;

const Transform3D Transform3D::Identity {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

Transform3D::Transform3D(const float matrix[16])
{
	std::copy(matrix, matrix + 16, mMatrix);
}

Transform3D::Transform3D(
	float a00, float a01, float a02, float a03,
	float a10, float a11, float a12, float a13,
	float a20, float a21, float a22, float a23,
	float a30, float a31, float a32, float a33
) :
	mMatrix {
		a00, a01, a02, a03,
		a10, a11, a12, a13,
		a20, a21, a22, a23,
		a30, a31, a32, a33
	}
{
}

Transform3D Transform3D::operator*(const Transform3D& other) const
{
	auto copy = *this;
	return copy.combine(other);
}
Transform3D& Transform3D::operator*=(const Transform3D& other)
{
	return combine(other);
}

const float* Transform3D::getMatrix() const
{
	return mMatrix;
}

Transform3D Transform3D::getInverse() const
{
	/*
	const auto& a = mMatrix;

	// Compute the determinant
	float det =
		a[0] * (a[15] * a[5] - a[7] * a[13]) -
		a[1] * (a[15] * a[4] - a[7] * a[12]) +
		a[3] * (a[13] * a[4] - a[5] * a[12]);

	// Compute the inverse if the determinant is not zero
	// (don't use an epsilon because the determinant may *really* be tiny)
	if (det != 0.f)
	{
		return Transform3D(
			(a[15] * a[5] - a[7] * a[13]) / det,
			-(a[15] * a[4] - a[7] * a[12]) / det,
			(a[13] * a[4] - a[5] * a[12]) / det,
			-(a[15] * a[1] - a[3] * a[13]) / det,
			(a[15] * a[0] - a[3] * a[12]) / det,
			-(a[13] * a[0] - a[1] * a[12]) / det,
			(a[7]  * a[1] - a[3] * a[5])  / det,
			-(a[7]  * a[0] - a[3] * a[4])  / det,
			(a[5]  * a[0] - a[1] * a[4])  / det);
	}
	else
	*/
	{
		return Identity;
	}
}

Transform3D& Transform3D::combine(const Transform3D& other)
{
	const auto &a = mMatrix, &b = other.mMatrix;

	float matrix[] = {
		a[0]  * b[0] + a[1]  * b[4] + a[2]  * b[8]  + a[3]  * b[12],
		a[0]  * b[1] + a[1]  * b[5] + a[2]  * b[9]  + a[3]  * b[13],
		a[0]  * b[2] + a[1]  * b[6] + a[2]  * b[10] + a[3]  * b[14],
		a[0]  * b[3] + a[1]  * b[7] + a[2]  * b[11] + a[3]  * b[15],
		a[4]  * b[0] + a[5]  * b[4] + a[6]  * b[8]  + a[7]  * b[12],
		a[4]  * b[1] + a[5]  * b[5] + a[6]  * b[9]  + a[7]  * b[13],
		a[4]  * b[2] + a[5]  * b[6] + a[6]  * b[10] + a[7]  * b[14],
		a[4]  * b[3] + a[5]  * b[7] + a[6]  * b[11] + a[7]  * b[15],
		a[8]  * b[0] + a[9]  * b[4] + a[10] * b[8]  + a[11] * b[12],
		a[8]  * b[1] + a[9]  * b[5] + a[10] * b[9]  + a[11] * b[13],
		a[8]  * b[2] + a[9]  * b[6] + a[10] * b[10] + a[11] * b[14],
		a[8]  * b[3] + a[9]  * b[7] + a[10] * b[11] + a[11] * b[15],
		a[12] * b[0] + a[13] * b[4] + a[14] * b[8]  + a[15] * b[12],
		a[12] * b[1] + a[13] * b[5] + a[14] * b[9]  + a[15] * b[13],
		a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14],
		a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15]
	};

	std::move(matrix, matrix + 16, mMatrix);

	return *this;
}

Transform3D& Transform3D::rotate(float x, float y, float z)
{
	if (x != 0)
		rotate(Axis_X, x);
	if (y != 0)
		rotate(Axis_Y, y);
	if (z != 0)
		rotate(Axis_Z, z);

	return *this;
}
Transform3D& Transform3D::rotate(Axis a, float ang)
{
	const float cos = std::cos(ang),
		sin = std::sin(ang);

	switch (a)
	{
	case Axis_X:
		return combine({
			1, 0, 0, 0,
			0, cos, -sin, 0,
			0, sin, cos, 0,
			0, 0, 0, 1
		});

	case Axis_Y:
		return combine({
			cos, 0, sin, 0,
			0, 1, 0, 0,
			-sin, 0, cos, 0,
			0, 0, 0, 1
		});
	case Axis_Z:
		return combine({
			cos, -sin, 0, 0,
			sin, cos, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		});
	}

	return *this;
}

Transform3D& Transform3D::translate(float x, float y, float z)
{
	return combine({
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1
	});
}
Transform3D& Transform3D::translate(const sf::Vector3f& vec)
{
	return combine({
		1, 0, 0, vec.x,
		0, 1, 0, vec.y,
		0, 0, 1, vec.z,
		0, 0, 0, 1
	});
}

Transform3D& Transform3D::scale(float x, float y, float z)
{
	return combine({
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	});
}
Transform3D& Transform3D::scale(const sf::Vector3f& vec)
{
	return combine({
		vec.x, 0, 0, 0,
		0, vec.y, 0, 0,
		0, 0, vec.z, 0,
		0, 0, 0,     1
	});
}
