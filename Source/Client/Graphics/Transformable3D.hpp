#pragma once

#include "Transform3D.hpp"

namespace Graphics
{

class Transformable3D
{
public:

	virtual ~Transformable3D() = default;

	virtual void move(float x, float y, float z);
	virtual void move(const sf::Vector3f& vec);
	virtual void rotate(float p, float y, float r);

	virtual const Transform3D& getTransform() const;
	virtual const Transform3D& getInverseTransform() const;

protected:

private:
	Transform3D mTransform;
};

}
