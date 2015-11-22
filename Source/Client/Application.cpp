#include "Application.hpp"
#include <Core/Time.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Sleep.hpp>

#include <Core/AS_Addons/scriptarray/scriptarray.h>
#include <Core/AS_Addons/scriptmath/scriptmath.h>
#include <Core/AS_Addons/scriptstdstring/scriptstdstring.h>
#include <Core/AS_SFML/AS_SFML.hpp>

#include <iostream>

Application::Application()
{
	auto dur = Time::getClockPrecision();

	std::cout << Clock::now() << std::endl;
	std::cout << "Clock precision seems to be ~" << dur << std::endl;

	if (dur > std::chrono::milliseconds(2))
		std::cout << "This might lead to problems running the application on your platform..." << std::endl;

	mEngine.add<ScriptManager>();
	mEngine.add<sf::RenderWindow>();
}

Application::~Application()
{
}

void Application::init()
{
	auto beg = Clock::now();
	mEngine.init();

	auto& man = mEngine.get<ScriptManager>();

	man.addExtension("Array", [](asIScriptEngine* eng) { RegisterScriptArray(eng, true); });
	man.addExtension("Math", RegisterScriptMath);
	man.addExtension("String", RegisterStdString);
	man.addExtension("String Utils", RegisterStdStringUtils);
	Time::registerTimeTypes(man);
	as::SFML::registerTypes(man);

	// TODO: Register application classes

	man.init();

	auto end = Clock::now();
	std::cout << "Init took " << (end - beg) << std::endl;

	mEngine.get<sf::RenderWindow>().create({ 800, 600 }, "AngelscriptMP Client");
}

void Application::run()
{
	std::cout << "Application started in " << Time::getRunTime() << std::endl;

	sf::Event ev;
	auto& window = mEngine.get<sf::RenderWindow>();

	while (window.isOpen())
	{
		if (window.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		window.display();
	}
}

sf::RenderTarget& Application::getRT()
{
	return mEngine.get<sf::RenderWindow>();
}
