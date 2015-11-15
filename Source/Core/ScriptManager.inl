#pragma once

template<typename T>
void ScriptManager::CSimpleType<T>::Store(CSerializedValue *val, void *ptr)
{
	val->SetUserData(new T(*reinterpret_cast<T*>(ptr)));
}
template<typename T>
void ScriptManager::CSimpleType<T>::Restore(CSerializedValue *val, void *ptr)
{
	auto buffer = reinterpret_cast<T*>(val->GetUserData());
	*reinterpret_cast<T*>(ptr) = *buffer;
}
template<typename T>
void ScriptManager::CSimpleType<T>::CleanupUserData(CSerializedValue *val)
{
	auto buffer = reinterpret_cast<T*>(val->GetUserData());

	if (buffer)
		delete buffer;

	val->SetUserData(nullptr);
}

template<typename T>
void ScriptManager::registerSerializedType(const std::string& name)
{
	registerSerializedType(name, []() -> CUserType* {
		return new CSimpleType<T>();
	});
}
