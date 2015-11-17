#pragma once

#include <Core/ScriptManager.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include <atomic>

class Application
{
public:
	Application();
	Application(const Application&) = delete;
	~Application();

	Application& operator=(const Application&) = delete;

	void init();

	void run();

	sf::RenderTarget& getRT();

private:
	ScriptManager mMan;
	sf::RenderWindow mWindow;
};
