#pragma once
#include <windows.h>

#define USE_API(NAME, API) static auto proc_##API = (decltype(&API))(GetProcAddress(GetModuleHandle(L#NAME), #API))

namespace  base
{
    // template <class Fn>
	// Fn DynamicFunction(const char* module, const char* function)
	// {
	// 	return reinterpret_cast<Fn>(::GetProcAddress(::GetModuleHandleA(module), function));
	// }

} // namespace  base
