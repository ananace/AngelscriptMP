#pragma once

#include "IRenderer.hpp"

namespace Graphics
{
namespace Renderer
{

class VBO : public IRenderer
{
public:
	VBO(sf::RenderWindow&);

	bool isAvailable() const;

	void beginRender();
	void endRender();

	void render(const Voxel* voxels, uint32_t count);

private:

};

}
}

