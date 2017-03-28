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

#include <initializer_list>

#include "intrinsics.h"
#include "pointers.h"


namespace eop {

	template<typename T>
		requires(Regular(T))
	struct slist_node 
	{
		typedef T value_type;
		typedef pointer(slist_node<T>) Link;
		T value;
		Link forward_link;
		slist_node() : forward_link(0) {}
		slist_node(T value, Link s = 0) : value(value), forward_link(s) {}
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
	struct slist_iterator
	{
		typedef T value_type;
		typedef int weight_type;
		pointer(slist_node<T>) ptr;
		slist_iterator(pointer(slist_node<T>) ptr = 0) : ptr(ptr) {}
		slist_iterator(slist_iterator<T> const& x) : ptr(x.ptr) {}
	};

	template<typename T>
		requires(Regular(T))
	struct weight_type<slist_iterator<T>>
	{
		typedef int type;
	};

	template<typename T>
		requires(Regular(T))
	struct value_type<slist_iterator<T>>
	{
		typedef T type;
	};

	template<typename T>
		requires(Regular(T))
	struct distance_type<slist_iterator<T>>
	{
		typedef int type;
	};

	template<typename T>
		requires(Regular(T))
	bool empty(slist_iterator<T> t)
	{
		typedef pointer(slist_node<T>) I;
		return t.ptr == I{ 0 };
	}

	template<typename T>
		requires(Regular(T))
	slist_iterator<T> successor(slist_iterator<T> t)
	{
		return slist_iterator<T>(source(t.ptr).forward_link);
	}

	template<typename T>
		requires(Regular(T))
	bool has_successor(slist_iterator<T> t)
	{
		return !empty(successor(t));
	}
	
	template<typename T>
		requires(Regular(T))
	void set_successor(slist_iterator<T> c, slist_iterator<T> s)
	{
		sink(c.ptr).forward_link = s.ptr;
	}

	template<typename T>
		requires(Regular(T))
	bool operator==(slist_iterator<T> const& a, slist_iterator<T> const& b)
	{
		return a.ptr == b.ptr;
	}

	template<typename T>
		requires(Regular(T))
	bool operator!=(slist_iterator<T> const& a, slist_iterator<T> const& b)
	{
		return a.ptr != b.ptr;
	}

	template<typename T>
		requires(Regular(T))
	T const& source(slist_iterator<T> c)
	{
		return source(c.ptr).value;
	}

	template<typename T>
		requires(Regular(T))
	T& sink(slist_iterator<T> c)
	{
		return sink(c.ptr).value;
	}

	template<typename T>
		requires(Regular(T))
	struct initializer_list_coordinate
	{
		typedef typename std::initializer_list<T> List;
		typedef typename std::initializer_list<T>::iterator Iterator; 
		Iterator first;
		Iterator last;
		initializer_list_coordinate(Iterator f, Iterator l) : first(f), last(l) {}
		initializer_list_coordinate(List const& l) : first(begin(l)), last(end(l)) {}
	};

	template<typename T>
		requires(Regular(T))
	bool empty(initializer_list_coordinate<T> const& c)
	{
		return c.first == c.last;
	}

	template<typename T>
		requires(Regular(T))
	T source(initializer_list_coordinate<T> const& c)
	{
		return source(c.first);
	}

	template<typename T>
		requires(Regular(T))
	typename std::initializer_list<T>::iterator successor(typename std::initializer_list<T>::iterator const& i)
	{
		return i + 1;
	} 

	template<typename T>
		requires(Regular(T))
	initializer_list_coordinate<T> successor(initializer_list_coordinate<T> const& c)
	{
		return initializer_list_coordinate<T>(c.first + 1, c.last);
	}

	static int slist_node_count = 0; /* ***** TESTING ***** */

	template<typename T>
		requires(Regular(T))
	struct slist_node_construct 
	{
		typedef initializer_list_coordinate<T> ILC;
		typedef slist_iterator<T> C;
		slist_node_construct() {}
		C operator()(T x, C s = C(0)) const
		{
			++slist_node_count;
			return C(new slist_node<T>(x, s.ptr));
		}
		C operator()(ILC c)    const { return (*this)(source(c), C(0)); }
		C operator()(C c)      const { return (*this)(source(c), C(0)); }
		C operator()(C c, C s) const { return (*this)(source(c), s); }
	};

	template<typename T>
		requires(Regular(T))
	slist_iterator<T> erase_first(slist_iterator<T> i)
	{
		slist_iterator<T> n = successor(i);
		--slist_node_count;
		delete i.ptr;
		return n;
	}

	template<typename T>
		requires(Regular(T))
	void erase_after(slist_iterator<T> i)
	{
		set_successor(i, erase_first(successor(i)));
	}

	template<typename T>
	void erase_all(slist_iterator<T> i)
	{
		while(!empty(i)) i = erase_first(i);
	}

	template<typename C, typename C1, typename Cons>
		requires(Regular(T) && ListNodeConstructor(Cons))
	C list_copy(C1 c)
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
		using I = slist_iterator<T>;
		using Cons = slist_node_construct<T>;
		using ILC =  initializer_list_coordinate<T>;
		I root;
		// default constructor
		slist() : root(0) {}

		// from value constructor
		slist(T x) : root(Cons()(x)) {}

		// copy constructor
		slist(const slist& x) : root(list_copy<I, I, Cons>(x.root)) {}

		// list-initialization
		slist(std::initializer_list<T> l) : root(list_copy<I, ILC, Cons>(ILC(l))) {}

		// move constructor
		slist(slist&& x) : root(x.root) 
		{
			x.root = 0;
		}

		// append to head
		slist(T x, const slist& l) : root(Cons()(x)) 
		{
			set_successor(root, list_copy<I, I, Cons>(l.root));
		}

		// desctructor
		~slist()
		{
			erase_all(root);
		}
	};

	template<typename T>
		requires(Regular(T))
	struct coordinate_type<slist<T>>
	{
		typedef slist_iterator<T> type;
	};

	template<typename T>
		requires(Regular(T))
	slist_iterator<T> begin(slist<T> const& x) { return slist_iterator<T>(x.root); }

	template<typename T>
		requires(Regular(T))
	slist_iterator<T> end(slist<T> const& x)  { return slist_iterator<T>(); }

	// double-linked list
	template<typename T>
		requires(Regular(T))
	struct list_node
	{
		typedef T value_type;
		typedef pointer(T) Link;
		T value;
		Link forward_link;
		Link predecessor_link;
		// default constructor
		list_node() : forward_link(0), predecessor_link(0) {}
		list_node(T x, Link s_link = 0, Link p_link = 0) : value(x),
			forward_link(s_link), predecessor_link(p_link) {}
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
