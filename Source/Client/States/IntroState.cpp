#include "IntroState.hpp"

#include <Graphics/Renderer/IRenderer.hpp>
#include <Core/Math.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Window/Event.hpp>

IntroState::IntroState() :
	mTime(0)
{

}
IntroState::~IntroState()
{

}

void IntroState::event(const sf::Event& ev)
{
	
}
void IntroState::tick(const Timespan&)
{

}
void IntroState::update(const Timespan& dt)
{
	mTime += dt;
}
void IntroState::draw(Graphics::Renderer::IRenderer& rend)
{

}
void IntroState::drawUI(Graphics::Renderer::IRenderer& rend)
{
	auto& rt = rend.getRT();
}
