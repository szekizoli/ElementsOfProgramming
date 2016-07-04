﻿// eop.h

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

#include <tuple>
#include <utility>

#include "type_functions.h"

namespace eop {

	// Chapter 1 - Foundations

	template<typename Op>
	requires(BinaryOperation(Op))
	Domain(Op) square(const Domain(Op)& x, Op op) {
		return op(x, x);
	}

	int multiply(int x, int y) {
		return x * y;
	}

	// Chapter 2 - Transformations and Their Orbits

	template<typename F, typename N>
	requires(Transformation(F) && Integer(N))
	Domain(F) power_unary(Domain(F) x, N n, F f) {
		// Precondition: n ≥ 0 ∧ (∀i ∈ N) 0 < i ≤ n => f^i(x) is defined
		while (n != N(0)) {
			n = n - N(1);
			x = f(x);
		}
		return x;
	}

	int increment(const int& x) {
		return x + 1;
	}

	template<typename F>
	requires(Transformation(F))
	DistanceType(F) distance(Domain(F) x, Domain(F) y, F f) {
		// Precondition: y is reachable from x using f
		typedef DistanceType(F) N;
		N n(0);
		while (x != y) {
			x = f(x);
			n = n + N(1);
		}
		return n;
	}

	template<typename F, typename P>
	requires(Transformation(F) && UnaryPredicate(P) &&
		Domain(F) == Domain(P))
	Domain(F) collision_point(const Domain(F)& x, F f, P p) {
		// Precondition : p(x) <=> f(x) is defined
		if (!p(x)) return x;
		Domain(F) slow = x;             // slow = f^0(x)
		Domain(F) fast = f(x);          // fast = f^1(
										// n <- 0 (completed operations)
		while (fast != slow) {          // slow = f^n(x) ? fast = f^(2n+1)(x)
			slow = f(slow);             // slow = f^(n+1)(x) ? fast = f^(2n+1)(x)
			if (!p(fast)) return fast;
			fast = f(fast);             // slow = f^(n+1)(x) ? fast = f^(2n+2)(x)
			if (!p(fast)) return fast;
			fast = f(fast);             // slow = f^(n+1)(x) ? fast = f^(2n+3)(x)
		}                               // n <- n + 1
		return fast;
		// Postcondition : return value is terminal point or collision point
	}

	bool less_than_ten(int x) {
		return x < 10;
	}

	bool always_true(int) {
		return true;
	}

	template<typename F>
	requires(Transformation(F))
	Domain(F) collision_point_nonterminating_orbit(const Domain(F)& x, F f) {
		Domain(F) slow = x;
		Domain(F) fast = f(x);

		while (fast != slow) {
			slow = f(slow);
			fast = f(fast);
			fast = f(fast);
		}
		return fast;
		// Postcondition : return value is collision point
	}

	template<typename F>
	requires(Transformation(F))
	bool circular_nonterminating_orbit(const Domain(F)& x, F f) {
		return x == f(collision_point_nonterminating_orbit(x, f));
	}

	template<typename F, typename P>
	requires(Transformation(F) && UnaryPredicate(P) &&
		Domain(F) == Domain(P))
	bool circular(const Domain(F)& x, F f, P p) {
		Domain(F) y = collision_point(x, f, p);
		return p(y) && x == f(y);
	}

	template<typename F>
	requires(Transformation(F))
	Domain(F) convergent_point(Domain(F) x0, Domain(F) x1, F f) {
		// Precondition : (∃n ∈ DistanceType(F)) n ≥ 0 ∧ f^n(x0) = f^n(x1)
		while (x0 != x1) {
			x0 = f(x0);
			x1 = f(x1);
		}
		return x0;
	}

	template<typename F>
	requires(Transformation(F))
	Domain(F) connection_point_nonterminating_orbit(const Domain(F)& x, F f) {
		return convergent_point(
			x,
			f(collision_point_nonterminating_orbit(x, f)),
			f
			);
	}

	template<typename F, typename P>
	requires(Transformation(F) && UnaryPredicate(P) &&
		Domain(F) == Domain(P))
	Domain(F) connection_point(const Domain(F)& x, F f, P p) {
		// Precondition : p(x) <=> f(x) is defined
		Domain(F) y = collision_point(x, f, p);
		if (!p(y)) return y;
		return convergent_point(x, f(y), f);
	}

