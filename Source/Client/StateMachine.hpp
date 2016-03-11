#pragma once

#include <Core/Time.hpp>

#include <list>

namespace sf { class Event; class RenderTarget; }

class IState;
class Engine;
class StateMachine
{
public:
	StateMachine(Engine&);
	~StateMachine();

	void event(const sf::Event&);
	void tick(const Timespan&);
	void update(const Timespan&);
	void draw(sf::RenderTarget&);
	void drawUI(sf::RenderTarget&);

	template<typename T>
	void changeState(bool remove = false);
	void changeState(IState* to, bool remove = false);

	void primeRT(sf::RenderTarget* rt);

private:
	
	Engine& mEngine;

	IState* mCurState;
	sf::RenderTarget* mLastSeenRT;

	std::list<IState*> mOldStates;

	friend class IState;
};

#include "StateMachine.inl"
