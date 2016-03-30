#pragma once

#include "../StateMachine.hpp"

class IState
{
public:
	virtual ~IState() { }

	virtual void enter(Graphics::Renderer::IRenderer*) { }
	virtual void exit(Graphics::Renderer::IRenderer*) { }

	virtual void event(const sf::Event&) = 0;
	virtual void tick(const Timespan&) = 0;
	virtual void update(const Timespan&) = 0;
	virtual void draw(Graphics::Renderer::IRenderer&) = 0;
	virtual void drawUI(Graphics::Renderer::IRenderer&) = 0;

protected:
	inline StateMachine* getStateMachine() { return mStateMachine; }
	inline Engine& getEngine() { return mStateMachine->mEngine; }

private:
	StateMachine* mStateMachine;

	friend class StateMachine;
};