	template<typename F>
	requires(Transformation(F))
	std::tuple<DistanceType(F), DistanceType(F), Domain(F)>
	orbit_structure_nonterminating_orbit(const Domain(F)& x, F f) {
		typedef DistanceType(F) N;
		Domain(F) y = connection_point_nonterminating_orbit(x, f);
		return std::make_tuple<N, N, Domain(F)>(distance(x, y ,f),
												distance(f(y), y, f),
												std::move(y));
	}

	template<typename F, typename P>
	requires(Transformation(F) && UnaryPredicate(P) &&
		Domain(F) == Domain(P))
	std::tuple<DistanceType(F), DistanceType(F), Domain(F)>
	orbit_structure(const Domain(F)& x, F f, P p) {
		// Precondition : p(x) <=> f(x) is defined
		typedef DistanceType(F) N;
		Domain(F) y = connection_point(x, f, p);
		N m = distance(x, y, f);
		N n{ 0 };
		if (p(y)) n = distance(f(y), y, f);
		// Terminating : m = h - 1 ∧ n = 0
		// Otherwise: m = h ∧ n = c - 1
		return std::make_tuple<N, N, Domain(F)>(m, n, std::move(y));
	}
	
	// Chapter 3 - Associative Operations

	// Computing powers
	template<typename I, typename Op>
		requires(Integer(I) && BinaryOperation(Op))
	Domain(Op) power_left_associated(Domain(Op) a, I n, Op op) {
		// Precondition : n > 0
		if (n == I{ 1 }) return a;
		return op(power_left_associated(a, n - I{ 1 }, op), a);
	}

	template<typename I, typename Op>
		requires(Integer(I) && BinaryAccumulationProcedure(Op))
	Domain(Op) power_left_associated_accumulative(Domain(Op) a, I n, Op op_accumulate) {
		// Precondition : n > 0
		if (n == I{ 1 }) return a;
		Domain(Op) r = power_left_associated_accumulative(a, n - I{ 1 }, op_accumulate);
		op_accumulate(r, a);
		return r;
	}

	template<typename I, typename Op>
		requires(Integer(I) && BinaryOperation(Op))
	Domain(Op) power_right_associated(Domain(Op) a, I n, Op op) {
		// Precondition : n > 0
		if (n == I{ 1 }) return a;
		return op(a, power_right_associated(a, n - I{ 1 }, op));
	}

	template<typename I, typename Op>
		requires(Integer(I) && BinaryAccumulationProcedure(Op))
	Domain(Op) power_right_associated_accumulative(Domain(Op) a, I n, Op op_accumulate) {
		// Precondition : n > 0
		if (n == I{ 1 }) { return a; };
		op_accumulate(a, power_right_associated_accumulative(a, n - I{ 1 }, op_accumulate));
		return a;
	}

   /*
    *         /  a,              if n is 1
    *  a^n =  |  (a^2)^(n/2),    if n is even
    *         \  (a^2)^( n/2)*a, if n is odd
	*/
	template<typename I, typename Op>
		requires(Integer(I) && BinaryOperation(Op))
	Domain(Op) power_0(Domain(Op) a, I n, Op op) {
		// Precondition : n > 0
		if (n == I{1}) return a;
		if (n % I{2} == 0) {
			return power_0(op(a, a), n / I{ 2 }, op);
		}
		return op(a, power_0(op(a, a), n / I{ 2 }, op));
	}

	// Applying common subexpression elimination
	template<typename I, typename Op>
	requires(Integer(I) && BinaryOperation(Op))
		Domain(Op) power_1(Domain(Op) a, I n, Op op) {
		// Precondition : n > 0
		if (n == I{ 1 }) return a;
		Domain(Op) r = power_1(op(a, a), n / I{ 2 }, op);
		if (n % I{ 2 } != I{ 0 } ) { r = op(r, a); }
		return r;
	}

	// Makinng power tail recursive

    // Introducing accumulation variable
	// If r0, a0 and n0 are the original values of r, a and n
	// r0*a0^n0 == r*a^n at every recursive call
	template<typename I, typename Op>
		requires(Integer(I) && BinaryOperation(Op))
	Domain(Op) power_accumulate_0(Domain(Op) a, Domain(Op) r, I n, Op op) {
		// Precondition : associative(op) ∧ n ≥ 0
		if (n == I{ 0 }) return r;
		if (n % I{ 2 } != I{ 0 } ) { r = op(r, a); }
		return power_accumulate_0(op(a, a), r, n / I{ 2 }, op);
	}

