#pragma once

#include <functional>
#include <typeindex>
#include <unordered_map>

class Engine
{
public:
	Engine();
	Engine(Engine&&);
	Engine(const Engine&) = delete;
	~Engine();

	Engine& operator=(const Engine&) = delete;

	template<typename T, typename... Args>
	void add(Args...);
	template<typename T>
	T& get() const;
	template<typename T>
	bool has() const;

	void init();

private:
	struct Module
	{
		std::function<void*()> Constructor;
		std::function<void(void*)> Destructor;

		void* Memory;
	};

	bool mInit;
	std::unordered_map<std::type_index, Module> mModules;
};

#include "Engine.inl"
