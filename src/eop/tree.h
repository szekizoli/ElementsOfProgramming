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
		stree_node(stree_node&& x) : value(x.value),
			left_successor_link(x.left_successor_link),
			right_successor_link(x.right_successor_link)
		{
			x.left_successor_link = 0;
			x.right_successor_link = 0;
		}
		stree_node& operator=(const stree_node& x)
		{
			value = x.value;
			left_successor_link = x.left_successor_link;
			right_successor_link = x.right_successor_link;
		}
		stree_node& operator=(stree_node&& x)
		{
			if (this != &x)
			{
				value = x.value;
				left_successor_link = x.left_successor_link;
				right_successor_link = x.right_successor_link;

				x.left_successor_link = 0;
				x.right_successor_link = 0;
			}
			return *this;
		}
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
		return source(t.ptr).value;
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
		return t.ptr->value;
	}

	template<typename T>
		requires(Regular(T))
	struct stree_node_construct {
		typedef typename stree_coordinate<T> C;
		stree_node_construct() {}
		C operator()(T x, C l = C(0), C r = C(0))
		{
			return C(new stree_node<T>(T, l.ptr, r.ptr));
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

} // namespace eop