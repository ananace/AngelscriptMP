#include "VBO.hpp"

#include "../Voxel.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

using namespace Graphics::Renderer;

VBO::VBO(sf::RenderWindow& target) : IRenderer(target)
{
}

bool VBO::isAvailable() const
{
	auto& rt = getRenderWindow();
	auto& settings = rt.getSettings();

	return settings.majorVersion > 2 ||
		(settings.majorVersion == 2 && settings.minorVersion >= 1);
}

void VBO::beginRender()
{
	// TODO: Set GL States
}
void VBO::endRender()
{
	// TODO: Reset GL States
}

void VBO::render(const Voxel* voxels, uint32_t count)
{
	// TODO: Bind and render VBOs
}

