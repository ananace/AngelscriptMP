#include "GeomShader.hpp"

using namespace Graphics::Renderer;

GeomShader::GeomShader(sf::RenderWindow& target) : IRenderer(target)
{
}

#ifdef GEOM_SHADERS

#include <SFML/OpenGL.hpp>

bool GeomShader::isAvailable() const
{
	return sf::Shader::isGeometryAvailable();
}

void GeomShader::beginRender()
{

}
void GeomShader::endRender()
{

}

void GeomShader::render(const Voxel* voxels, uint32_t count)
{

}

#else
bool GeomShader::isAvailable() const
{
	return false;
}
void GeomShader::beginRender() { }
void GeomShader::endRender() { }
void GeomShader::render(const Voxel* voxels, uint32_t count) { }
#endif
