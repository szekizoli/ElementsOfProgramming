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

#include "intrinsics.h"
#include "pointers.h"
//#include "type_functions.h"

namespace eop {

	template<typename T>
		requires(Regular(T))
	struct slist_node 
	{
		typedef T value_type;
		typedef pointer(slist_node<T>) Link;
		T value;
		Link successor_link;
		slist_node() : successor_link(0) {}
		slist_node(T value, Link s = 0) : value(value), successor_link(s) {}
		slist_node(const slist_node& other) = default;
	};

	template<typename T>
		requires(Regular(T))
	struct value_type<slist_node<T>>
	{
		typedef T type;
	};

	template<typename T>
		requires(Regular(T))
	struct slist_coordinate
	{
		typedef T value_type;
		typedef int weight_type;
		pointer(slist_node<T>) ptr;
		slist_coordinate(pointer(slist_node<T>) ptr = 0) : ptr(ptr) {}
	};

	template<typename T>
		requires(Regular(T))
	struct weight_type<slist_coordinate<T>>
	{
		typedef int type;
	};

	template<typename T>
		requires(Regular(T))
	struct value_type<slist_coordinate<T>>
	{
		typedef T type;
	};

	template<typename T>
		requires(Regular(T))
	bool empty(slist_coordinate<T> t)
	{
		typedef pointer(slist_node<T>) I;
		return t.ptr == I{ 0 };
	}

	template<typename T>
		requires(Regular(T))
	slist_coordinate<T> successor(slist_coordinate<T> t)
	{
		return source(t.ptr).successor_link;
	}

	template<typename T>
		requires(Regular(T))
	bool has_successor(slist_coordinate<T> t)
	{
		return !empty<T>(source(t.ptr).successor_link);
	}
	
	template<typename T>
		requires(Regular(T))
	void set_successor(slist_coordinate<T> c, slist_coordinate<T> s)
	{
		source(c.ptr).successor_link = s.ptr;
	}

	template<typename T>
		requires(Regular(T))
	bool operator==(slist_coordinate<T> const& a, slist_coordinate<T> const& b)
	{
		return a.ptr == b.ptr;
	}

	template<typename T>
		requires(Regular(T))
	bool operator!=(slist_coordinate<T> const& a, slist_coordinate<T> const& b)
	{
		return a.ptr != b.ptr;
	}

	template<typename T>
		requires(Regular(T))
	T const& source(slist_coordinate<T> c)
	{
		return source(c.ptr).value;
	}

	template<typename T>
		requires(Regular(T))
	T& sink(slist_coordinate<T> c)
	{
		return sink(c.ptr).value;
	}


	static int slist_node_count = 0; /* ***** TESTING ***** */

	template<typename T>
		requires(Regular(T))
	struct slist_node_construct 
	{
		typedef slist_coordinate<T> C;
		slist_node_construct() {}
		C operator()(T x, C s = C(0)) const
		{
			++slist_node_count;
			return C(new slist_node<T>(x, s.ptr));
		}
		C operator()(C c)      const { return (*this)(source(c), C(0)); }
		C operator()(C c, C s) const { return (*this)(source(c), s); }
	};

	template<typename T>
		requires(Regular(T))
	struct slist_node_destroy
	{
		void operator()(slist_coordinate<T> c)
		{
			--slist_node_count;
			delete c.ptr;
		}
	};

	template<typename C, typename ND>
		requires(Regular(T) && ListNodeDeleter(ND))
	void list_erase(C c, ND node_deleter)
	{
		while (!empty(c))
		{
			C s = successor(c);
			node_deleter(c);
			c = s;
		}
	}

	template<typename C, typename Cons>
		requires(Regular(T) && ListNodeConstructor(Cons))
	C list_copy(C c)
	{
		if (empty(c)) return C(0);
		Cons construct_node;
		C f = construct_node(c);
		C l = f;
		c = successor(c);
		while (!empty(c))
		{
			set_successor(l, construct_node(c));
			l = successor(l);
			c = successor(c);
		}
		return f;
	}

	// singly-linked list
	template<typename T>
		requires(Regular(T))
	struct slist
	{
		using C = slist_coordinate<T>;
		using Cons = slist_node_construct<T>;
		C root;
		// default constructor
		slist() : root(0) {}

		// from value constructor
		slist(T x) : root(Cons()(x)) {}

		// copy constructor
		slist(const slist& x) : root(list_copy<C, Cons>(x.root)) {}

		// move constructor
		slist(slist&& x) : root(x.root) 
		{
			x.root = 0;
		}

		// append to head
		slist(T x, const slist& l) : root(Cons()(x)) 
		{
			set_successor(root, list_copy<C, Cons>(l.root));
		}

		// desctructor
		~slist()
		{
			list_erase(root, slist_node_destroy<T>());
		}
	};

	// double-linked list
	template<typename T>
		requires(Regular(T))
	struct list_node
	{
		typedef T value_type;
		typedef pointer(T) Link;
		T value;
		Link successor_link;
		Link predecessor_link;
		// default constructor
		list_node() : successor_link(0), predecessor_link(0) {}
		list_node(T x, Link s_link = 0, Link p_link = 0) : value(x),
			successor_link(s_link), predecessor_link(p_link) {}
		// copy constructor
		list_node(list_node const& x) = default;
	};

	template<typename T>
		requires(Regular(T))
	struct value_type<list_node<T>>
	{
		typedef T type;
	};

	template<typename T>
		requires(Regular(T))
	struct list_coordinate
	{
		typedef T value_type;
		typedef int weight_type;
		pointer(list_node<T>) ptr;
		list_coordinate(pointer(list_node<T>) ptr = 0) : ptr(ptr) {}
	};

} // namespace eop
