#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <unordered_map>

namespace sf
{
	class Font;
	class Music;
	class SoundBuffer;
	class Shader;
	class Texture;
}

class ScriptManager;
class ResourceManager
{
public:
	template<typename T>
	class res_ptr
	{
	public:
		res_ptr();
		res_ptr(const res_ptr& ptr);
		res_ptr(res_ptr&& ptr);
		~res_ptr();

		res_ptr& operator=(const res_ptr& ptr);

		T* get();
		explicit operator T*();

		operator bool();

		T& operator*();
		const T& operator*() const;
		T* operator->();
		const T* operator->() const;


	private:
		res_ptr(ResourceManager& man, T* data);

		ResourceManager* mManager;
		T* mData;

		friend class ResourceManager;
	};

	ResourceManager();
	ResourceManager(const ResourceManager&) = delete;
	~ResourceManager();

	ResourceManager& operator=(const ResourceManager&) = delete;

	template<typename T>
	res_ptr<T> get(const std::string& name);

	void addref(void* data);
	void release(void* data);

	void registerScript(ScriptManager& man);

	typedef res_ptr<sf::Font> Font;
	typedef res_ptr<sf::Music> Music;
	typedef res_ptr<sf::SoundBuffer> Sound;
	typedef res_ptr<sf::Texture> Texture;
	typedef res_ptr<sf::Shader> Shader;

private:
	struct Resource
	{
		std::atomic_int Counter;
		std::function<void(void*)> Destructor;

		void* Memory;
	};

	template<typename T>
	bool loadResource(T* res, const std::string& file);

	std::unordered_map<std::string, Resource> mResources;
};

#include "ResourceManager.inl"
