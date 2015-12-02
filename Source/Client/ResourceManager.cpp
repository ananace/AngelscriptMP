#include "ResourceManager.hpp"

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