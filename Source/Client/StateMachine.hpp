#pragma once

#include <Core/Time.hpp>

#include <list>

namespace Graphics { namespace Renderer { class IRenderer; } }
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
	void draw(Graphics::Renderer::IRenderer&);
	void drawUI(Graphics::Renderer::IRenderer&);

	template<typename T>
	void changeState(bool remove = false);
	void changeState(IState* to, bool remove = false);

	void primeRenderer(Graphics::Renderer::IRenderer* rt);

private:
	
	Engine& mEngine;

	IState* mCurState;
	Graphics::Renderer::IRenderer* mRenderer;

	std::list<IState*> mOldStates;

	friend class IState;
};

#include "StateMachine.inl"
