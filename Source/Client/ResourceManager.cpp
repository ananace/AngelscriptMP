#include "ResourceManager.hpp"

#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::addref(void* data)
{
	for (auto& it : mResources)
		if (it.second.Memory == data)
		{
			++it.second.Counter;
			break;
		}
}

void ResourceManager::release(void* data)
{
	for (auto it = mResources.begin(); it != mResources.end(); ++it)
		if (it->second.Memory == data)
		{
			if (--it->second.Counter == 0)
			{
				it->second.Destructor(it->second.Memory);
				mResources.erase(it);
			}

			break;
		}
}

template<>
bool ResourceManager::loadResource<sf::Texture>(sf::Texture* res, const std::string& file)
{
	return res->loadFromFile(file);
}
template<>
bool ResourceManager::loadResource<sf::SoundBuffer>(sf::SoundBuffer* res, const std::string& file)
{
	return res->loadFromFile(file);
}
template<>
bool ResourceManager::loadResource<sf::Music>(sf::Music* res, const std::string& file)
{
	return res->openFromFile(file);
}
