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

#include "common.h"

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

// Chapter 2

// DistanceType : Transformation -> Integer
template<typename F>
	requires(Transformation(F))
struct distance_type;

template<>
struct distance_type<int> {
	typedef int type;
};

#define DistanceType(T) typename distance_type<Domain(T)>::type

// ValueType
template< typename I>
	requires(Readable(I))
struct value_type;

#define ValueType(I) typename I::value_type
