#include "Voxel.hpp"

using namespace Graphics;

Voxel::Voxel()
{

}

bool Voxel::loadFromFile(const std::string& file)
{
	return false;
}
bool Voxel::loadFromStream(sf::InputStream& stream)
{
	return false;
}

void Voxel::draw(Renderer::IRenderer& renderer) const
{
	renderer.render(this, 1);
}
