// coordinate_iterator_adapter.h

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

// Project 7.2 : Design an adapter type that, given a biderectional bifurcate coordinate
//               type, produces an iterator type that accesses coordinates in a traversal
//               order (pre, in, or post) specified when an iterator is constructed

#pragma once

#include <iterator>

#include "eop.h"
#include "intrinsics.h"
#include "tree.h"
#include "type_functions.h"

namespace eop {

	template<typename C>
		requires(BidirectionalBifurcateCoordinate(C))
	struct coordinate_iterator : public std::iterator<std::forward_iterator_tag, ValueType(C), WeightType(C)>
	{
		C root;
		visit order;
		C current;
		coordinate_iterator(C root, visit order = visit::pre) : root(root), order(order) 
		{
			current = advance_to_first(root, order, root);
		}
		coordinate_iterator<C>(C root, C current, visit order = visit::pre) : root(root), order(order), current(current)
		{}
		coordinate_iterator<C>(const coordinate_iterator<C>& x) = default;
		coordinate_iterator<C>(coordinate_iterator<C>&& x) = default;
		coordinate_iterator<C>& operator=(const coordinate_iterator<C>& x)
		{
			root.ptr = x.root.ptr;
			order = x.order;
			current.ptr = x.current.ptr;
			return *this;
		}
		coordinate_iterator& operator++() 
		{
			current = successor_c(current, order);
			return *this;
		}
		coordinate_iterator operator++(int)
		{
			coordinate_iterator<C> tmp{ *this };
			operator++();
			return tmp;
		}
	};

	template<typename C>
		requires(BidirectionalBifurcateCoordinate(C))
	C advance_to_first(C root, visit order, C current)
	{
		// advance to the first node based on the visit type
		if (empty(root)) return root;
		if (order == visit::pre) return root;
		visit v = visit::pre;
		do {
			traverse_step(current, v);
			if (v == order) return current;
		} while (!has_predecessor(current) || v != visit::post);
		return C{ 0 }; // we've reached end(...)
	}

	template<typename C>
		requires(BidirectionalBifurcateCoordinate(C))
	coordinate_iterator<C> begin(C c, visit order)
	{
		return coordinate_iterator<C>(c, order);
	}

	template<typename T>
		requires(Regular(T))
	coordinate_iterator<tree_coordinate<T>> begin(tree_coordinate<T> c, visit order = visit::pre)
	{
		return coordinate_iterator<tree_coordinate<T>>(c, order);
	}

	template<typename C>
		requires(BidirectionalBifurcateCoordinate(C))
	coordinate_iterator<C> end(C c, visit order)
	{
		return coordinate_iterator<C>(c, C{ 0 }, order);
	}

	template<typename T>
		requires(Regular(T))
	coordinate_iterator<tree_coordinate<T>> end(tree_coordinate<T> c, visit order = visit::pre)
	{
		return coordinate_iterator<tree_coordinate<T>>(c, tree_coordinate<T>{ 0 }, order);
	}

	template<typename C>
		requires(BidirectionalBifurcateCoordinate(C))
	ValueType(C) source(const coordinate_iterator<C>& c)
	{
		return source(c.current);
	}

	template<typename C>
		requires(BidirectionalBifurcateCoordinate(C))
	ValueType(C) operator*(const coordinate_iterator<C>& c)
	{
		return source(c);
	}

	template<typename C>
		requires(BidirectionalBifurcateCoordinate(C))
	bool empty(const coordinate_iterator<C>& c)
	{
		return empty(c.current);
	}

	template<typename C>
		requires(BidirectionalBifurcateCoordinate(C))
	coordinate_iterator<C> successor(coordinate_iterator<C> x)
	{
		return ++x;
	}

	template<typename C>
		requires(BidirectionalBifurcateCoordinate(C))
	struct value_type<coordinate_iterator<C>>
	{
		typedef ValueType(C) type;
	};

	template<typename C>
		requires(BidirectionalBifurcateCoordinate(C))
	struct distance_type<coordinate_iterator<C>>
	{
		typedef WeightType(C) type;
	};

	template<typename C>
		requires(BidirectionalBifurcateCoordinate(C))
	bool operator==(const coordinate_iterator<C>& x, const coordinate_iterator<C>& y)
	{
		return x.root == y.root && x.order == y.order && x.current == y.current;
	}

	template<typename C>
		requires(BidirectionalBifurcateCoordinate(C))
	bool operator!=(const coordinate_iterator<C>& x, const coordinate_iterator<C>& y)
	{
		return !(x == y);
	}

	template<typename C>
		requires(BidirectionalBifurcateCoordinate(C))
	WeightType(C) distance(coordinate_iterator<C> f, coordinate_iterator<C> l)
	{
		typedef WeightType(C) N;
		N n{ 0 };
		while (f != l) {
			f = successor(f);
			n = successor(n);
		}
		return n;
	}

	template<typename C>
		requires(BidirectionalBifurcateCoordinate(C))
	coordinate_iterator<C> advance(coordinate_iterator<C> i, WeightType(C) n)
	{
		while (!zero(n)) {
			i = successor(i);
			n = predecessor(n);
		}
		return i;
	}

} // namespace eop
