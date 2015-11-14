#include "Engine.hpp"

Engine::Engine()
{
}
Engine::Engine(Engine&& rhs) :
	mModules(std::move(rhs.mModules))
{

}
Engine::~Engine()
{
	for (auto& it : mModules)
	{
		auto& mod = it.second;
		if (mod.Destructor && mod.Memory)
		{
			mod.Destructor(mod.Memory);
			mod.Memory = nullptr;
		}
	}
}

void Engine::init()
{
	for (auto& it : mModules)
	{
		auto& mod = it.second;
		if (mod.Memory || !mod.Constructor)
		{
			mod.Memory = mod.Constructor();
			mod.Constructor = nullptr;
		}
	}
}
