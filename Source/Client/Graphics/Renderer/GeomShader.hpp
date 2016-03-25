#pragma once

#include "IRenderer.hpp"

#include <SFML/Graphics/Shader.hpp>

#if SFML_VERSION_MAJOR > 2 || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR > 3) || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR == 3 && SFML_VERSION_PATCH > 2)
#define GEOM_SHADERS
#endif

namespace Graphics
{
namespace Renderer
{

class GeomShader : public IRenderer
{
public:
	GeomShader(sf::RenderWindow&);

	bool isAvailable() const;

	void beginRender();
	void endRender();

	void render(const Voxel* voxels, uint32_t count);

private:
	sf::Shader mShader;
};

}
}
