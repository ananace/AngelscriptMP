#include "StateMachine.hpp"
#include "States/IState.hpp"

#include <algorithm>
#include <typeinfo>

StateMachine::StateMachine(Engine& eng) :
	mEngine(eng), mCurState(nullptr), mRenderer(nullptr)
{

}
StateMachine::~StateMachine()
{
	for (auto& state : mOldStates)
	{
		if (state == mCurState)
			continue;

		delete state;
	}

	if (mCurState)
	{
		mCurState->exit(mRenderer);
		delete mCurState;
	}
}

void StateMachine::event(const sf::Event& ev)
{
	if (mCurState)
		mCurState->event(ev);
}

void StateMachine::tick(const Timespan& dt)
{
	if (mCurState)
		mCurState->tick(dt);
}
void StateMachine::update(const Timespan& dt)
{
	if (mCurState)
		mCurState->update(dt);
}
void StateMachine::draw(Graphics::Renderer::IRenderer& target)
{
	if (mRenderer != &target)
		mRenderer = &target;
	if (mCurState)
		mCurState->draw(target);
}
void StateMachine::drawUI(Graphics::Renderer::IRenderer& target)
{
	if (mRenderer != &target)
		mRenderer = &target;
	if (mCurState)
		mCurState->drawUI(target);
}

void StateMachine::primeRenderer(Graphics::Renderer::IRenderer* rt)
{
	mRenderer = rt;
}

void StateMachine::changeState(IState* to, bool remove)
{
	if (mCurState == to)
		return;

	auto it = std::find_if(mOldStates.begin(), mOldStates.end(), [&to](IState* st) {
		return st == to;
	});

	if (mCurState)
	{
		mCurState->exit(mRenderer);

		auto it2 = std::find_if(mOldStates.begin(), mOldStates.end(), [this](IState* st) {
			return st == mCurState;
		});

		if (remove)
		{
			delete mCurState;

			if (it2 != mOldStates.end())
				mOldStates.erase(it2);
		}
		else if (it2 == mOldStates.end())
			mOldStates.push_front(mCurState);
	}

	if (to)
	{
		to->mStateMachine = this;
		to->enter(mRenderer);
	}

	if (it == mOldStates.end())
		mOldStates.push_back(to);

	mCurState = to;
}
