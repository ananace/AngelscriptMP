#pragma once

#include "IState.hpp"
#include <Graphics/Voxel.hpp>

class IntroState : public IState
{
public:
	IntroState();
	~IntroState();

	virtual void event(const sf::Event&);
	virtual void tick(const Timespan&);
	virtual void update(const Timespan&);
	virtual void draw(Graphics::Renderer::IRenderer&);
	virtual void drawUI(Graphics::Renderer::IRenderer&);

private:
	Timespan mTime;
	Graphics::Voxel mVoxel;
};
