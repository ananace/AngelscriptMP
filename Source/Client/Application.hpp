#pragma once

#include <Core/ScriptManager.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

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
	void eventThread();
	void drawThread();
	void updateThread();

	ScriptManager mMan;
	sf::RenderWindow mWindow;
};
