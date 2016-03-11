#pragma once

template<typename T>
ResourceManager::res_ptr<T>::res_ptr() :
	mManager(nullptr), mData(nullptr)
{

}

template<typename T>
ResourceManager::res_ptr<T>::res_ptr(ResourceManager& man, T* data) :
	mManager(&man), mData(data)
{
	
}
template<typename T>
ResourceManager::res_ptr<T>::res_ptr(const res_ptr& ptr) :
	mManager(ptr.mManager), mData(ptr.mData)
{
	mManager->addref(mData);
}
template<typename T>
ResourceManager::res_ptr<T>::res_ptr(res_ptr&& ptr) : 
	mManager(std::move(ptr.mManager)), mData(std::move(ptr.mData))
{
	ptr.mManager = nullptr;
	ptr.mData = nullptr;
}

template<typename T>
ResourceManager::res_ptr<T>::~res_ptr()
{
	if (mData && mManager)
		mManager->release(mData);
}

template<typename T>
ResourceManager::res_ptr<T>& ResourceManager::res_ptr<T>::operator=(const res_ptr& ptr)
{
	if (&ptr == this)
		return *this;

	if (mManager && mData)
		mManager->release(mData);
	
	mManager = ptr.mManager;
	mData = ptr.mData;

	if (mManager && mData)
		mManager->addref(mData);

	return *this;
}

template<typename T>
inline T* ResourceManager::res_ptr<T>::get()
{
	auto data = mData;
	mData = nullptr;

	return data;
}

template<typename T>
inline ResourceManager::res_ptr<T>::operator T*()
{
	return mData;
}
template<typename T>
inline ResourceManager::res_ptr<T>::operator bool()
{
	return mManager && mData;
}

template<typename T>
inline T& ResourceManager::res_ptr<T>::operator*()
{
	return *mData;
}
template<typename T>
inline const T& ResourceManager::res_ptr<T>::operator*() const
{
	return *mData;
}
template<typename T>
inline T* ResourceManager::res_ptr<T>::operator->()
{
	return mData;
}

template<typename T>
inline const T* ResourceManager::res_ptr<T>::operator->() const
{
	return mData;
}

template<typename T>
ResourceManager::res_ptr<T> ResourceManager::get(const std::string& name)
{
	if (mResources.count(name) == 0)
	{
		T* resource = new T();
		
		if (!loadResource(resource, name))
		{
			delete resource;
			throw new std::runtime_error("Failed to load " + name);
		}

		auto& res = mResources[name];
		res.Counter = 0;
		res.Destructor = [](void* mem) {
			delete (T*)mem;
		};
		res.Memory = resource;
	}

	auto& res = mResources.at(name);
	res_ptr<T> ptr(*this, reinterpret_cast<T*>(res.Memory));
	++res.Counter;

	return ptr;
}
