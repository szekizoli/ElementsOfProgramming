// intrinsics.h

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

#include <new> // placement operator new

// As explained in Appendix B.2, to allow the language defined in Appendix B.1
// to compile as a valid C++ program, a few macros and structure
// definitions are necessary.


// Template constraints

//  The requires clause is implemented with this macro (this
//  implementation treats requirements as documentation only):

#define requires(...)

#define Arity(...)
#define BidirectionalBifurcateCoordinate(...)
#define BidirectionalIterator(...)
#define BifurcateCoordinate(...)
#define BinaryOperation(...)
#define ForwardIterator(...)
#define Integer(...)
#define IndexedIterator(...)
#define Iterator(...)
#define Procedure(...)
#define Readable(...)
#define Relation(...)
#define Regular(...)
#define Transformation(...)
#define UnaryPredicate(...)

// Intrinsics

//  pointer(T) and addressof(x) are introduced to give us a simple
//  linear notation and allow simple top-down parsing. They are
//  implemented as:

#define pointer(T) T*

template<typename T>
pointer(T) addressof(T& x)
{
	return &x;
}
