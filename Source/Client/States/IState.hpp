#pragma once

#include "../StateMachine.hpp"

class IState
{
public:
	virtual ~IState() { }

	virtual void enter(sf::RenderTarget*) { }
	virtual void exit(sf::RenderTarget*) { }

	virtual void event(const sf::Event&) = 0;
	virtual void tick(const Timespan&) = 0;
	virtual void update(const Timespan&) = 0;
	virtual void draw(sf::RenderTarget&) = 0;
	virtual void drawUI(sf::RenderTarget&) = 0;

protected:
	inline StateMachine* getStateMachine() { return mStateMachine; }
	inline Engine& getEngine() { return mStateMachine->mEngine; }

private:
	StateMachine* mStateMachine;

	friend class StateMachine;
};