#include "IRenderer.hpp"
#include "../Drawable3D.hpp"

using namespace Graphics::Renderer;

IRenderer::IRenderer(sf::RenderWindow& target)
	: mRT(target)
{

}

IRenderer::~IRenderer() { }

void IRenderer::render(const Drawable3D& drawable)
{
	drawable.draw(*this);
}
