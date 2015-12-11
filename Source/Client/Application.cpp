#include "Application.hpp"
#include "ResourceManager.hpp"

#include <Core/Time.hpp>
#include <Core/FileWatcher.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Sleep.hpp>

#include <Core/AS_Addons/scriptarray/scriptarray.h>
#include <Core/AS_Addons/scriptmath/scriptmath.h>
#include <Core/AS_Addons/scriptstdstring/scriptstdstring.h>
#include <Core/AS_SFML/AS_SFML.hpp>

#include <codecvt>
#include <iostream>
#include <sstream>

namespace
{
	struct CArrayType : public CUserType
	{
		void Store(CSerializedValue *val, void *ptr)
		{
			CScriptArray *arr = (CScriptArray*)ptr;
			for (unsigned int i = 0; i < arr->GetSize(); i++)
				val->m_children.push_back(new CSerializedValue(val, "", "", arr->At(i), arr->GetElementTypeId()));
		}
		void Restore(CSerializedValue *val, void *ptr)
		{
			CScriptArray *arr = (CScriptArray*)ptr;
			arr->Resize(val->m_children.size());
			for (size_t i = 0; i < val->m_children.size(); ++i)
				val->m_children[i]->Restore(arr->At(i), arr->GetElementTypeId());
		}
	};
	/*
	struct CGridType : public CUserType
	{
		void Store(CSerializedValue *val, void *ptr)
		{
			CScriptGrid *grid = (CScriptGrid*)ptr;
			val->SetUserData(new unsigned int(grid->GetWidth()));

			for (unsigned int x = 0; x < grid->GetWidth(); ++x)
				for (unsigned int y = 0; y < grid->GetHeight(); ++y)
					val->m_children.push_back(new CSerializedValue(val, "", "", grid->At(x, y), grid->GetElementTypeId()));
		}
		void Restore(CSerializedValue *val, void *ptr)
		{
			CScriptGrid *grid = (CScriptGrid*)ptr;
			unsigned int width = *(unsigned int*)val->GetUserData();
			if (width == 0)
				return;

			grid->Resize(width, val->m_children.size() / width);
			for (size_t i = 0; i < val->m_children.size(); ++i)
				val->m_children[i]->Restore(grid->At(i / width, i % width), grid->GetElementTypeId());
		}
		void CleanupUserData(CSerializedValue *val)
		{
			unsigned int *buffer = (unsigned int*)val->GetUserData();
			delete buffer;
		}
	};
	*/

	template<typename T>
	void print(T in)
	{
		std::cout << in;
	}

#ifdef WIN32
	template<>
	void print(const std::string& str)
	{
		typedef std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
		std::wcout << convert().from_bytes(str);
	}
#endif

	void printany(const void* ref, int type)
	{
		asIScriptContext* ctx = asGetActiveContext();
		asIObjectType* objType = ctx->GetEngine()->GetObjectTypeById(type);
		asIScriptFunction* func = objType->GetMethodByDecl("string ToString() const");
		std::string output;

		if (func)
		{
			asIScriptContext* callCtx = ctx->GetEngine()->RequestContext();
			callCtx->Prepare(func);
			callCtx->SetObject(const_cast<void*>(ref));
			callCtx->Execute();

			output = *reinterpret_cast<std::string*>(callCtx->GetReturnAddress());

			ctx->GetEngine()->ReturnContext(callCtx);
		}
		else
		{
			std::ostringstream oss;

			if (type & asTYPEID_OBJHANDLE)
				oss << "@";

			oss << objType->GetName();

			if (type & asTYPEID_SCRIPTOBJECT)
				oss << "[SO]";

			oss << "{" << ref << "}";

			output = oss.str();
		}

		std::cout << output;
	}

	template<typename T>
	void println(T in)
	{
		std::cout << in << std::endl;
	}

#ifdef WIN32
	template<>
	void println(const std::string& str)
	{
		typedef std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
		std::wcout << convert().from_bytes(str) << std::endl;
	}
#endif

	void printlnany(void* ref, int type)
	{
		printany(ref, type);
		std::cout << std::endl;
	}

	void println()
	{
		std::cout << std::endl;
	}
}

