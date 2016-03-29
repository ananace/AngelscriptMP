#pragma once

#include "Renderer/IRenderer.hpp"

namespace Graphics
{

class Drawable3D
{
public:
	virtual void draw(Renderer::IRenderer& renderer) const = 0;
};

}
