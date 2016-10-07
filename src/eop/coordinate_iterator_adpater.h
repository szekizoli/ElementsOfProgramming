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
		coordinate_iterator(C root, visit order = visit::pre, C current = root) : root(root), order(order), current(current) {}

		coordinate_iterator<C> operator++() 
		{
			visit v = order;
			successor_c(current, v);
			return *this;
		}

		coordinate_iterator<C> operator++(int)
		{
			coordinate_iterator<C> r{ root, order, current };
			visit v = order;
			successor_c(current, v);
			return r;
		}

		bool operator==(const coordinate_iterator<C>& x, const coordinate_iterator<C>& y)
		{
			return x.root == y.root && x.order == y.order == x.current == y.current;
		}

		bool operator!=(const coordinate_iterator<C>& x, const coordinate_iterator<C>& y)
		{
			return !(x == y);
		}

	private:
		const C root;
		const visit order;
		C current;
	};

	template<typename C>
		requires(BidirectionalBifurcateCoordinate(C))
	coordinate_iterator<C> begin(C c, visit order == visit::pre)
	{
		return coordinate_iterator(c, order);
	}

	template<typename C>
		requires(BidirectionalBifurcateCoordinate(C))
	coordinate_iterator<C> end(C c, visit order == visit::pre)
	{
		return coordinate_iterator(c, order, C{0});
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