Application::Application()
{
	auto dur = Time::getClockPrecision();

	std::cout << Clock::now() << std::endl;
	std::cout << "Clock precision seems to be ~" << dur << std::endl;

	if (dur > std::chrono::milliseconds(2))
		std::cout << "This might lead to problems running the application on your platform..." << std::endl;

	mEngine.add<ScriptManager>();
	mEngine.add<sf::RenderWindow>();
	mEngine.add<FileWatcher>();
	mEngine.add<ResourceManager>();
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
	man.registerSerializedType("array", []() {
		static CArrayType ser;
		return &ser;
	});
	man.addExtension("Math", RegisterScriptMath);
	man.addExtension("String", RegisterStdString);
	man.addExtension("String Utils", RegisterStdStringUtils);
	man.addExtension("Printing", [](asIScriptEngine* eng) {
		AS_ASSERT(eng->RegisterGlobalFunction("void print(int)", asFUNCTION(print<int>), asCALL_CDECL));
		AS_ASSERT(eng->RegisterGlobalFunction("void print(float)", asFUNCTION(print<float>), asCALL_CDECL));
		AS_ASSERT(eng->RegisterGlobalFunction("void print(string&in)", asFUNCTION(print<const std::string&>), asCALL_CDECL));
		AS_ASSERT(eng->RegisterGlobalFunction("void print(const ?&in)", asFUNCTION(printany), asCALL_CDECL));
		AS_ASSERT(eng->RegisterGlobalFunction("void println()", asFUNCTIONPR(println, (), void), asCALL_CDECL));
		AS_ASSERT(eng->RegisterGlobalFunction("void println(int)", asFUNCTION(println<int>), asCALL_CDECL));
		AS_ASSERT(eng->RegisterGlobalFunction("void println(float)", asFUNCTION(println<float>), asCALL_CDECL));
		AS_ASSERT(eng->RegisterGlobalFunction("void println(string&in)", asFUNCTION(println<const std::string&>), asCALL_CDECL));
		AS_ASSERT(eng->RegisterGlobalFunction("void println(const ?&in)", asFUNCTION(printlnany), asCALL_CDECL));
	});
	Time::registerTimeTypes(man);
	as::SFML::registerTypes(man);

	// TODO: Register application classes for scripts
	
	man.init();

	// Load scripts;
	std::list<std::string> files;
	FileWatcher::recurseDirectory(".", files, "*.as");

	for (auto& entry : files)
	{
		if (entry.substr(entry.length() - 3) != ".as")
			continue;

		man.loadFromFile(entry);
	}

	auto& watch = mEngine.get<FileWatcher>();

	watch.addSource(".", true);

	auto end = Clock::now();
	std::cout << "Init took " << (end - beg) << std::endl;
}

void Application::run()
{
	std::cout << "Application started in " << Time::getRunTime() << std::endl;

	sf::Event ev;
	std::string modified;
	auto& window = mEngine.get<sf::RenderWindow>();
	auto& watch = mEngine.get<FileWatcher>();

	window.create({ 800, 600 }, "AngelscriptMP Client");
	sf::View uiView = window.getDefaultView(), gameView({}, { 0, 2500 });
	{
		sf::Vector2f size = (sf::Vector2f)window.getSize();
		uiView.setSize(size);
		uiView.setCenter(size / 2.f);

		gameView.setSize(gameView.getSize().y * (size.x / size.y), gameView.getSize().y);
	}

	const Timespan tickLength = std::chrono::milliseconds(15);
	Timespan tickTime;

	auto framepoint = Clock::now();

	while (window.isOpen())
	{
		auto point = Clock::now();
		Timespan dt = point - framepoint;
		framepoint = point;
		tickTime += dt;

		if (watch.pollChange(modified))
		{
			std::cout << modified << " was modified!" << std::endl;
			if (modified.substr(modified.length() - 3) == ".as")
			{
				auto& man = mEngine.get<ScriptManager>();
				man.loadFromFile(modified);
			}
		}


		if (window.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				window.close();
			else if (ev.type == sf::Event::Resized)
			{
				sf::Vector2f size = (sf::Vector2f)window.getSize();
				uiView.setSize(size);
				uiView.setCenter(size / 2.f);

				gameView.setSize(gameView.getSize().y * (size.x / size.y), gameView.getSize().y);
			}
			else
			{
				if (ev.type == sf::Event::KeyPressed ||
					ev.type == sf::Event::KeyReleased)
				{
					bool pressed = ev.type == sf::Event::KeyPressed;
					
					// man.runHook<void, bool>("Key."
				}
			}

			// TODO: Pass events
			// FIXME: How?
		}

		while (tickTime >= tickLength)
		{
			// Run fixed updates
			// man.runHook<void, const Timespan&>("Tick", tickLength);

			tickTime -= tickLength;
		}

		// Run per-frame updates
		// man.runHook<void, const Timespan&>("Update", dt);

		window.clear();

		window.setView(gameView);

		// Draw things
		// man.runHook<void, sf::RenderTarget*>("Draw", &window);

		gameView = window.getView();
		window.setView(uiView);

		// Draw things
		// man.runHook<void, sf::RenderTarget*>("DrawUI", &window);

		window.display();
	}
}

sf::RenderTarget& Application::getRT()
{
	return mEngine.get<sf::RenderWindow>();
}
