// common.h

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

#define requires(...)

#define Arity(...)
#define BinaryOperation(...)
#define ForwardIterator(...)
#define Integer(...)
#define Iterator(...)
#define Procedure(...)
#define Readable(...)
#define Relation(...)
#define Transformation(...)
#define UnaryPredicate(...)

#define pointer(T) T*

#define ValueType(T) typename eop::value_type<T>::type;

namespace eop {

	template<typename T>
	pointer(T) addressof(T& x) {
		return &x;
	}

	template<typename T>
	struct value_type
	{
		typedef T type;
	};

} // namespace eop