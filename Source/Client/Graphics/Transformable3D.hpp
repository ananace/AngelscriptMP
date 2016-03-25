#pragma once

#include "Transform3D.hpp"

namespace Graphics
{

class Transformable3D
{
public:
	Transformable3D();
	virtual ~Transformable3D() = default;

	virtual void move(float x, float y, float z);
	virtual void move(const sf::Vector3f& vec);
	virtual void rotate(float p, float y, float r);
	virtual void scale(float x, float y, float z);

	virtual void setPosition(float x, float y, float z);
	virtual void setPosition(const sf::Vector3f& vec);
	virtual void setRotation(float p, float y, float r);
	virtual void setScale(float x, float y, float z);
	virtual void setScale(const sf::Vector3f& vec);

	virtual const Transform3D& getTransform();
	virtual const Transform3D& getInverseTransform();

protected:

private:
	bool mTransformDirty, mInverseTransformDirty;
	sf::Vector3f mPosition, mRotation, mScale;
	Transform3D mTransform, mInverseTransform;
};

}
