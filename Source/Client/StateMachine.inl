#include <typeindex>

template<typename T>
void StateMachine::changeState(bool remove)
{
	IState* toChangeTo = nullptr;
	for (auto& it : mOldStates)
	{
		if (dynamic_cast<T*>(it))
		{
			toChangeTo = it;
			break;
		}
	}

	if (!toChangeTo)
		toChangeTo = new T();

	changeState(toChangeTo, remove);
}