	// Elimintating the one unnecessary squaring going from 1 to 0
	template<typename I, typename Op>
	requires(Integer(I) && BinaryOperation(Op))
		Domain(Op) power_accumulate_1(Domain(Op) a, Domain(Op) r, I n, Op op) {
		// Precondition : associative(op) ∧ n ≥ 0
		if (n == I{ 0 }) return r;
		if (n == I{ 1 }) return op(r, a);
		if (n % I{ 2 } != I{ 0 }) { r = op(r, a); }
		return power_accumulate_1(op(a, a), r, n / I{ 2 }, op);
	}

	// Reordering tests based on expected frequencies
	template<typename I, typename Op>
	requires(Integer(I) && BinaryOperation(Op))
		Domain(Op) power_accumulate_2(Domain(Op) a, Domain(Op) r, I n, Op op) {
		// Precondition : associative(op) ∧ n ≥ 0
		if (n % I{ 2 } != I{ 0 }) {
			r = op(r, a);
			if (n == I{ 1 }) return r;
		} else if (n == I{ 0 }) return r;
		
		return power_accumulate_2(op(a, a), r, n / I{ 2 }, op);
	}

	// A strict-tail-recursive procedure is one which all the tail-recursive calls are done
	// with the formal parameters of the procedure being corresponding to the arguments
	template<typename I, typename Op>
		requires(Integer(I) && BinaryOperation(Op))
	Domain(Op) power_accumulate_3(Domain(Op) a, Domain(Op) r, I n, Op op) {
		// Precondition : associative(op) ∧ n ≥ 0
		if (n % I{ 2 } != I{ 0 }) {
			r = op(r, a);
			if (n == I{ 1 }) return r;
		}
		else if (n == I{ 0 }) return r;
		a = op(a, a);
		n = n / I{ 2 };
		return power_accumulate_3(a, r, n, op);
	}

	// A strict-tail-recurisive procedure can be transformed to an iterative one
	// by replacing each recursive call with a goto to the beginning of the procedure
	// or by using an equivalen iterative construction
    template<typename I, typename Op>
        requires(Integer(I) && BinaryOperation(Op))
			Domain(Op) power_accumulate_4(Domain(Op) a, Domain(Op) r, I n, Op op) {
			// Precondition : associative(op) ∧ n ≥ 0
		while (true) {
			if (n % I{ 2 } != I{ 0 }) {
				r = op(r, a);
				if (n == I{ 1 }) return r;
			}
			else if (n == I{ 0 }) return r;
			a = op(a, a);
			n = n / I{ 2 };
		}
	}

	// If n > 0 it will pass through 1 before becoming 0. We take adventage
	// of this by removing the test for 0 and strengthening the precondition.
	template<typename I, typename Op>
		requires(Integer(I) && BinaryOperation(Op))
	Domain(Op) power_accumulate_positive_0(Domain(Op) a, Domain(Op) r, I n, Op op) {
		// Precondition : associative(op) ∧ n > 0
		while (true) {
			if (n % I{ 2 } != I{ 0 }) {
				r = op(r, a);
				if (n == I{ 1 }) return r;
			}
			a = op(a, a);
			n = n / I{ 2 };
		}
	}

	// Relaxing the precondition
	template<typename I, typename Op>
		requires(Integer(I) && BinaryOperation(Op))
	Domain(Op) power_accumulate_5(Domain(Op) a, Domain(Op) r, I n, Op op) {
		// Precondition : associative(op) ∧ n ≥ 0
		if (n == I{ 0 }) return r;
		return power_accumulate_positive_0(a, r, n, op);
	}

	// Transformation: accumulation-variable elimination
	// a^n = a*a^(n-1)
	template<typename I, typename Op>
	requires(Integer(I) && BinaryOperation(Op))
		Domain(Op) power_2(Domain(Op) a, I n, Op op) {
		// Precondition : associative(op) ∧ n > 0
		return power_accumulate_5(a, a, n - I{ 1 }, op);
	}

