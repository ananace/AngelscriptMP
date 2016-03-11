#include "StateMachine.hpp"
#include "States/IState.hpp"

#include <algorithm>
#include <typeinfo>

StateMachine::StateMachine(Engine& eng) :
	mEngine(eng), mCurState(nullptr), mLastSeenRT(nullptr)
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
		mCurState->exit(mLastSeenRT);
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
void StateMachine::draw(sf::RenderTarget& target)
{
	if (mCurState)
		mCurState->draw(target);

	mLastSeenRT = &target;
}
void StateMachine::drawUI(sf::RenderTarget& target)
{
	if (mCurState)
		mCurState->drawUI(target);

	mLastSeenRT = &target;
}

void StateMachine::primeRT(sf::RenderTarget* rt)
{
	mLastSeenRT = rt;
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
		mCurState->exit(mLastSeenRT);

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
		to->enter(mLastSeenRT);
	}

	if (it == mOldStates.end())
		mOldStates.push_back(to);

	mCurState = to;
}