#include "Transformable3D.hpp"
#include <Core/Math.hpp>

using namespace Graphics;

Transformable3D::Transformable3D() :
	mTransformDirty(false), mInverseTransformDirty(false)
{

}

void Transformable3D::move(float x, float y, float z)
{
	mPosition += { x, y, z };
	mTransformDirty = mInverseTransformDirty = true;
}
void Transformable3D::move(const sf::Vector3f& vec)
{
	mPosition += vec;
	mTransformDirty = mInverseTransformDirty = true;
}
void Transformable3D::rotate(float p, float y, float r)
{
	mRotation += { p, y, r };
	mTransformDirty = mInverseTransformDirty = true;
}
void Transformable3D::scale(float x, float y, float z)
{
	mScale *= sf::Vector3f(x, y, z);
	mTransformDirty = mInverseTransformDirty = true;
}
void Transformable3D::scale(const sf::Vector3f& vec)
{
	mScale *= vec;
	mTransformDirty = mInverseTransformDirty = true;
}

void Transformable3D::setPosition(float x, float y, float z)
{
	mPosition = { x, y, z };
	mTransformDirty = mInverseTransformDirty = true;
}
void Transformable3D::setPosition(const sf::Vector3f& vec)
{
	mPosition = vec;
	mTransformDirty = mInverseTransformDirty = true;
}
void Transformable3D::setRotation(float p, float y, float r)
{
	mRotation = { p, y, r };
	mTransformDirty = mInverseTransformDirty = true;
}
void Transformable3D::setScale(float x, float y, float z)
{
	mScale = { x, y, z };
	mTransformDirty = mInverseTransformDirty = true;
}
void Transformable3D::setScale(const sf::Vector3f& vec)
{
	mScale = vec;
	mTransformDirty = mInverseTransformDirty = true;
}

const Transform3D& Transformable3D::getTransform()
{
	if (mTransformDirty)
	{
		// TODO: Improve and replace with direct matrix calculation
		mTransform = Transform3D::Identity;
		mTransform.translate(mPosition).rotate(mRotation.x, mRotation.y, mRotation.z).scale(mScale);

		mTransformDirty = false;
	}

	return mTransform;
}
const Transform3D& Transformable3D::getInverseTransform()
{
	if (mInverseTransformDirty)
	{
		mInverseTransform = mTransform.getInverse();
		mInverseTransformDirty = false;
	}

	return mInverseTransform;
}