	// power_2 performs more operations than needed
	template<typename I, typename Op>
	requires(Integer(I) && BinaryOperation(Op))
		Domain(Op) power_3(Domain(Op) a, I n, Op op) {
		// Precondition : associative(op) ∧ n > 0
		while (n % I{ 2 } == 0) {
			a = op(a, a);
			n = n / I{ 2 };
		}
		n = n / I{ 2 };
		if (n == I{ 0 }) return a;
		return power_accumulate_positive_0(op(a, a), a, n , op);
	}

	// final implementation

	template<typename I, typename Op>
		requires(Integer(I) && BinaryOperation(Op))
	Domain(Op) power_accumulate_positive(Domain(Op) a, Domain(Op) r, I n, Op op) {
		// Precondition : associative(op) ∧ n > 0
		while (true) {
			if (odd(n)) {
				r = op(r, a);
				if (one(n)) return r;
			}
			a = op(a, a);
			n = half_nonnegative(n);
		}
	}

	template<typename I, typename Op>
		requires(Integer(I) && BinaryOperation(Op))
	Domain(Op) power_accumulate_positive_accumulative(Domain(Op) a, Domain(Op) r, I n, Op op) {
		// Precondition : associative(op) ∧ n > 0
		while (true) {
			if (odd(n)) {
				op(r, a);
				if (one(n)) return r;
			}
			op(a, a);
			n = half_nonnegative(n);
		}
	}

	template<typename I, typename Op>
	requires(Integer(I) && BinaryOperation(Op))
		Domain(Op) power_accumulate(Domain(Op) a, Domain(Op) r, I n, Op op) {
		// Precondition : associative(op) ∧ n ≥ 0
		if (zero(n)) return r;
		return power_accumulate_positive(a, r, n, op);
	}

	template<typename I, typename Op>
		requires(Integer(I) && BinaryOperation(Op))
	Domain(Op) power_accumulate_accumulative(Domain(Op) a, Domain(Op) r, I n, Op op) {
		// Precondition : associative(op) ∧ n ≥ 0
		if (zero(n)) return r;
		return power_accumulate_positive_accumulative(a, r, n, op);
	}

	template<typename I, typename Op>
	requires(Integer(I) && BinaryOperation(Op))
		Domain(Op) power(Domain(Op) a, I n, Op op) {
		// Precondition : associative(op) ∧ n > 0
		while (even(n)) {
			a = op(a, a);
			n = half_nonnegative(n);
		}
		n = half_nonnegative(n);
		if (zero(n)) return a;
		return power_accumulate_positive(op(a, a), a, n, op);
	}

	template<typename I, typename Op>
		requires(Integer(I) && BinaryOperation(Op))
	Domain(Op) power(Domain(Op) a, I n, Op op, Domain(Op) id) {
		// Precondition : associative(op) ∧ ¬negative
		if (zero(n)) return id;
		return power(a, n, op);
	}

	template<typename I>
		requires(Integer(I))
	std::pair<I, I> fibonacci_matrix_multiply(const std::pair<I, I>& x,
											  const std::pair<I, I>& y) {
		return std::pair<I, I>(
			x.first * (y.second + y.first) + x.second * y.first,
			x.first * y.first + x.second * y.second);
	}

	template<typename I>
		requires(Integer(I))
	I fibonacci(I n) {
		// Precondition: n ≥ 0
		if (zero(n)) return I{ 0 };
		return power(std::pair<I, I>(I{ 1 }, I{ 0 }),
					 n,
					 fibonacci_matrix_multiply<I>).first;
	}

	// special case procedures
	// signed integer
	template<typename I>
		requires(Integer(I))
	I successor(I n) {
		return ++n;
	}

	template<typename I>
		requires(Integer(I))
	I predecessor(I n) {
		return n - I{ 1 };
	}

	template<typename I>
		requires(Integer(I))
	I twice(I n) {
		return n << 1;
	}

	template<typename I>
		requires(Integer(I))
	I half_nonnegative(I n) {
		// Precondition : n ≥ 0
		return n >> 1;
	}

	template<typename I>
		requires(Integer(I))
	I binary_scale_down_nonnegative(I n, I k) {
		// Precondition : n,k ≥ 0
		return n >> k;
	}

	template<typename I>
		requires(Integer(I))
	I binary_scale_up_nonnegative(I n, I k) {
		// Precondition : n,k ≥ 0
		return n << k;
	}

