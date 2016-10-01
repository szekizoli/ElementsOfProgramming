// type_functions.h

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

namespace eop {

	template< typename T, int i >
		requires(FunctionalProcedure(T))
	struct input_type;

#define InputType(T, i) typename input_type<T, i>::type

#define Domain(T) InputType(T, 0)

	// Binary Operation
	template <typename T>
	requires(Regular(T))
		struct input_type<T(*) (T x, T y), 0> {
		typedef T type;
	};

	template <typename T>
	requires(Regular(T))
		struct input_type<T(*) (const T& x, const T& y), 0> {
		typedef T type;
	};

	// Binary accumulation
	template <typename T>
	requires(Regular(T))
		struct input_type<void(*) (T& x, T y), 0> {
		typedef T type;
	};

	// Unary Function
	template <typename T>
	requires(Regular(T))
		struct input_type<T(*) (T x), 0> {
		typedef T type;
	};

	template <typename T>
	requires(Regular(T))
		struct input_type<T(*) (const T& x), 0> {
		typedef T type;
	};

	template <typename T>
	requires(Regular(T))
		struct input_type<T, 0> {
		typedef typename T::first_argument_type type;
	};

	// Chapter 2

	// DistanceType : Transformation -> Integer
	template<typename F>
	requires(Transformation(F))
		struct distance_type;

	template<>
	struct distance_type<int> {
		typedef int type;
	};

	template<typename T>
	struct distance_type
	{
		typedef typename T::difference_type type;
	};

#define DifferenceType(T) typename T::difference_type
#define DistanceType(T) typename distance_type<T>::type

// ValueType
template<typename T>
	requires(Readable(T))
struct value_type;

template<typename T>
	requires(Readable(T))
struct value_type 
{
	typedef T type;
};

#define ValueType(T) typename T::value_type
//#define ValueType(T) typename eop::value_type<T>::type;


	// Chapter 7

	// Weight Type : BifurcateCoordinate -> int
	template<typename T>
	requires(WeakBifurcateCoordinate(T))
		struct weight_type;

#define WeightType(T) typename weight_type<T>::type

	template<typename T>
		requires(Readable(T))
	ValueType(T) source(T x) {
		return *x;
	}

	// CoordinateType
	template<typename T>
		requires(Regular(T))
	struct coordinate_type;
} // namespace eop