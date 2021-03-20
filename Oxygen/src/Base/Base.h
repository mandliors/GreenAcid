#pragma once

#include <iostream>
#include <assert.h>

#ifdef OXYGEN_DEBUG
#define OX_ASSERT(condition, message) \
		if (!(condition)) \
		{ \
			std::cerr << "Assertion '" #condition "' failed in " << __FILE__ << " line " << __LINE__ << ": " << message << std::endl; \
				std::terminate(); \
		}
#else
#define OX_ASSERT(condition, message)
#endif

template <typename T>
using Pointer = std::shared_ptr<T>;
template <typename T, typename ... Args>
constexpr Pointer<T> MakePointer(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}