	template<typename I>
		requires(Integer(I))
	bool positive(I n) {
		return n > I{ 0 };
	}

	template<typename I>
		requires(Integer(I))
	bool zero(I n) {
		return n == I{ 0 };
	}

	template<typename I>
		requires(Integer(I))
	bool one(I n) {
		return n == I{ 1 };
	}

	template<typename I>
		requires(Integer(I))
	bool even(I n) {
		return (n & I{ 1 }) == I{ 0 };
	}

	template<typename I>
		requires(Integer(I))
	bool odd(I n) {
		return (n & I{ 1 }) == I{ 1 };
	}

	// Chapter 4 - Linear Orderings
	template<typename R>
		requires(Relation(R))
	const Domain(R)& select_0_2(const Domain(R)& a,
	                 			const Domain(R)& b, R r) {
		// Precondition: weak_ordering(r)
		if (r(b, a)) return b;
		return a;
	}

	template<typename R>
		requires(Relation(R))
	const Domain(R)& select_1_2(const Domain(R)& a,
	                 			const Domain(R)& b, R r) {
		// Precondition: weak_ordering(r)
		if (r(b, a)) return a;
		return b;
	}

	template<typename R>
		requires(Relation(R))
	const Domain(R)& select_0_3(const Domain(R)& a,
								const Domain(R)& b,
	                 			const Domain(R)& c, R r) {
		// Precondition: weak_ordering(r)
		return select_0_2(select_0_2(a, b, r), c, r);
	}

	template<typename R>
		requires(Relation(R))
	const Domain(R)& select_2_3(const Domain(R)& a,
								const Domain(R)& b,
	                 			const Domain(R)& c, R r) {
		// Precondition: weak_ordering(r)
		return select_1_2(select_1_2(a, b, r), c, r);
	}

	/* select_1_3 */

	template<typename R>
		requires(Relation(R))
	const Domain(R)& select_1_3_ab(const Domain(R)& a,
								   const Domain(R)& b,
	                 			   const Domain(R)& c, R r) {
		// Precondition: weak_ordering(r)
		if (!r(c, b)) return b;     // a, b, c are sorted
		return select_1_2(a, c, r); // b is not the median
	}

	template<typename R>
		requires(Relation(R))
	const Domain(R)& select_1_3(const Domain(R)& a,
								const Domain(R)& b,
	                 			const Domain(R)& c, R r) {
		// Precondition: weak_ordering(r)
		if (r(b, a)) return select_1_3_ab(b, a, c, r);
		return              select_1_3_ab(a, b, c, r);
	}

	/* select_1_4 */

	template<typename R>
		requires(Relation(R))
	const Domain(R)& select_1_4_ab_cd(const Domain(R)& a,
								      const Domain(R)& b,
								      const Domain(R)& c,
	                 			      const Domain(R)& d, R r) {
		// Precondition: weak_ordering(r)
		if (r(c, a)) return select_0_2(a, d, r);
		return              select_0_2(b, c, r);
	}

	template<typename R>
		requires(Relation(R))
	const Domain(R)& select_1_4_ab(const Domain(R)& a,
								   const Domain(R)& b,
								   const Domain(R)& c,
	                 			   const Domain(R)& d, R r) {
		// Precondition: weak_ordering(r)
		if (r(d, c)) return select_1_4_ab_cd(a, b, d, c, r);
		return              select_1_4_ab_cd(a, b, c, d, r);
	}

	template<typename R>
		requires(Relation(R))
	const Domain(R)& select_1_4(const Domain(R)& a,
								const Domain(R)& b,
								const Domain(R)& c,
	                 			const Domain(R)& d, R r) {
		// Precondition: weak_ordering(r)
		if (r(b, a)) return select_1_4_ab(b, a, c, d, r);
		return              select_1_4_ab(a, b, c, d, r);
	}

	/* select_2_4 */
		template<typename R>
		requires(Relation(R))
	const Domain(R)& select_2_4_ab_cd(const Domain(R)& a,
								      const Domain(R)& b,
								      const Domain(R)& c,
	                 			      const Domain(R)& d, R r) {
		// Precondition: weak_ordering(r)
		if (r(d, b)) return select_1_2(a, d, r);
		return              select_1_2(b, c, r);
	}

