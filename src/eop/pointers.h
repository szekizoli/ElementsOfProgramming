// pointers.h

// Copyright (c) 2009 Alexander Stepanov and Paul McJones
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without
// fee, provided that the above copyright notice appear in all copies
// and that both that copyright notice and this permission notice
// appear in supporting documentation. The authors make no
// representations about the suitability of this software for any
// purpose. It is provided "as is" without express or implied
// warranty.

#pragma once

#include "intrinsics.h"
#include "type_functions.h"

#include <cstddef> // ptrdiff_t

namespace eop {
	template<typename T>
	T& sink(pointer(T) x)
	{
		return *x;
	}

	template<typename T>
		requires(Readable(T))
	constexpr const T& source(const pointer(T) x)
	{
		return *x;
	}

	template<typename T>
		requires(Readable(T))
	T& source(pointer(T) x) 
	{
		return *x;
	}

	template<typename T>
	struct distance_type<pointer(T)>
	{
		typedef int type;
	};
}