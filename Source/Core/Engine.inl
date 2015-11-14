#pragma once

template<typename T, typename... Args>
void Engine::add(Args... args)
{
	if (has<T>())
		return;
	Module toAdd;

	if (mInit)
		toAdd = { nullptr, [](void* mem) { delete (T*)mem; }, new T(args...) };
	else
		toAdd = {
		    [args...]() { return new T(args...); },
		    [](void* mem) { delete (T*)mem; }
		};

	mModules[typeid(T)] = toAdd;
}
template<typename T>
T& Engine::get() const
{
    return mModules.at(typeid(T));
}
template<typename T>
bool Engine::has() const
{
    return mModules.count(typeid(T)) > 0;
}