	template<typename R>
		requires(Relation(R))
	const Domain(R)& select_2_4_ab(const Domain(R)& a,
								   const Domain(R)& b,
								   const Domain(R)& c,
	                 			   const Domain(R)& d, R r) {
		// Precondition: weak_ordering(r)
		if (r(d, c)) return select_2_4_ab_cd(a, b, d, c, r);
		return              select_2_4_ab_cd(a, b, c, d, r);
	}

	template<typename R>
		requires(Relation(R))
	const Domain(R)& select_2_4(const Domain(R)& a,
								const Domain(R)& b,
								const Domain(R)& c,
	                 			const Domain(R)& d, R r) {
		// Precondition: weak_ordering(r)
		if (r(b, a)) return select_2_4_ab(b, a, c, d, r);
		return              select_2_4_ab(a, b, c, d, r);
	}

	/* maintaining stability */

	template<bool strict, typename R>
		requires(Relation(R))
	struct compare_strict_or_reflexive;

	/* specializing for two cases:
	 * (1): stability indices in increasing order
	 * (2): decreasing order
	 */

	template<typename R>
		requires(Relation(R))
	struct compare_strict_or_reflexive<true, R>
	{
		bool operator()(const Domain(R)& a,
						const Domain(R)& b, R r) {
			return r(a, b);
		}
	};

	template<typename R>
		requires(Relation(R))
	struct compare_strict_or_reflexive<false, R>
	{
		bool operator()(const Domain(R)& a,
						const Domain(R)& b, R r) {
			return !r(b, a); // complement_of_connverse r (a,b)
		}
	};

	template<int ia, int ib, typename R>
		requires(Relation(R))
	const Domain(R)& select_0_2(const Domain(R)& a,
								const Domain(R)& b, R r) {
		compare_strict_or_reflexive < (ia < ib), R> cmp;
		if (cmp(b, a, r)) return b;
		return a;
	}

	/* select_1_4 with stability indices*/

	template<int ia, int ib, int ic, int id, typename R>
		requires(Relation(R))
	const Domain(R)& select_1_4_ab_cd(const Domain(R)& a,
								      const Domain(R)& b,
								      const Domain(R)& c,
	                 			      const Domain(R)& d, R r) {
		// Precondition: weak_ordering(r)
		compare_strict_or_reflexive<(ia < ic), R> cmp;
		if (cmp(c, a, r)) return select_0_2<ia, id>(a, d, r);
		return                   select_0_2<ib, ic>(b, c, r);
	}

	template<int ia, int ib, int ic, int id, typename R>
		requires(Relation(R))
	const Domain(R)& select_1_4_ab(const Domain(R)& a,
								   const Domain(R)& b,
								   const Domain(R)& c,
	                 			   const Domain(R)& d, R r) {
		// Precondition: weak_ordering(r)
		compare_strict_or_reflexive<(ic < id), R> cmp;
		if (cmp(d, c, r)) return select_1_4_ab_cd<ia, ib, id, ic>(a, b, d, c, r);
		return	                 select_1_4_ab_cd<ia, ib, ic, id>(a, b, c, d, r);
	}

	template<int ia, int ib, int ic, int id, typename R>
		requires(Relation(R))
	const Domain(R)& select_1_4(const Domain(R)& a,
								const Domain(R)& b,
								const Domain(R)& c,
	                 			const Domain(R)& d, R r) {
		// Precondition: weak_ordering(r)
		compare_strict_or_reflexive<(ia < ib), R> cmp;
		if (cmp(b, a, r)) return select_1_4_ab<ib, ia, ic, id>(b, a, c, d, r);
		return		             select_1_4_ab<ia, ib, ic, id>(a, b, c, d, r);
	}

	template<int ia, int ib, int ic, int id, int ie, typename R>
		requires(Relation(R))
	const Domain(R)& select_2_5_ab_cd(const Domain(R)& a,
									  const Domain(R)& b,
									  const Domain(R)& c,
									  const Domain(R)& d,
	                 				  const Domain(R)& e, R r) {
		// Precondition: weak_ordering(r)
		compare_strict_or_reflexive<(ia < ic), R> cmp;
		if (cmp(c, a, r)) return 
			select_1_4_ab<ia, ib, id, ie>(a, b, d, e, r);
		return 
			select_1_4_ab<ic, id, ib, ie>(c, d, b, e, r);
	}

