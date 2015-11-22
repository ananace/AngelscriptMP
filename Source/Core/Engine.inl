#pragma once

template<typename T, typename... Args>
void Engine::add(Args... args)
{
	if (has<T>())
		return;

	Module toAdd = {};
	toAdd.Destructor = [](void* mem) { delete (T*)mem; };

	if (mInit)
		toAdd.Memory = new T(std::forward<Args>(args)...);
	else
		toAdd.Constructor = [=]() { return new T(args...); };

	mModules[typeid(T)] = toAdd;
}

template<typename T>
T& Engine::get() const
{
    return *(T*)(mModules.at(typeid(T)).Memory);
}

template<typename T>
bool Engine::has() const
{
    return mModules.count(typeid(T)) > 0;
}
