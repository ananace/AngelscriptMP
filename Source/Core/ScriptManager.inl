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

namespace
{
	inline void setCTXArg(asIScriptContext*, uint32_t) { }

#define PRIMITIVE_ARG(Type, SetType) template<typename... Args> \
    inline void setCTXArg(asIScriptContext* ctx, uint32_t id, Type arg, Args... args) \
    { \
        ctx->SetArg ## SetType (id, arg); \
        setCTXArg(ctx, id + 1, args...); \
    } //

	PRIMITIVE_ARG(uint8_t, Byte)
	PRIMITIVE_ARG(uint16_t, Word)
	PRIMITIVE_ARG(uint32_t, DWord)
	PRIMITIVE_ARG(uint64_t, QWord)
	PRIMITIVE_ARG(int8_t, Byte)
	PRIMITIVE_ARG(int16_t, Word)
	PRIMITIVE_ARG(int32_t, DWord)
	PRIMITIVE_ARG(int64_t, QWord)
	PRIMITIVE_ARG(float, Float)
	PRIMITIVE_ARG(double, Double)

#undef PRIMITIVE_ARG

	template<typename T, typename... Args>
	inline void setCTXArg(asIScriptContext* ctx, uint32_t id, T* arg, Args... args)
	{
		ctx->SetArgObject(id, const_cast<T*>(arg));
		setCTXArg(ctx, id + 1, args...);
	}

	template<typename T, typename... Args>
	inline void setCTXArg(asIScriptContext* ctx, uint32_t id, const T& arg, Args... args)
	{
		ctx->SetArgObject(id, const_cast<T*>(&arg));
		setCTXArg(ctx, id + 1, args...);
	}
}

template<typename... Args>
void ScriptManager::runHook(const std::string& name, Args... args)
{
	if (mRegisteredHooks.count(name) == 0 || mScriptHooks.count(name) == 0)
		return;

	auto* ctx = mEngine->RequestContext();

	for (auto& hook : mScriptHooks.at(name))
	{
		int r = 0;
		r = ctx->Prepare(hook.Function);
		if (r < 0)
			continue;
		r = ctx->SetObject(hook.Object);
		if (r < 0)
			continue;

		setCTXArg(ctx, 0, std::forward<Args>(args)...);

		ctx->Execute();

		ctx->Unprepare();
	}

	mEngine->ReturnContext(ctx);
}