	template<int ia, int ib, int ic, int id, int ie, typename R>
		requires(Relation(R))
	const Domain(R)& select_2_5_ab(const Domain(R)& a,
								   const Domain(R)& b,
								   const Domain(R)& c,
								   const Domain(R)& d,
	                 		  	   const Domain(R)& e, R r) {
		// Precondition: weak_ordering(r)
		compare_strict_or_reflexive<(ic < id), R> cmp;
		if (cmp(d, c, r)) return 
			select_2_5_ab_cd<ia, ib, id, ic, ie>(a, b, d, c, e, r);
		return 
			select_2_5_ab_cd<ia, ib, ic, id, ie>(a, b, c, d, e, r);
	}

	template<int ia, int ib, int ic, int id, int ie, typename R>
		requires(Relation(R))
	const Domain(R)& select_2_5(const Domain(R)& a,
								const Domain(R)& b,
								const Domain(R)& c,
								const Domain(R)& d,
	                 		  	const Domain(R)& e, R r) {
		// Precondition: weak_ordering(r)
		compare_strict_or_reflexive<(ia < ib), R> cmp;
		if (cmp(b, a, r)) return 
			select_2_5_ab<ib, ia, ic, id, ie>(b, a, c, d, e, r);
		return 
			select_2_5_ab<ia, ib, ic, id, ie>(a, b, c, d, e, r);
	}

	template<typename R>
		requires(Relation(R))
	const Domain(R)& median_5(const Domain(R)& a,
							  const Domain(R)& b,
							  const Domain(R)& c,
							  const Domain(R)& d,
	                 		  const Domain(R)& e, R r) {
		// Precondition: weak_ordering(r)
		return select_2_5<0, 1, 2, 3, 4>(a, b, c, d, e, r);
	}

	// Chapter 6 - Iterators

	template<typename T>
	struct is_Even {
		typedef T input_type;
		bool operator()(T value) {
			return eop::even(value);
		}
	};

	template<typename T>
	struct is_Odd {
		typedef T input_type;
		bool operator()(T value) {
			return eop::odd(value);
		}
	};

	template<typename I>
	requires(Iterator(I))
		I increment(I& x) {
		// Precondition: successor(x) is defined
		return successor(x);
	}

	template<typename I>
	requires(Iterator(I))
		I operator+(I f, DistanceType(I) n) {
		// Precondition: n >= 0 & weak_range(f, n)
		while (!zero(n)) {
			n = predecessor(n);
			f = successor(f);
		}
		return f;
	}

	template<typename I>
		requires(Iterator(I))
	DistanceType(I) operator-(I l, I f) {
		// Precondition: bounded_range(f, l)
		DistanceType(I) n(0);
		while (f != l) {
			n = successor(n);
			f = successor(f);
		}
		return n;
	}

	template<typename I>
		//requires(Readable(I))
	ValueType(I) source(I x) {
		return *x;
	}

	template<typename I, typename Proc>
		requires(Readable(I) && Iterator(I) && Procedure(Proc) && Arity(Proc) == 1 && 
			ValueType(I) == InputType(Proc, 0))
	Proc for_each(I f, I l, Proc proc) {
		// Precondition: readable_bounded_range(f, l)
		while (f != l) {
			proc(source(f));
			f = successor(f);
		}
		return proc;
	}

	template<typename I>
		requires(Readable(I) && Iterator(I))
	I find(I f, I l, const ValueType(I)& x) {
		// Precondition: readable_bounded_range(f, l)
		while (f != l && source(f) != x) f = successor(f);
		return f;
	}

	template<typename I, typename P>
		requires(Readable(I) && Iterator(I) && UnaryPredicate(P)
			&& ValueType(I) == Domain(P))
	I find_if(I f, I l, P p) {
		// Precondition: readable_bounded_range(f, l)
		while (f != l && !p(source(f))) f = successor(f);
		return f;
	}

	template<typename I, typename P>
		requires(Readable(I) && Iterator(I) && UnaryPredicate(P)
		&& ValueType(I) == Domain(P))
	I find_if_not(I f, I l, P p) {
		// Precondition: readable_bounded_range(f, l)
		while (f != l && p(source(f))) f = successor(f);
		return f;
	}

