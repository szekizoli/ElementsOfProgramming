#pragma once

#include "intrinsics.h"
#include "pointers.h"
#include "eop.h"
#include "tree.h"
#include "type_functions.h"

// Project 7.1 : Implement versions of algorithms in Chapter 6 for 
//               bidirectional bifurcate coordinates.

namespace eop {

	template<typename C>
		requires(BidirectionalBifurcateCoordinate(C))
	C successor_c(C c)
	{
		// Precondition: !empty(c)
		visit v = visit::pre;
		do {
			traverse_step(c, v);
		} while ((v != visit::pre) && (has_predecessor(c) || v != visit::post));
		return v == visit::pre ? c : C{ 0 };
	}

	template<typename C, typename Proc>
	requires(BidirectionalBifurcateCoordinate(C) && Procedure(Proc) && Arity(Proc) == 1 &&
		ValueType(C) == InputType(Proc, 0))
	Proc for_each(C c, Proc proc, visit order = visit::in)
	{
		// Precondition: tree(c)
		if (empty(c)) return proc;
		C root = c;
		visit v = visit::pre;
		if (v == order) proc(source(c));
		do {
			traverse_step(c, v);
			if (v == order) proc(source(c));
		} while (c != root || v != visit::post);
		return proc;
	}

	template<typename C>
		requires(BidirectionalBifurcateCoordinate(C))
	C find(C c, const ValueType(C)& x)
	{
		// Precondition: tree(c)
		while (!empty(c) && source(c) != x) c = successor_c(c);
		return c;
	}

	template<typename C, typename P>
		requires(BidirectionalBifurcateCoordinate(C) &&
				 UnaryPredicate(P) && ValueType(C) == Domain(P))
	C find_if(C c, P p)
	{
		// Precondition: tree(c)
		while (!empty(c) && !p(source(c))) c = successor_c(c);
		return c;
	}

	template<typename C, typename P>
		requires(BidirectionalBifurcateCoordinate(C) &&
				 UnaryPredicate(P) && ValueType(C) == Domain(P))
	C find_if_not(C c, P p)
	{
		// Precondition: tree(c)
		while (!empty(c) && p(source(c))) c = successor_c(c);
		return c;
	}

	template<typename C, typename P>
		requires(BidirectionalBifurcateCoordinate(C) && 
			     UnaryPredicate(P) && ValueType(C) == Domain(P))
	bool all(C c, P p)
	{
		// Precondition: tree(c)
		return empty(find_if_not(c, p));
	}

	template<typename C, typename P>
		requires(BidirectionalBifurcateCoordinate(C) &&
		  		 UnaryPredicate(P) && ValueType(C) == Domain(P))
	bool none(C c, P p)
	{
		// Precondition: tree(c)
		return empty(find_if(c, p));
	}

	template<typename C, typename P>
		requires(BidirectionalBifurcateCoordinate(C) &&
		 		 UnaryPredicate(P) && ValueType(C) == Domain(P))
	bool not_all(C c, P p)
	{
		// Precondition: tree(c)
		return !all(c, p);
	}

	template<typename C, typename P>
		requires(BidirectionalBifurcateCoordinate(C) &&
			 	 UnaryPredicate(P) && ValueType(C) == Domain(P))
	bool some(C c, P p)
	{
		// Precondition: tree(c)
		return !none(c, p);
	}

	template<typename C, typename P, typename J>
		requires(BidirectionalBifurcateCoordinate(C)
			     && UnaryPredicate(P) && ValueType(I) == Domain(P))
	J count_if(C c, P p, J j) {
		// Precondition: tree(c)
		return for_each(c, counter_if<P, J>(p, j)).j;
	}

	template<typename C, typename P>
		requires(BidirectionalBifurcateCoordinate(C)
			&& UnaryPredicate(P) && ValueType(I) == Domain(P))
	WeightType(C) count_if(C c, P p) {
		// Precondition: tree(c)
		return count_if(c, p, WeightType(C){0});
	}

} // namespace eop
