// tree.h (originally in eop.h)

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

// The origin of the contents of this file is
// http://www.elementsofprogramming.com/code/eop.h

#pragma once

#include <stack>

#include "intrinsics.h"
#include "pointers.h"
#include "type_functions.h"

namespace eop {

	template<typename T>
		requires(Regular(T))
	struct stree_node
	{
		typedef typename T value_type;
		typedef pointer(stree_node<T>) Link;
		T value;
		Link left_successor_link;
		Link right_successor_link;
		stree_node() : left_successor_link(0), right_successor_link(0) {}
		stree_node(T value, Link l = 0, Link r = 0) :
			value(value),
			left_successor_link(l), right_successor_link(r) {}
		stree_node(const stree_node& x) = default;
	};

	template<typename T>
		requires(Regular(T))
	struct stree_coordinate
	{
		typedef typename T value_type;
		typedef typename int weight_type;
		typedef typename int difference_type;
		pointer(stree_node<T>) ptr;
		explicit stree_coordinate(pointer(stree_node<T>) ptr = 0) : ptr(ptr) {}
	};

	template<typename T>
		requires(Regular(T))
	struct weight_type< stree_coordinate<T> >
	{
		typedef int type;
	};

	template<typename T>
		requires(Regular(T))
	struct value_type< stree_coordinate<T> >
	{
		typedef T type;
	};

	template<typename T>
		requires(Regular(T))
	bool empty(stree_coordinate<T> t)
	{
		typedef pointer(stree_node<T>) I;
		return t.ptr == I{ 0 };
	}

	template<typename T>
		requires(Regular(T))
	stree_coordinate<T> left_successor(stree_coordinate<T> t)
	{
		return stree_coordinate<T>(sink(t.ptr).left_successor_link);
	}

	template<typename T>
		requires(Regular(T))
	bool has_left_successor(stree_coordinate<T> t)
	{
		return !empty(left_successor(t));
	}

	template<typename T>
		requires(Regular(T))
	stree_coordinate<T> right_successor(stree_coordinate<T> t)
	{
		return stree_coordinate<T>(sink(t.ptr).right_successor_link);
	}

	template<typename T>
		requires(Regular(T))
	bool has_right_successor(stree_coordinate<T> t)
	{
		return !empty(right_successor(t));
	}

	template<typename T>
		requires(Regular(T))
	void set_left_successor(stree_coordinate<T> c, stree_coordinate<T> l)
	{
		sink(c.ptr).left_successor_link = l.ptr;
	}

	template<typename T>
		requires(Regular(T))
	void set_right_successor(stree_coordinate<T> c, stree_coordinate<T> r)
	{
		sink(c.ptr).right_successor_link = r.ptr;
	}

	template<typename T>
		requires(Regular(T))
	bool operator==(stree_coordinate<T> a, stree_coordinate<T> b)
	{
		return a.ptr == b.ptr;
	}

	template<typename T>
		requires(Regular(T))
	T& source(stree_coordinate<T> t)
	{
		return sink(t.ptr).value;
	}

	template<typename T>
		requires(Regular(T))
	stree_node<T>& sink(pointer(stree_node<T>) t)
	{
		return *t;
	}

	template<typename T>
		requires(Regular(T))
	T& sink(stree_coordinate<T> t)
	{
		return sink(t.ptr).ptrvalue;
	}

	template<typename T>
		requires(Regular(T))
	struct stree_node_construct {
		typedef typename stree_coordinate<T> C;
		stree_node_construct() {}
		C operator()(T x, C l = C(0), C r = C(0))
		{
			return C(new stree_node<T>(x, l.ptr, r.ptr));
		}
		C operator()(C c) {
			return (*this) (source(c), left_successor(c),
				right_successor(c));
		}
		C operator()(C c, C l, C r) { return (*this) (source(c), l, r); }
	};

	template<typename T>
		requires(Regular(T))
	struct stree_node_destroy {
		stree_node_destroy() {}
		void operator()(stree_coordinate<T> c)
		{
			delete c.ptr;
		}
	};

	template<typename C, typename ND>
		requires(BifurcateCoordinate(C) && TreeNodeDeleter(ND))
	void bifurcate_erase(C c, ND node_delete)
	{
		if (empty(c)) return;
		C stack{ 0 }; // chained through left successor
		while (true) {
			C left = left_successor(c);
			C right = right_successor(c);
			if (!empty(left)) {
				if (!empty(right)) {
					set_left_successor(c, stack);
					stack = c;
				}
				else
					node_delete(c);
				c = left;
			}
			else if (!empty(right)) {
				node_delete(c);
				c = right;
			}
			else {
				node_delete(c);
				if (!empty(stack)) {
					c = stack;
					stack = left_successor(stack);
					set_left_successor(c, C{ 0 });
				}
				else return;
			}
		}
	}

	/*
	The next function is based on MAKECOPY in this paper:

	K. P. Lee.
	A linear algorithm for copying binary trees using bounded workspace.
	Commun. ACM 23, 3 (March 1980), 159-162. DOI=10.1145/358826.358835
	http://doi.acm.org/10.1145/358826.358835
	*/

	template<typename C, typename Cons>
	requires(EmptyLinkedBifurcateCoordinate(C) &&
		TreeNodeConstructor(Cons) && NodeType(C) == NodeType(Cons))
	C bifurcate_copy(C c)
	{
		Cons construct_node;
		if (empty(c)) return c;              // Us      / Lee
		C stack = construct_node(c, c, C()); // stack   / V'
		C c_new = stack;                     // c\_new  / COPY
		while (!empty(stack)) {              // empty() / null
			c = left_successor(stack);       // c       / V
			C l = left_successor(c);
			C r = right_successor(c);
			C top = stack;
			if (!empty(l)) {
				if (!empty(r)) {
					r = construct_node(r, r, right_successor(stack));
					stack = construct_node(l, l, r);
				}
				else {
					r = C();
					stack = construct_node(l, l, right_successor(stack));
				}
				l = stack;
			}
			else if (!empty(r)) {
				stack = construct_node(r, r, right_successor(stack));
				r = stack;
			}
			else
				stack = right_successor(stack);
			set_right_successor(top, r);
			set_left_successor(top, l);
		}
		return c_new;
	}

	template<typename T>
		requires(Regular(T))
	struct stree
	{
		typedef stree_coordinate<T> C;
		typedef stree_node_construct<T> Cons;
		C root;
		stree() : root(0) {}
		stree(T x) : root(Cons()(x)) {}
		stree(T x, const stree& left, const stree& right) : root(Cons()(x))
		{
			set_left_successor(root, bifurcate_copy<C, Cons>(left));
			set_right_successor(root, bifurcate_copy<C, Cons>(right));
		}
		stree(const stree& x) : root(bifurcate_copy<C, Cons>(x.root)) {}
		~stree() { bifurcate_erase(root, stree_node_destroy<T>{}); }
		void operator=(stree x) { swap(root, x.root); }
	};

	/*template<typename T>
		requires(Regular(T))
	struct coordinate_type< stree<T> >
	{
		typedef stree_coordinate<T> type;
	};*/
} // namespace eop