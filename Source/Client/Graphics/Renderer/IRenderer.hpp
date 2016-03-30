#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include <cstdint>

namespace Graphics
{

class Drawable3D;
class Voxel;

namespace Renderer
{

class IRenderer
{
public:
	IRenderer(sf::RenderWindow& target) : mRT(target) { }

	virtual ~IRenderer() = default;

	virtual bool isAvailable() const = 0;

	virtual void beginRender() = 0;
	virtual void endRender() = 0;

	virtual void render(const Voxel* voxels, uint32_t count) = 0;
	virtual void render(const Drawable3D&);

	sf::RenderTarget& getRT() { return mRT; }
	const sf::RenderTarget& getRT() const { return mRT; }

protected:
	sf::RenderWindow& getRenderWindow() { return mRT; }
	const sf::RenderWindow& getRenderWindow() const { return mRT; }

private:
	sf::RenderWindow& mRT;
};

}
}