	template<typename I, typename P>
		requires(Readable(I) && Iterator(I) && UnaryPredicate(P)
			&& ValueType(I) == Domain(P))
	bool all(I f, I l, P p) {
		// Precondition: readable_bounded_range(f, l)
		return find_if_not(f, l, p) == l;
	}

	template<typename I, typename P>
		requires(Readable(I) && Iterator(I) && UnaryPredicate(P)
			&& ValueType(I) == Domain(P))
	bool none(I f, I l, P p) {
		// Precondition: readable_bounded_range(f, l)
		return find_if(f, l, p) == l;
	}

	template<typename I, typename P>
		requires(Readable(I) && Iterator(I) && UnaryPredicate(P)
			&& ValueType(I) == Domain(P))
	bool not_all(I f, I l, P p) {
		// Precondition: readable_bounded_range(f, l)
		return !all(f, l, p);
	}

	template<typename I, typename P>
		requires(Readable(I) && Iterator(I) && UnaryPredicate(P)
			&& ValueType(I) == Domain(P))
	bool some(I f, I l, P p) {
		// Precondition: readable_bounded_range(f, l)
		return !none(f, l, p);
	}

	template<typename P, typename J>
		requires(UnaryPredicate(P))
	struct counter_if {
		J j;
		P p;
		counter_if(P p, J j) : p(p), j(j) {}
		void operator()(Domain(P) x) {
			if (p(x)) j = successor(j);
		}
	};

	template<typename I, typename P, typename J>
		requires(Readable(I) && Iterator(I) && UnaryPredicate(P)
			&& ValueType(I) == Domain(P))
	J count_if(I f, I l, P p, J j) {
		// Precondition: readable_bounded_range(f, l)
		return for_each(f, l, counter_if<P, J>(p, j)).j;
	}

	template<typename I, typename P>
	requires(Readable(I) && Iterator(I) && UnaryPredicate(P)
		&& ValueType(I) == Domain(P))
	DistanceType(I) count_if(I f, I l, P p) {
		// Precondition: readable_bounded_range(f, l)
		return count_if(f, l, p, DistanceType(I){0});
	}

	template<typename T>
	struct plus {
		typedef T input_type;
		T operator()(T a, T b) {
			return a + b;
		}
	};

	template<typename T>
	struct multiplies {
		typedef T input_type;
		T operator()(T a, T b) {
			return a * b;
		}
	};

	template<typename I>
	struct source_function {
		typedef I input_type;
		ValueType(I) operator()(I i) {
			return source(i);
		}
	};

	template<typename I, typename Op, typename F>
	requires(Iterator(I) && BinaryOperation(Op) &&
		UnaryFunction(F) &&
		I == Domain(F) && Codomain(F) == Domain(Op))
	Domain(Op) reduce_nonempty(I f, I l, Op op, F fun) {
		// Precondition: bounded_range(f, l) && f != l
		// Precondition: partially_associative(op)
		// Precondition: (All x in [f, l)) fun(x) is defined
		Domain(Op) r = fun(f);
		f = successor(f);
		while (f != l) {
			r = op(r, fun(f));
			f = successor(f);
		}
		return r;
	}

	template<typename I, typename Op, typename F>
	requires(Iterator(I) && BinaryOperation(Op) &&
		UnaryFunction(F) &&
		I == Domain(F) && Codomain(F) == Domain(Op))
	Domain(Op) reduce_nonempty(I f, I l, Op op, F fun, const Domain(Op)& z) {
		// Precondition: bounded_range(f, l) && f != l
		// Precondition: partially_associative(op)
		// Precondition: (All x in [f, l)) fun(x) is defined
		if (f == l) return z;
		return reduce_nonempty(f, l, op, fun);
	}

	template<typename I, typename Op, typename F>
	requires(Iterator(I) && BinaryOperation(Op) && 
		UnaryFunction(F) &&
		I == Domain(F) && Codomain(F) == Domain(Op))
	Domain(Op) reduce_nonzeros(I f, I l, Op op, F fun, const Domain(Op)& z) {
		Domain(Op) x;
		do {
			if (f == l) return z;
			x = fun(f);
			f = successor(f);
		} while (x == z);
		while (f != l) {
			Domain(Op) y = fun(f);
			if (z != y) x = op(x, y);
			f = successor(f);
		}
		return x;
	}
} // namespace eop