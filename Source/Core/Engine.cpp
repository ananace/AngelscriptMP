#include "Engine.hpp"

Engine::Engine() : 
	mInit(false)
{
}
Engine::Engine(Engine&& rhs) :
	mInit(std::move(rhs.mInit)), mModules(std::move(rhs.mModules))
{

}
Engine::~Engine()
{
	for (auto& it : mModules)
	{
		auto& mod = it.second;
		if (mod.Memory)
		{
			mod.Destructor(mod.Memory);
			mod.Memory = nullptr;
		}
	}
}

void Engine::init()
{
	mInit = true;

	for (auto& it : mModules)
	{
		auto& mod = it.second;
		if (!mod.Memory || mod.Constructor)
		{
			if (mod.Memory)
				mod.Destructor(mod.Memory);
			mod.Memory = mod.Constructor();
			mod.Constructor = nullptr;
		}
	}
}
