// eop.h

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
#include <vector>

#include "intrinsics.h"
#include "pointers.h"
#include "type_functions.h"

#include <iostream>

namespace eop {

  // *******************************************************
  // Chapter 1 - Foundations
  // *******************************************************

  template<typename Op>
  requires(BinaryOperation(Op))
  Domain(Op) square(const Domain(Op)& x, Op op) {
    return op(x, x);
  }

  // *******************************************************
  // Chapter 2 - Transformations and Their Orbits
  // *******************************************************

  template<typename F, typename N>
    requires(Transformation(F) && Integer(N))
  Domain(F) power_unary(Domain(F) x, N n, F f, transformation_trait) {
    // Precondition: n ≥ 0 ∧ (∀i ∈ N) 0 < i ≤ n => f^i(x) is defined
    while (n != N(0)) {
      n = n - N(1);
      x = f(x);
    }
    return x;
  }

  template<typename F, typename N>
    requires(Transformation(F) && Integer(N))
  Domain(F) power_unary(Domain(F) x, N n, F f, action_trait)
  {
    // Precondition: n ≥ 0 ∧ (∀i ∈ N) 0 < i ≤ n => f^i(x) is defined
    while (n != N(0)) {
      n = n - N(1);
      f(x);
    }
    return x;
  }

  template<typename F, typename N>
  requires(Transformation(F) && Integer(N))
  Domain(F) power_unary(Domain(F) x, N n, F f) 
  {
    // Precondition: n ≥ 0 ∧ (∀i ∈ N) 0 < i ≤ n => f^i(x) is defined
    return power_unary(x, n, f, FunctionTrait(F)());
  }

  template<typename F>
    requires(Transformation(F))
  DistanceType(F) distance(Domain(F) x, Domain(F) y, F f, transformation_trait) {
    // Precondition: y is reachable from x using f
    typedef DistanceType(F) N;
    N n(0);
    while (x != y) {
      x = f(x);
      n = n + N(1);
    }
    return n;
  }

  template<typename F>
    requires(Transformation(F))
  DistanceType(F) distance(Domain(F) x, Domain(F) y, F f, action_trait) {
    // Precondition: y is reachable from x using f
    typedef DistanceType(F) N;
    N n(0);
    while (x != y) {
      f(x);
      n = n + N(1);
    }
    return n;
  }

  template<typename F>
    requires(Transformation(F))
  DistanceType(F) distance(Domain(F) x, Domain(F) y, F f) {
    // Precondition: y is reachable from x using f
    return distance(x, y, f, FunctionTrait(F)());
  }

  template <typename F, typename P>
  requires(Transformation(F) && UnaryPredicate(P) &&
    Domain(F) == Domain(P))
  Domain(F) collision_point(const Domain(F)& x, F f, P p, transformation_trait) {
    // Precondition : p(x) <=> f(x) is defined
    if (!p(x)) return x;
    Domain(F) slow = x;   // slow = f^0(x)
    Domain(F) fast = f(x);    // fast = f^1(
              // n <- 0 (completed operations)
    while (fast != slow) {    // slow = f^n(x) ? fast = f^(2n+1)(x)
      slow = f(slow);   // slow = f^(n+1)(x) ? fast = f^(2n+1)(x)
      if (!p(fast)) return fast;
      fast = f(fast);   // slow = f^(n+1)(x) ? fast = f^(2n+2)(x)
      if (!p(fast)) return fast;
      fast = f(fast);   // slow = f^(n+1)(x) ? fast = f^(2n+3)(x)
    }       // n <- n + 1
    return fast;
    // Postcondition : return value is terminal point or collision point
  }

  template<typename F, typename P>
  requires(Action(F) && UnaryPredicate(P) &&
    Domain(F) == Domain(P))
  Domain(F) collision_point(const Domain(F)& x, F a, P p, action_trait) {
    // Precondition : p(x) <=> f(x) is defined
    if (!p(x)) return x;
    Domain(F) slow = x;   // slow = f^0(x)
    Domain(F) fast = x; a(fast);  // fast = f^1(
              // n <- 0 (completed operations)
    while (fast != slow) {    // slow = f^n(x) ? fast = f^(2n+1)(x)
      a(slow);    // slow = f^(n+1)(x) ? fast = f^(2n+1)(x)
      if (!p(fast)) return fast;
      a(fast);    // slow = f^(n+1)(x) ? fast = f^(2n+2)(x)
      if (!p(fast)) return fast;
      a(fast);    // slow = f^(n+1)(x) ? fast = f^(2n+3)(x)
    }       // n <- n + 1
    return fast;
    // Postcondition : return value is terminal point or collision point
  }

  template<typename F, typename P>
  requires(Transformation(F) && UnaryPredicate(P) &&
    Domain(F) == Domain(P))
  bool terminating(const Domain(F)& x, F f, P p) 
  {
    // Precondition: p(x) <=> f(x) is defined
    return !p(collision_point(x, f, p, FunctionTrait(F)()));
    // Postcondition: return true if the orbit is terminating
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
    Domain(F) y = collision_point(x, f, p, FunctionTrait(F)());
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
    Domain(F) y = collision_point(x, f, p, FunctionTrait(F)());
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
  
  // *******************************************************
  // Chapter 3 - Associative Operations
  // *******************************************************

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
  *   /  a,    if n is 1
  *  a^n =  |  (a^2)^(n/2),  if n is even
  *   \  (a^2)^( n/2)*a, if n is odd
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
    requires(Iterator(I))
  I successor(I n) {
    return ++n;
  }

  template<typename I>
    requires(Integer(I))
  I predecessor(I n) {
    return --n;
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

  // *******************************************************
  // Chapter 4 - Linear Orderings
  // *******************************************************

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
    if (!r(c, b)) return b;   // a, b, c are sorted
    return select_1_2(a, c, r); // b is not the median
  }

  template<typename R>
    requires(Relation(R))
  const Domain(R)& select_1_3(const Domain(R)& a,
            const Domain(R)& b,
        const Domain(R)& c, R r) {
    // Precondition: weak_ordering(r)
    if (r(b, a)) return select_1_3_ab(b, a, c, r);
    return    select_1_3_ab(a, b, c, r);
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
    return    select_0_2(b, c, r);
  }

  template<typename R>
    requires(Relation(R))
  const Domain(R)& select_1_4_ab(const Domain(R)& a,
             const Domain(R)& b,
             const Domain(R)& c,
           const Domain(R)& d, R r) {
    // Precondition: weak_ordering(r)
    if (r(d, c)) return select_1_4_ab_cd(a, b, d, c, r);
    return    select_1_4_ab_cd(a, b, c, d, r);
  }

  template<typename R>
    requires(Relation(R))
  const Domain(R)& select_1_4(const Domain(R)& a,
            const Domain(R)& b,
            const Domain(R)& c,
        const Domain(R)& d, R r) {
    // Precondition: weak_ordering(r)
    if (r(b, a)) return select_1_4_ab(b, a, c, d, r);
    return    select_1_4_ab(a, b, c, d, r);
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
    return    select_1_2(b, c, r);
  }

  template<typename R>
    requires(Relation(R))
  const Domain(R)& select_2_4_ab(const Domain(R)& a,
             const Domain(R)& b,
             const Domain(R)& c,
           const Domain(R)& d, R r) {
    // Precondition: weak_ordering(r)
    if (r(d, c)) return select_2_4_ab_cd(a, b, d, c, r);
    return    select_2_4_ab_cd(a, b, c, d, r);
  }

  template<typename R>
    requires(Relation(R))
  const Domain(R)& select_2_4(const Domain(R)& a,
            const Domain(R)& b,
            const Domain(R)& c,
        const Domain(R)& d, R r) {
    // Precondition: weak_ordering(r)
    if (r(b, a)) return select_2_4_ab(b, a, c, d, r);
    return    select_2_4_ab(a, b, c, d, r);
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
    return     select_0_2<ib, ic>(b, c, r);
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
    return     select_1_4_ab_cd<ia, ib, ic, id>(a, b, c, d, r);
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
    return       select_1_4_ab<ia, ib, ic, id>(a, b, c, d, r);
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

  // *******************************************************
  // Chapter 5 - Ordered Algebraic Structures
  // *******************************************************

  template<typename T>
  T remainder(T a, T b)
  {
    return a % b;
  }

  template<typename T>
  T gcd(T a, T b)
  {
    //Precondition:
    while(true) {
      if (b == T(0)) return a;
      a = remainder(a, b);
      if (a == T(0)) return b;
      b = remainder(b, a);
    }
  }

  // *******************************************************
  // Chapter 6 - Iterators
  // *******************************************************

  template<typename T>
  struct is_Even 
  {
    typedef T first_argument_type;
    typedef bool result_type;
    typedef T input_type;
    bool operator()(T value) {
      return eop::even(value);
    }
  };

  template<typename T>
  struct is_Odd 
  {
    typedef T first_argument_type;
    typedef bool result_type;
    typedef T input_type;
    bool operator()(T value) {
      return eop::odd(value);
    }
  };

  template<typename I>
    requires(Iterator(I))
  I increment(I& x) 
  {
    // Precondition: successor(x) is defined
    return successor(x);
  }

  template<typename I>
    requires(Iterator(I))
  I operator+(I f, DistanceType(I) n) 
  {
    // Precondition: n >= 0 & weak_range(f, n)
    while (!zero(n)) {
      n = predecessor(n);
      f = successor(f);
    }
    return f;
  }

  template<typename I>
    requires(Iterator(I))
  DistanceType(I) operator-(I l, I f) 
  {
    // Precondition: bounded_range(f, l)
    DistanceType(I) n(0);
    while (f != l) {
      n = successor(n);
      f = successor(f);
    }
    return n;
  }

  template<typename I, typename Proc>
    requires(Readable(I) && Iterator(I) && Procedure(Proc) && Arity(Proc) == 1 && 
    ValueType(I) == InputType(Proc, 0))
  Proc for_each(I f, I l, Proc proc) 
  {
    // Precondition: readable_bounded_range(f, l)
    while (f != l) {
      proc(source(f));
      f = successor(f);
    }
    return proc;
  }

  template<typename I>
    requires(Readable(I) && Iterator(I))
  I find(I f, I l, const ValueType(I)& x) 
  {
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
    typedef Domain(P) first_argument_type;
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

  template<typename P, typename J>
    requires(UnaryPredicate(P))
  struct counter_if_not {
    typedef Domain(P) first_argument_type;
    J j;
    P p;
    counter_if_not(P p, J j) : p(p), j(j) {}
    void operator()(Domain(P) x) {
      if (!p(x)) j = successor(j);
    }
  };

  template<typename I, typename P, typename J>
    requires(Readable(I) && Iterator(I) && UnaryPredicate(P)
    && ValueType(I) == Domain(P))
  J count_if_not(I f, I l, P p, J j) {
    // Precondition: readable_bounded_range(f, l)
    return for_each(f, l, counter_if_not<P, J>(p, j)).j;
  }

  template<typename I, typename P>
    requires(Readable(I) && Iterator(I) && UnaryPredicate(P)
      && ValueType(I) == Domain(P))
  DistanceType(I) count_if_not(I f, I l, P p) {
    // Precondition: readable_bounded_range(f, l)
    return count_if_not(f, l, p, DistanceType(I){0});
  }

  template<typename T>
  struct plus {
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;
    typedef T input_type;
    T operator()(T a, T b) {
      return a + b;
    }
  };

  template<typename T>
  struct multiplies {
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;
    typedef T input_type;
    T operator()(T a, T b) {
      return a * b;
    }
  };

  template<typename I>
  struct source_function {
    typedef I first_argument_type;
    typedef I second_argument_type;
    typedef I result_type;
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
    // Precondition: bounded_range(f, l)
    // Precondition: partially_associative(op)
    // Precondition: (All x in [f, l)) fun(x) is defined
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

  template<typename I>
  void step(I& f, DistanceType(I)& n) {
    f = successor(f);
    n = predecessor(n);
  }

  template<typename I, typename Op, typename F>
  requires(Iterator(I) && BinaryOperation(Op) &&
    UnaryFunction(F) &&
    I == Domain(F) && Codomain(F) == Domain(Op))
  Domain(Op) reduce_nonempty_n(I f, DistanceType(I) n, Op op, F fun) {
    // Precondition: weak_range(f, n) && !zero(n)
    // Precondition: partially_associative(op)
    // Precondition: (All x in [f, f+n)) fun(x) is defined
    Domain(Op) r = fun(f);
    step(f, n);
    while (!zero(n)) {
      r = op(r, fun(f));
      step(f, n);
    }
    return r;
  }

  template<typename I, typename Op, typename F>
  requires(Iterator(I) && BinaryOperation(Op) &&
    UnaryFunction(F) &&
    I == Domain(F) && Codomain(F) == Domain(Op))
  Domain(Op) reduce_nonempty_n(I f, DistanceType(I) n, Op op, F fun, const Domain(Op)& z) {
    // Precondition: weak_range(f, n) && n != 0
    // Precondition: partially_associative(op)
    // Precondition: (All x in [f, f+n)) fun(x) is defined
    if (zero(n)) return z;
    return reduce_nonempty_n(f, n, op, fun);
  }
  
  template<typename I, typename Op, typename F>
  requires(Iterator(I) && BinaryOperation(Op) && 
    UnaryFunction(F) &&
    I == Domain(F) && Codomain(F) == Domain(Op))
  Domain(Op) reduce_nonzeros_n(I f, DistanceType(I) n, Op op, const Domain(Op)& z) {
    // Precondition: bounded_range(f, l)
    // Precondition: partially_associative(op)
    // Precondition: (All x in [f, l)) fun(x) is defined
    Domain(Op) x;
    do {
      if (zero(n)) return z;
      x = fun(f);
      f = successor(f);
      n = predecessor(n);
    } while (x == z);
    while (!zero(n)) {
      Domain(Op) y = fun(f);
      if (z != y) x = op(x, y);
      f = successor(f);
      n = predecessor(n);
    }
    return x;
  }

  template<typename I, typename Proc>
  requires(Readable(I) && Iterator(I) && 
    Procedure(Proc) && Arity(Proc) == 1 &&
    ValueType(I) == InputType(Proc, 0))
  std::pair<Proc, I> for_each_n(I f, DistanceType(I) n, Proc proc) {
    // Precondition: readable_weak_range(f, n)
    while (!zero(n)) {
      n = predecessor(n);
      proc(source(f));
      f = successor(f);
    }
    return std::pair<Proc, I>(proc, f);
  }

  template<typename I>
    requires(Readable(I) && Iterator(I))
  std::pair<I, DistanceType(I)> find_n(I f, DistanceType(I) n,
      const ValueType(I)& x) 
  {
    // Precondition: readable_weak_range(f, n)
    while (!zero(n) && source(f) != x) {
      n = predecessor(n);
      f = successor(f);
    }
    return std::pair<I, DistanceType(I)>(f, n);
  }

  // Exercise 6.3
  template<typename I>
    requires(Readable(I) && Iterator(I))
  std::pair<I, DistanceType(I)> find_if_n(I f, DistanceType(I) n,
    const ValueType(I)& x)
  {
    // Precondition: readable_weak_range(f, n)
    while (!zero(n) && source(f) != x) {
      n = predecessor(n);
      f = successor(f);
    }
    return std::make_pair(f, n);
  }

  template<typename I>
    requires(Readable(I) && Iterator(I))
  std::pair<I, DistanceType(I)> find_if_not_n(I f, DistanceType(I) n,
    const ValueType(I)& x)
  {
    // Precondition: readable_weak_range(f, n)
    while (!zero(n) && source(f) == x) {
      n = predecessor(n);
      f = successor(f);
    }
    return std::make_pair(f, n);
  }

  template<typename I, typename P>
  requires(Readable(I) && Iterator(I) && UnaryPredicate(P) &&
    ValueType(I) == Domain(P))
  bool all_n(I f, DistanceType(I) n, P p) 
  {
    // Precondition: readable_weak_range(f, l)
    while (!zero(n) && p(source(f))) {
      n = predecessor(n);
      f = successor(f);
    }
    return zero(n);
  }

  template<typename I, typename P>
  requires(Iterator(I) && Readable(I) && UnaryPredicate(P) &&
    ValueType(I) == Domain(P))
  bool none_n(I f, DistanceType(I) n, P p) 
  {
    // Precondition: readable_weak_range(f, n)
    while (!zero(n) && !p(source(f))) {
      n = predecessor(n);
      f = successor(f);
    }
    return zero(n);
  }

  template<typename I, typename P>
  requires(Iterator(I) && Readable(I) && UnaryPredicate(P) &&
    ValueType(I) == Domain(P))
  bool not_all_n(I f, DistanceType(I) n, P p) 
  {
    // Precondition: readable_weak_range(f, n)
    return !all_n(f, n, p);
  }

  template<typename I, typename P>
  requires(Iterator(I) && Readable(I) && UnaryPredicate(P) &&
    ValueType(I) == Domain(P))
  bool some_n(I f, DistanceType(I) n, P p)
  {
    // Precondition: readable_weak_range(f, n)
    return !none_n(f, n, p);
  }

  template<typename I, typename P>
    requires(Readable(I) && Iterator(I) &&
    UnaryPredicate(P) && ValueType(I) == Domain(P))
  I find_if_ungarded(I f, P p)
  {
    // Precondition: (Exists L) readable_bounded_range(f, l) && some(f, l, p)
    while (!p(source(f))) f = successor(f);
    return f;
  }

  template<typename I, typename P>
    requires(Readable(I) && Iterator(I) &&
    UnaryPredicate(P) && ValueType(I) == Domain(P))
  I find_if_not_ungarded(I f, P p)
  {
    // Precondition: (Exists L) readable_bounded_range(f, l) && some(f, l, p)
    while (p(source(f))) f = successor(f);
    return f;
  }

  template<typename I0, typename I1, typename R>
  requires(Readable(I0) && Iterator(I0) &&
    Readable(I1) && Iterator(I1) && Relation(R) &&
    ValueType(I0) == ValueType(I1) &&
    ValueType(I0) == Domain(R))
  std::pair<I0, I1> find_mismatch(I0 f0, I0 l0, I1 f1, I1 l1, R r) 
  {
    // Precondition: readable_bounded_range(f0, l0)
    // Precondition: readable_bounded_range(f1, l1)
    while (f0 != l0 && f1 != l1 && r(source(f0), source(f1))) {
    f0 = successor(f0);
    f1 = successor(f1);
    }
    return std::make_pair(f0, f1);
    // Postcondition: 
  }

  template<typename T>
  struct is_equal {
    bool operator()(T const& a, T const& b) {
      return a == b;
    }
  };

  template<typename I0, typename I1>
  requires(Readable(I0) && Iterator(I0) &&
    Readable(I1) && Iterator(I1) &&
    ValueType(I0) == ValueType(I1))
  bool equals(I0 f0, I0 l0, I1 f1, I1 l1) {
    std::pair<I0, I1> mismatch = find_mismatch(f0, l0, f1, l1, is_equal<ValueType(I0)>());
    return mismatch.first == l0 && mismatch.second == l1;
  }

  template<typename I0, typename I1, typename R>
  requires(Readable(I0) && Iterator(I0) &&
    Readable(I1) && Iterator(I1) && Relation(R) &&
    ValueType(I0) == ValueType(I1) &&
    ValueType(I0) == Domain(R))
  std::pair<std::pair<I0, DistanceType(I0)>, I1> find_mismatch(
    I0 f0, DistanceType(I0) n0, I1 f1, I1 l1, R r)
  {
    // Precondition: readable_weak_range(f0, n0)
    // Precondition: readable_bounded_range(f1, l1)
    while (!zero(n0) && f1 != l1 && r(source(f0), source(f1))) {
      f0 = successor(f0);
      n0 = predecessor(n0);
      f1 = successor(f1);
    }
    return std::make_pair(std::make_pair(f0, n0), f1);
    // Postcondition: 
  }

  template<typename I0, typename I1, typename R>
  requires(Readable(I0) && Iterator(I0) &&
    Readable(I1) && Iterator(I1) && Relation(R) &&
    ValueType(I0) == ValueType(I1) &&
    ValueType(I0) == Domain(R))
  std::pair<I0, std::pair<I1, DistanceType(I1)>> find_mismatch(
    I0 f0, I0 l0, I1 f1, DistanceType(I1) n1, R r)
  {
    // Precondition: readable_bounded_range(f0, l0)
    // Precondition: readable_weak_range(f1, n1)
    while (f0 != l0 && !zero(n1) && r(source(f0), source(f1))) {
      f0 = successor(f0);
      f1 = successor(f1);
      n1 = predecessor(n1);
    }
    return std::make_pair(f0, std::make_pair(f1, n1));
    // Postcondition: 
  }

  template<typename I0, typename I1, typename R>
  requires(Readable(I0) && Iterator(I0) &&
    Readable(I1) && Iterator(I1) && Relation(R) &&
    ValueType(I0) == ValueType(I1) &&
    ValueType(I0) == Domain(R))
  std::pair<std::pair<I0, DistanceType(I0)>, std::pair<I1, DistanceType(I1)>> find_mismatch(
    I0 f0, DistanceType(I0) n0, I1 f1, DistanceType(I1) n1, R r)
  {
    // Precondition: readable_weak_range(f0, n0)
    // Precondition: readable_weak_range(f1, n1)
    while (!zero(n0) && !zero(n1) && r(source(f0), source(f1))) {
      f0 = successor(f0);
      n0 = predecessor(n0);
      f1 = successor(f1);
      n1 = predecessor(n1);
    }
    return std::make_pair(std::make_pair(f0, n0), std::make_pair(f1, n1));
    // Postcondition: 
  }

  template<typename I, typename R>
  requires(Readable(I) && Iterator(I) && Relation(R) &&
    ValueType(I) == Domain(R))
  I find_adjacent_mismatch(I f, I l, R r) 
  {
    // Precondition: readable_bounded_range(f, l)
    if (f == l) return f;
    ValueType(I) x = source(f);
    f = successor(f);
    while (f != l && r(x, source(f))) {
      x = source(f);
      f = successor(f);
    }
    return f;
    // Postcondition: returns an iterator to the first element that is not in
    //    R relation with its previous element
  }

  template<typename I, typename R>
  requires(Readable(I) && Iterator(I) && Relation(R) &&
    ValueType(I) == Domain(R))
  bool relation_preserving(I f, I l, R r) 
  {
    // Precondition: readable_bounded_range(f, l)
    return l == find_adjacent_mismatch(f, l, r);
  }

  template<typename I, typename R>
  requires(Readable(I) && Iterator(I) && Relation(R) &&
    ValueType(I) == Domain(R))
  bool strictly_increasing_range(I f, I l, R r) 
  {
    // Precondition: readable_bounded_range(f, l)
    return relation_preserving(f, l, r);
  }

  template<typename R>
  struct complement_of_converse
  {
    typedef Domain(R) T;
    R r;
    complement_of_converse(const R& r) : r(r) {}
    bool operator()(const T& a, const T& b) 
    {
      return !r(b, a);
    }
  };

  template<typename I, typename R>
  requires(Readable(I) && Iterator(I) && Relation(R) &&
    ValueType(I) == Domain(R))
  bool increasing_range(I f, I l, R r)
  {
    // Precondition: readable_bounded_range(f, l)
    return relation_preserving(f, l, complement_of_converse<R>(r));
  }

  template<typename I, typename P>
  requires(Readable(I) && Iterator(I) && UnaryPredicate(P) &&
    ValueType(I) == Domain(P))
  bool partitioned(I f, I l, P p) 
  {
    // Precondition: readable_bounded_range(f, l)
    return l == find_if_not(find_if(f, l, p), l, p);
  }

  template<typename I, typename P>
  requires(Readable(I) && Iterator(I) && UnaryPredicate(P) &&
    ValueType(I) == Domain(P))
  bool partitioned_n(I f, DistanceType(I) n, P p) 
  {
    // Precondition: readable_counted_range(f, n)
    auto r0 = find_if_n(f, n, p);
    auto r1 = find_if_not_n(r0.first, r0.second, p);
    return zero(r1.second);
  }

  // Forward iterators

  template<typename I, typename R>
  requires(ForwardIterator(I) && Readable(I) && Relation(R) &&
    ValueType(I) == Domain(R))
  I find_adjacent_mismatch_forward(I f, I l, R r) 
  {
    // Precondition: readable_bounded_range(f, l)
    if (f == l) return l;
    I t;
    do {
      t = f;
      f = successor(f);
    } while (f != l && r(source(t), source(f)));
    return f;
  }

  template<typename I, typename P>
  requires(ForwardIterator(I) && Readable(I) && UnaryPredicate(P) &&
    ValueType(I) == Domain(R))
  I partition_point_n(I f, DistanceType(I) n, P p) 
  {
    // Precondition: readable_counted_range(f, n) & partitioned_n(f, n, p)
    while (!zero(n)) {
      DistanceType(I) h = half_nonnegative(n);
      I m = f + h;
      if (p(source(m))) {
        n = h;
      }
      else {
        n = n - successor(h);
        f = successor(m);
      }
    }
    return f;
  }

  template<typename I, typename P>
  requires(ForwardIterator(I) && Readable(I) && UnaryPredicate(P) &&
    ValueType(I) == Domain(P))
  I partition_point(I f, I l, P p) 
  {
    // Precondition: readable_bounded_range(f, l) & partitioned(f, l, p)
    return partition_point_n(f, l - f, p);
  }

  template<typename R>
    requires(Relation(R))
  struct lower_bound_predicate {
    typedef Domain(R) T;
    const T& a;
    R r;
    lower_bound_predicate(const T& a, R r) : a(a), r(r) {}
    bool operator()(const T& x) { return !r(x, a); }
  };

  template<typename I, typename R>
  requires(ForwardIterator(I) && Readable(R) && Relation(R) &&
    ValueType(I) == Domain(R))
  I lower_bound_n(I f, DistanceType(I) n, const ValueType(I)& a, R r) 
  {
    // Precondition: weak_increasing(r) && increasing_counted_range(f, n, r)
    lower_bound_predicate<R> p(a, r);       
    return partition_point_n(f, n, p);
  }

  template<typename R>
    requires(Relation(R))
  struct upper_bound_predicate {
    typedef Domain(R) T;
    const T& a;
    R r;
    upper_bound_predicate(const T& a, R r) : a(a), r(r) {}
    bool operator()(const T& x) { return r(a, x); }
  };

  template<typename I, typename R>
  requires(ForwardIterator(I) && Readable(I) && Relation(R) &&
    ValueType(I) == Domain(R))
  I upper_bound_n(I f, DistanceType(I) n, const ValueType(I)& a, R r) 
  {
    // Precondition: weak_increasing(r) && increasing_counted_range(f, n, r)
    upper_bound_predicate<R> p(a, r);
    return partition_point_n(f, n, p);
  }

  template<typename I, typename R>
  requires(ForwardIterator(I) && Readable(R) && Relation(R) &&
    ValueType(I) == Domain(R))
  I lower_bound(I f, I l, const ValueType(I)& a, R r)
  {
    // Precondition: weak_increasing(r) && increasing_bounded_range(f, l, r)
    return lower_bound_n(f, l - f, a, r);
  }

  template<typename I, typename R>
  requires(ForwardIterator(I) && Readable(I) && Relation(R) &&
    ValueType(I) == Domain(R))
  I upper_bound(I f, I l, const ValueType(I)& a, R r) 
  {
    // Precondition weak_increasing(r) && increasing_bounded_range(f, l, r)
    return upper_bound_n(f, l - f, a, r);
  }

  template<typename I, typename R>
  requires(ForwardIterator(I) && Readable(I) && Relation(R) &&
    ValueType(I) == Domain(R))
  std::pair<I, I> equal_range_n(I f, DistanceType(I) n, const ValueType(I)& a, R r) 
  {
    // Precondition: weak_incresing(r) && increasing_counted_range(f, n, r)
    I lower = lower_bound_n(f, n, a, r);
    I upper = upper_bound_n(lower, n - (lower - f), a, r);
    return std::pair<I, I>(lower, upper);
  }

  template<typename I, typename R>
  requires(ForwardIterator(I) && Readable(I) && Relation(R) &&
    ValueType(I) == Domain(R))
  std::pair<I, I> equal_range(I f, I l, const ValueType(I)& a, R r)
  {
    // Precondition weak_increasing(r) && increasing_bounded_range(f, l, r)
    I lower = lower_bound(f, l, a, r);
    I upper = upper_bound(lower, l, a, r);
    return std::pair<I, I>(lower, upper);
  }

  // Bidirectional iterators

  template<typename I>
  requires(BidirectionalIterator(I) && Readable(I))
  I operator-(I l, DistanceType(I) n)
  {
    // Precondition: n >= 0 & Exists f in I => weak_range(f, l) & l = f + n
    while (!zero(n)) {
      l = predecessor(l);
      n = predecessor(n);
    }
    return l;
  }

  template<typename I, typename P>
  requires(Readable(I) && BidirectionalIterator(I) && UnaryPredicate(P) &&
    ValueType(I) == Domain(P))
  I find_backward_if(I f, I l, P p) 
  {
    // Precondition: (f, l] is a readable bounde half-open on left range
    I i = l;
    while (f != l && (i = predecessor(i), !p(source(i)))) {
      l = i;
    }
    return l;
  }

  // incomplete
  template<typename I>
    requires(BidirectionalIterator(I))
  struct reverse_iterator {
    reverse_iterator() {}
    reverse_iterator(I _i) : i(_i) {}
    reverse_iterator(const reverse_iterator& x) : i(x.i) {}
    reverse_iterator<I>& operator=(const reverse_iterator<I>& x)
    {
      i = x.i;
      return *this;
    }
    bool operator==(const reverse_iterator<I>& x)
    {
      return i == x.i;
    }
    bool operator!=(const reverse_iterator<I>& x)
    {
      return !(i == x.i);
    }
    bool operator<(const reverse_iterator<I>& x)
    {
      return i < x.i;
    }
    bool operator>(const reverse_iterator<I>& x)
    {
      return x.i < i;
    }
    reverse_iterator<I>& operator++()
    {
      ++i;
      return *this;
    }
    reverse_iterator<I> operator++(int)
    {
      reverse_iterator<I> r(i);
      ++i;
      return r;
    }
    I current()
    {
      return i;
    }
  private:
    I i;
  };

  template<typename I>
    requires(BidirectionalIterator(I))
  reverse_iterator<I> successor(reverse_iterator<I> r)
  {
    return reverse_iterator<I>(predecessor(r.current()));
  }

  template<typename I>
    requires(BidirectionalIterator(I))
  reverse_iterator<I> predecessor(reverse_iterator<I> r)
  {
    return reverse_iterator<I>(successor(r.current()));
  }

  template<typename I>
    requires(BidirectionalIterator(I) && Readable(I))
  ValueType(I) source(reverse_iterator<I> r)
  {
    return source(predecessor(r.current()));
  }

  template<typename I>
    requires(BidirectionalIterator(I) && Readable(I))
  bool is_palindrom(I f, I l)
  {
    // Precondition: readable_bounded_range(f, l)
    while (f != l && source(f) == source(predecessor(l))) {
      f = successor(f);
      if (f == l) return true;
      l = predecessor(l);
    }
    return f == l;
    // Postcondition: returns true, if the provided range is a palindrom
  }

  // Chapter 7 - Coordinate Structures
  template<typename C>
    requires(BifurcateCoordinate(C))
  WeightType(C) weight_recursive(C c)
  {
    // Precondition: tree(c)
    typedef WeightType(C) N;
    if (empty(c)) return N{ 0 };
    N l{ 0 };
    N r{ 0 };
    if (has_left_successor(c)) {
      l = weight_recursive(left_successor(c));
    }
    if (has_right_successor(c)) {
      r = weight_recursive(right_successor(c));
    }
    return successor(l + r);
  }

  template<typename C>
    requires(BifurcateCoordinate(C))
  WeightType(C) height_recursive(C c) 
  {
    // Precondition: tree(c)
    typedef WeightType(C) N;
    if (empty(c)) return N{ 0 };
    N l{ 0 };
    N r{ 0 };
    if (has_left_successor(c)) {
      l = height_recursive(left_successor(c));
    }
    if (has_right_successor(c)) {
      r = height_recursive(right_successor(c));
    }
    return successor(select_1_2(l, r, std::less<N>()));
  }

  enum class visit {pre, in, post};

  template<typename C, typename Proc>
  requires(BifurcateCoordinate(C) && 
    Procedure(Proc) && Arity(Proc) == 2 && 
    visit == InputType(Proc, 0) && C == InputType(Proc, 1))
  Proc traverse_nonempty(C c, Proc proc)
  {
    // Precondition: tree(c) && !empty(c)
    proc(visit::pre, c);
    if (has_left_successor(c)) {
      proc = traverse_nonempty(left_successor(c), proc);
    }
    proc(visit::in, c);
    if (has_right_successor(c)) {
      proc = traverse_nonempty(right_successor(c), proc);
    }
    proc(visit::post, c);
    return proc;
  }

  template<typename C>
    requires(BifurcateCoordinate(C))
  bool is_left_successor(C c)
  {
    // Precondition: has_predecessor(c)
    return c == left_successor(predecessor(c));
  }

  template<typename C>
    requires(BifurcateCoordinate(C))
  bool is_right_successor(C c)
  {
    // Precondition: has_predecessor(c)
    return c == right_successor(predecessor(c));
  }

  template<typename C>
    requires(BifurcateCoordinate(C))
  WeightType(C) traverse_step(C& c, visit& v) 
  {
    // Precondition: !root(c) || v != visit.post
    switch (v)
    {
      case visit::pre:
      if (has_left_successor(c)) {
          c = left_successor(c);    return 1;
      }   v = visit::in;            return 0;
      case visit::in:
      if (has_right_successor(c)) {
        v = visit::pre; c = right_successor(c); 
                                    return 1;
      } v = visit::post;            return 0;
      case visit::post:
      if (is_left_successor(c))
        v = visit::in;
        c = predecessor(c);         return -1;
    }
  }

  template<typename C>
    requires(BidirectionalBifurcateCoordinate(C))
  bool reachable(C x, C y)
  {
    // Precondition: tree(x)
    if (empty(x)) return false;
    C root = x;
    visit v = visit::pre;
    do {
      if (x == y) return true;
      traverse_step(x, v);
    } while (x != root || v != visit::post);
    return false;
  }

  template<typename C>
    requires(BidirectionalBifurcateCoordinate(C))
  WeightType(C) weight(C c)
  {
    // Precondition: tree(c)
    typedef WeightType(C) N;
    if (empty(c)) return N{ 0 };

    C root = c;
    visit v = visit::pre;
    N n{ 1 }; // Invarian: n counts the number of pre visits so far
    do {
      traverse_step(c, v);
      if (v == visit::pre) n = successor(n);
    } while (c != root || v != visit::post);
    return n;
  }

  template<typename C>
    requires(BidirectionalBifurcateCoordinate(C))
  WeightType(C) height(C c)
  {
    //Precondition: tree(c)
    typedef WeightType(C) N;
    if (empty(c)) return N{ 0 };

    C root = c;
    visit v = visit::pre;
    N n{ 1 }; // Invariant: n is the height of the current pre visit
    N m{ 1 }; // Invariant: m is the max of height of pre visits so far
    do {
      n = (n - N{1 }) + N{ traverse_step(c, v) + 1 };
      if (m < n) m = n;
    } while (c != root || v != visit::post);
    return m;
  }

  template<typename C, typename Proc>
  requires(BidirectionalBifurcateCoordinate(C) &&
    Procedure(Proc) && Arity(Proc) == 2 &&
    InputType(Proc, 0) == eop::visit && InputType(Proc, 1) == C)
  Proc traverse(C c, Proc proc)
  {
    // Precondition: tree(c)
    if (empty(c)) return proc;
    C root = c;
    visit v = visit::pre;
    proc(v, c);
    do {
      traverse_step(c, v);
      proc(v, c);
    } while (c != root || v != visit::post);
    return proc;
  }

  // Exercise 7.2 : use traverse_step and the procedures of Chapter 2
  //    to determine whether the descendants of a bidirectional
  //    bifurcate coordinate form a DAG

  template<typename C, typename T>
    requires(BidirectionalBifurcateCoordinate(C) && FunctionTrait(T))
  struct traverse_step_pair;

  template<typename C>
    requires(BidirectionalBifurcateCoordinate(C))
  struct traverse_step_pair<C, transformation_trait>
  {
    std::pair<C, visit> operator()(const std::pair<C, visit>& p)
    {
      C c = p.first;
      visit v = p.second;
      traverse_step(c, v);
      return std::make_pair(c, v);
    }
  };

  template<typename C>
    requires(BidirectionalBifurcateCoordinate(C))
  struct traverse_step_pair<C, action_trait>
  {
    void operator()(std::pair<C, visit>& p)
    {
      traverse_step(p.first, p.second);
    }
  };

  template<typename C, typename Trait>
    requires(BidirectionalBifurcateCoordinate(C))
  struct input_type<eop::traverse_step_pair<C, Trait>, 0>
  {
    typedef std::pair<C, visit> type;
  };

  template<typename C, typename Trait>
    requires(BidirectionalBifurcateCoordinate(C))
  struct function_trait<eop::traverse_step_pair<C, Trait>>
  {
    typedef Trait trait;
  };

  template<typename C>
    requires(BidirectionalBifurcateCoordinate(C))
  struct bidirectional_bifurcate_coordinate_termination_condition   
  {
    const C root;
    bidirectional_bifurcate_coordinate_termination_condition(C root = C{ 0 }) : root(root) {}
    bool operator()(const std::pair<C, visit>& p)
    {
      return p.first != root || p.second != visit::post;
    }
  };

  template<typename C, typename Trait = action_trait>
    requires(BidirectionalBifurcateCoordinate(C) && FunctionTrait(Trait))
  bool is_dag(C c, Trait trait = Trait{})
  {
    // Precondition: tree(c)
    return !terminating(std::make_pair(c, visit::pre),
        traverse_step_pair<C, Trait> {},
          bidirectional_bifurcate_coordinate_termination_condition<C>{c});
  }

  template<typename C0, typename C1>
    requires(BifurcateCoordinate(C0) && BifurcateCoordinate(C1))
  bool bifurcate_isomorphic_nonempty(C0 c0, C1 c1)
  {
    // Preconditions: !empty(c0) && !empty(c1)
    if (has_left_successor(c0)) {
      if (has_left_successor(c1)) {
        if (!bifurcate_isomorphic_nonempty(left_successor(c0), left_successor(c1)))
          return false;
      } else  return false;
    }
    else if (has_left_successor(c1)) return false;
    
    if (has_right_successor(c0)) {
      if (has_right_successor(c1)) {
        if (!bifurcate_isomorphic_nonempty(right_successor(c0), right_successor(c1)))
          return false;
      } else  return false;
    }
    else if (has_right_successor(c1)) return false;

    return true;
  }

  template<typename C0, typename C1>
    requires(BidirectionalBifurcateCoordinate(C0) && BidirectionalBifurcateCoordinate(C1))
  bool bifurcate_isomorphic(C0 c0, C1 c1)
  {
    if (empty(c0)) return empty(c1);
    if (empty(c1)) return false;
    C0 root0 = c0;
    visit v0 = visit::pre;
    visit v1 = visit::pre;
    do {
      traverse_step(c0, v0);
      traverse_step(c1, v1);
      if (v0 != v1) return false;
    } while (c0 != root0 || v0 != visit::post);

    return true;
  }

  template<typename I0, typename I1, typename R>
  requires(Readable(I0) && Iterator(I0) &&
    Readable(I1) && Iterator(I1) && Relation(R) &&
    ValueType(I0) == ValueType(I1) &&
    ValueType(I0) == Domain(R))
  bool lexicograpical_equivalent(I0 f0, I0 l0, I1 f1, I1 l1, R r)
  {
    // Precondition: readable_Bounded_range(f0, l0)
    // Precondition: readable_Bounded_range(f1, l1)
    // Precondition: equivalence(r)
    std::pair<I0, I1> p = find_mismatch(f0, l0, f1, l1, r);
    return p.first == l0 && p.second == l1;
  }

  template<typename T>
    requires(Regular(T))
  struct equal
  {
    bool operator()(const T& a, const T& b)
    {
      return a == b;
    }
  };

  template<typename I0, typename I1>
  requires(Readable(I0) && Iterator(I0) &&
    Readable(I1) && Iterator(I1) &&
    ValueType(I0) == ValueType(I1))
  bool lexicographical_equal(I0 f0, I0 l0, I1 f1, I1 l1)
  {
    return lexicograpical_equivalent(f0, l0, f1, l1, equal<ValueType(I0)>());
  }

  template<typename C0, typename C1,
    typename R>
  requires(Readable(C0) && BifurcateCoordinate(C0) &&
    Readable(C1) && BifurcateCoordinate(C1) &&
    Relation(R) &&
    ValueType(C0) == ValueType(C1) &&
    ValueType(C0) == Domain(R))
  bool bifurcate_equivalent_nonempty(C0 c0, C1 c1, R r)
  {
    // Precondition: readable_tree(c0) && readable_tree(c1)
    // Precondition: !empty(c0) && !empty(c1)
    if (!r(source(c0), source(c1)))
    return false;
    if (has_left_successor(c0)) {
      if (has_left_successor(c1)) {
        if (!bifurcate_equivalent_nonempty(left_successor(c0), left_successor(c1), r))
          return false;
      } else  return false;
    }
    else if (has_left_successor(c1))
    return   false;

    if (has_right_successor(c0)) {
      if (has_right_successor(c1)) {
        if (!bifurcate_equivalent_nonempty(right_successor(c0), right_successor(c1), r))
          return false;
      }
      else  return false;
    }
    else if (has_right_successor(c1))
      return false;
    return true;
  }

  template<typename C0, typename C1>
    requires(Readable(C0) && BifurcateCoordinate(C0) &&
    Readable(C1) && BifurcateCoordinate(C1) &&
    ValueType(C0) == ValueType(C1))
  bool bifurcate_equal_nonempty(C0 c0, C1 c1)
  {
    // Precondition: readable_tree(c0) && readable_tree(c1)
    // Precondition: !empty(c0) && !empty(c1)
    return bifurcate_equivalent_nonempty(c0, c1, equal<ValueType(C0)>());
  }

  template<typename C0, typename C1, typename R>
  requires(Readable(C0) && BidirectionalBifurcateCoordinate(C0) &&
    Readable(C1) && BidirectionalBifurcateCoordinate(C1) &&
    Relation(R) &&
    ValueType(C0) == ValueType(C1)
    ValueType(C0) == Domain(R))
  bool bifurcate_equivalent(C0 c0, C1 c1, R r)
  {
    // Precondition: readable_tree(c0) && readable_tree(c1)
    if (empty(c0)) return empty(c1);
    if (empty(c1)) return false;
    C0 root0 = c0;
    visit v0 = visit::pre;
    visit v1 = visit::pre;

    while (true) {
      if (v0 == visit::pre && !r(source(c0), source(c1)))
        return false;
      traverse_step(c0, v0);
      traverse_step(c1, v1);
      if (v0 != v1) return false;
      if (c0 == root0 && v0 == visit::post) return true;
    }
  }

  template<typename C0, typename C1>
  requires(Readable(C0) && BidirectionalBifurcateCoordinate(C0) &&
    Readable(C1) && BidirectionalBifurcateCoordinate(C1) &&
    ValueType(C0) == ValueType(C1))
  bool bifurcate_equal(C0 c0, C1 c1)
  {
    // Precondition: readable_tree(c0) && readable_tree(c1)
    return bifurcate_equivalent(c0, c1, equal<ValueType(C0)>());
  }

  template<typename I0, typename I1, typename R>
  requires(Readable(I0) && Iterator(I0) &&
    Readable(I1) && Iterator(I1) &&
    Relation(R) &&
    ValueType(I0) == ValueType(I1) &&
    ValueType(I0) == Domain(R))
  bool lexicograpichal_compare(I0 f0, I0 l0, I1 f1, I1 l1, R r)
  {
    // Precondition: readable_bounded_range(f0, l0)
    // Precondition: readable_bounded_range(f1, l1)
    // weak_ordering(r)
    while (true) {
      if (f1 == l1) return false;
      if (f0 == l0) return true;
      if (r(source(f0), source(f1))) return true;
      if (r(source(f1), source(f0))) return false;
      f0 = successor(f0);
      f1 = successor(f1);
    }
  }

  template<typename T>
    requires(TotallyOrdered(T))
  struct less 
  {
    bool operator()(const T& x, const T& y)
    {
      return x < y;
    }
  };

  template<typename I0, typename I1>
  requires(Readable(I0) && Iterator(I0) &&
    Readable(I1) && Iterator(I1))
  bool lexicograpichal_less(I0 f0, I0 l0, I1 f1, I1 l1)
  {
    return lexicographical_compare(f0, l0, f1, l1, less <ValueType(I0)>());
  }

  enum class comparison { less, equal, greater };

  template<typename C0, typename C1, typename R>
  requires(BifurcateCoordinate(C0) && Readable(C0) &&
    BifurcateCoordinate(C1) && Readable(C1) &&
    Relation(R) &&
    ValueType(C0) == ValueType(C1) &&
    ValueType(C0) == Domain(R))
  comparison bifurcate_compare_nonempty_recursive(C0 c0, C1 c1, R r) {
    // Precondition: readable_tree(c0) && readable_tree(c1)
    // Precondition: !empty(c0) && !empty(c1)
    if (r(source(c0), source(c1))) return comparison::less;
    if (r(source(c1), source(c0))) return comparison::greater;
    if (has_left_successor(c0)) {
      if (has_left_successor(c1)) {
        comparison c = bifurcate_compare_nonempty_recursive(left_successor(c0), left_successor(c1), r);
        if (c != comparison::equal) return c;
      }
      else return comparison::greater;
    }
    else if (has_left_successor(c1)) {
      return comparison::less;
    }

    if (has_right_successor(c0)) {
      if (has_right_successor(c1)) {
        comparison c = bifurcate_compare_nonempty_recursive(right_successor(c0), right_successor(c1), r);
        if (c != comparison::equal) return c;
      }
      else return comparison::greater;
    }
    else if (has_right_successor(c1)) {
      return comparison::less;
    }
    return comparison::equal;
  }

  template<typename C0, typename C1, typename R>
  requires(BifurcateCoordinate(C0) && Readable(C0) &&
    BifurcateCoordinate(C1) && Readable(C1) &&
    Relation(R) &&
    ValueType(C0) == ValueType(C1) &&
    ValueType(C0) == Domain(R))
  bool bifurcate_compare_nonempty(C0 c0, C1 c1, R r)
  {
    return bifurcate_compare_nonempty_recursive(c0, c1, r) == comparison::less;
  }

  template<typename C0, typename C1>
  requires(BifurcateCoordinate(C0) && Readable(C0) &&
    BifurcateCoordinate(C1) && Readable(C1) &&
    ValueType(C0) == ValueType(C1))
  bool bifurcate_less_nonempty(C0 c0, C1 c1)
  {
    return bifurcate_compare_nonempty(c0, c1, less<ValueType(C0)>());
  }

  template<typename C0, typename C1, typename R>
  requires(BidirectionalBifurcateCoordinate(C0) && Readable(C0) &&
    BidirectionalBifurcateCoordinate(C1) && Readable(C1) &&
    Relation(R) &&
    ValueType(C0) == ValueType(C1) &&
    ValueType(C0) == Domain(R))
  bool bifurcate_compare(C0 c0, C1 c1, R r)
  {
    // Precondition: readable_tree(c0) && readable_tree(c1)
    if (empty(c1)) return false;
    if (empty(c0)) return true;
    C0 root0 = c0;
    visit v0 = visit::pre;
    visit v1 = visit::pre;
    while (true) {
      if (v0 == visit::pre) {
        if (r(source(c0), source(c1))) return true;
        if (r(source(c1), source(c0))) return false;
    }
    traverse_step(c0, v0);
    traverse_step(c1, v1);
    if (v0 != v1) return v0 > v1;
    if (c0 == root0 && v0 == visit::post) return false;
    }
  }

  template<typename C0, typename C1>
  requires(BidirectionalBifurcateCoordinate(C0) && Readable(C0) &&
    BidirectionalBifurcateCoordinate(C1) && Readable(C1) &&
    ValueType(C0) == ValueType(C1))
  bool bifurcate_less(C0 c0, C1 c1)
  {
    return bifurcate_compare(c0, c1, less<ValueType(C0)>());
  }


  // Exercise 7.6 (copied)
  template<typename R>
    requires(Relation(R))
  struct comparator_3_way
  {
    typedef Domain(R) T;
    R r;
    comparator_3_way(R r) : r(r)
    {
      // Precondition: $\property{weak\_ordering}(r)$
      // Postcondition: three_way_compare(comparator_3_way(r))
    }
    int operator()(const T& a, const T& b)
    {
      if (r(a, b)) return 1;
      if (r(b, a)) return -1;
      return 0;
    }
  };

  //
  // Chapter 8  - Link Rearrangements
  //

  template<typename I>
    requires(LinkedForwardIterator(I))
  struct forward_linker
  {
    void operator()(I& x, I& y) const
    {
      sink(x.ptr).forward_link = y.ptr;
    }
  };

  template<typename I>
    requires(LinkedForwardIterator(I))
  struct iterator_type<forward_linker<I>>
  {
    typedef I type;
  };

  template<typename I>
    requires(LinkedBidirectionalIterator(I))
  struct backward_linker
  {
    void operator()(I& x, I& y)
    {
      sink(y.ptr).backward_link = x.ptr;
    }
  };

  template<typename I>
    requires(LinkedBidirectionalIterator)
  struct iterator_type<backward_linker<I>>
  {
    typedef I type; 
  };

  template<typename I>
    requires(LinkedBidirectionalIterator(I))
  struct bidirectional_linker
  {
    void operator()(I& x, I& y)
    {
      forward_linker<I>()(x, y);
      backward_linker<I>()(x, y);
    }
  };

  template<typename   I>
    requires(LinkedBidirectionalIterator)
  struct iterator_type<bidirectional_linker<I>>
  {
    typedef I type;
  };

  template<typename I>
    requires(ForwardIterator(I))
  void advance_tail(I& t, I& f)
  {
    // Precondition: successor(f) is defined
    t = f;
    f = successor(f);
  }

  template<typename S>
  requires(ForwardLinker(S))
  struct linker_to_tail 
  {
    typedef IteratorType<S> I;
    const S set_link;
    linker_to_tail(const S& set_link) : set_link(set_link) {}
    void operator()(I& t, I& f) const {
      // Precondition: successor(f) is defined
      set_link(t, f);
      advance_tail(t, f);
    }
  };
  
  template<typename I>
  requires(ForwardIterator(I))
  I find_last(I f, I l)
  {
    // Precondition: bounded_range(f, l) & f != l
    I t;
    do
      advance_tail(t, f);
    while (f != l);
    return t;
  }

  template<typename I, typename S, typename Pred>
    requires(ForwardIterator(I) && ForwardLinker(S) &&
    IteratorType(S) == I && UnaryPseudoPredicate(Pred) &&
    I == Domain(Pred))
  std::pair<std::pair<I, I>, std::pair<I, I>>
  split_linked(I f, I l, Pred p, S set_link)
  {
    // Precondition: bounded_range(f, l)
    typedef std::pair<I, I> P;
    linker_to_tail<S> link_to_tail(set_link);
    I h0 = l; I t0 = l;
    I h1 = l; I t1 = l;
    if (f == l)      goto s4;
    if (p(f)) { h1 = f; advance_tail(t1, f); goto s1; }
    else      { h0 = f; advance_tail(t0, f); goto s0; }

    s0: if (f == l)  goto s4;
    if (p(f)) { h1 = f; advance_tail(t1, f); goto s3; }
    else      {         advance_tail(t0, f); goto s0; }

    s1: if (f == l) goto s4;
    if (p(f)) {         advance_tail(t1, f); goto s1; }
    else      { h0 = f; advance_tail(t0, f); goto s2; }

    s2: if (f == l) goto s4;
    if (p(f)) {         link_to_tail(t1, f); goto s3; }
    else      {         advance_tail(t0, f); goto s2; }

    s3: if (f == l) goto s4;
    if (p(f)) {         advance_tail(t1, f); goto s3; }
    else      {         link_to_tail(t0, f); goto s2; }

    s4: return std::pair<P, P>(P(h0, t0), P(h1, t1));
  }

  template<typename I, typename S, typename R>
    requires(ForwardLinker(S) && I == IteratorType(S) &&
    PseudoRelation(R) && I == Domain(R))
  std::tuple<I, I, I>
  combine_linked_nonempty(I f0, I l0, I f1, I l1, R r, S set_link)
  {
    linker_to_tail<S> link_to_tail(set_link);
    I h; I t;
    if (r(f1, f0)) { h = f1; advance_tail(t, f1); goto s1; }
    else           { h = f0; advance_tail(t, f0); goto s0; }

    s0: if (f0 == l0)       goto s2;
    if (r(f1, f0)) {         link_to_tail(t, f1); goto s1; }
    else           {         advance_tail(t, f0); goto s0; }

    s1: if (f1 == l1)       goto s3;
    if (r(f1, f0)) {         advance_tail(t, f1); goto s1; }
    else           {         link_to_tail(t, f0); goto s0; }

    s2: set_link(t, f1); return std::make_tuple(h, t, l1);

    s3: set_link(t, f0); return std::make_tuple(h, t, l0);
  }


  template<typename I, typename S, typename R>
    requires(ForwardLinker(S) && I == IteratorType(S) &&
    PseudoRelation(R) && I == Domain(R))
  std::tuple<I, I, I>
  combine_linked(I f0, I l0, I f1, I l1, R r, S set_link)
  {
    if (f0 == l0) return std::make_tuple(f1, f1, l1);
    if (f1 == l1) return std::make_tuple(f0, f0, l0);
    return combine_linked_nonempty(f0, l0, f1, l1, r, set_link);
  }

  template<typename I, typename S>
    requires(ForwardLinker(S) && I == IteratorType(S))
  struct linker_to_head
  {
    S set_link;
    linker_to_head(const S& set_link) : set_link(set_link) {}
    void operator()(I& h, I& f)
    {
      // Precondition: successor(f) is defined_test_names_
      IteratorType<S> tmp = successor(f);
      set_link(f, h);
      h = f;
      f = tmp;
    }
  };

  template<typename I, typename S>
    requires(ForwardLinker(S) && I == IteratorType(S))
  I reverse_append(I f, I l, I h, S set_link)
  {
    linker_to_head<I, S> link_to_head(set_link);
    while(f != l) link_to_head(h, f);
    return h;
  }

  // 8.3 Applications of Link Rearrangements

  // Adapter to convert from predicate on values to predicate on iterators
  template<typename I, typename P>
    requires(Readable(I) && Predicate(P)
    && ValueType(I) == Domain(P))
  struct predicate_source
  {
    P p;
    predicate_source(P p) : p(p) {}
    bool operator()(I i) {
      return p(source(i));
    }
  };

  template<typename I, typename S, typename P>
    requires(ForwardLinker(S) && I == IteratorType(S) &&
    UnaryPredicate(P) && ValueType(I) == Domain(P))
  std::pair<std::pair<I, I>, std::pair<I, I>>
  partition_linked(I f, I l, P p, S set_link)
  {
    predicate_source<I, P> ps(p);
    return split_linked(f, l, ps, set_link);
  }

  // Adapter to convert from relation on values to relation on iterators
  template<typename I0, typename I1, typename R>
  requires(Readable(I0) && Readable(I1) &&
    ValueType(I0) == ValueType(I1) && Relation(R) &&
    ValueType(I0) == Domain(R))
  struct relation_source
  {
    R r;
    relation_source(R r) : r(r) {}
    bool operator()(I0 i0, I1 i1)
    {
      return r(source(i0), source(i1));
    }
  };

  template<typename I, typename S, typename R>
    requires(Readable(I) && ForwardLinker(S) && I == IteratorType(S)
    && Relation(R) && ValueType(I) == Domain(R))
  std::pair<I, I> merge_linked_nonempty(I f0, I l0, I f1, I l1, R r, S set_link)
  {
    // Precondition: f0 != l0 & f1 != l1
    // Precondition: increasing_range(f0, l0, r)
    // Precondition: increasing_range(f1, l1, r)
    relation_source<I, I, R> rs(r);
    std::tuple<I, I, I> t = combine_linked_nonempty(f0, l0, f1, l1, rs, set_link);
    I last = find_last(std::get<1>(t), std::get<2>(t));
    set_link(last, l1);
    return std::pair<I, I>(std::get<0>(t), l1);
  }

  template<typename I, typename S, typename R>
    requires(Readable(I) && ForwardLinker(S) && I == IteratorType(S)
    && Relation(R) && ValueType(I) == Domain(R))
  std::pair<I, I> sort_linked_nonempty_n(I f, DistanceType(I) n, R r, S set_link)
  {
    // Precondition: counted_range(f, n) && 0 < n && weak_oredering(r)
    
    typedef DistanceType(I) N;
    typedef std::pair<I, I> P;
    if (n == N(1)) return P(f, successor(f));
    N h = half_nonnegative(n);
    P p0 = sort_linked_nonempty_n(f, h, r, set_link);
    P p1 = sort_linked_nonempty_n(p0.second, n - h, r, set_link);
    return merge_linked_nonempty(p0.first, p0.second, p1.first, p1.second, r, set_link);
  }

  // Merge sort analysis
  // Worst-case of calling relation R
  // T(N) = C*N + 2*T(N/2) = C*N + 2*(C*N/2 + 2*T(N/4)) = C*N + C*N + 4*(C*N/4 + 2*T(N/8)) = lg(N)*C*N = O(N*lgN) 
  // Worst-case of calling relinking - when always call set_link after calling relation_source
  // o(N*lgN)

  template<typename I, typename S, typename R>
    requires(Readable(I) && ForwardLinker(S) && I == IteratorType(S)
    && Relation(R) && ValueType(I) == Domain(R))
  std::pair<I, I> sort_linked_n(I f, DistanceType(I) n, R r, S set_link)
  {
    typedef DistanceType(I) N;
    if (n == N(0)) return std::pair<I, I>(f, f);
    return sort_linked_nonempty_n(f, n, r, set_link);
  }

  template<typename I, typename R>
    requires(Readable(I) && Relation(R) && ValueType(I) == Domain(R))
  std::pair<I, I> sort_bidirectioanl_linked_nonempty_n(I f, DistanceType(I) n, R r)
  {
    std::pair<I, I> p = sort_linked_nonempty_n(f, n, r, forward_linker<I>());
    f = p.first;
    while(f != p.second) { 
      set_backward_link(f, successor(f)); 
      f = sucessor(f); 
    }
    return p;
  }

  template<typename I, typename R>
    requires(Readable(I) && Iterator(I) && Relation(R) && ValueType(I) == Domain(R))
  struct equals_last_predicate
  {
    const R r;
    I last_value;
    equals_last_predicate(R const& r) : r(r), last_value(I{}) {}
    bool operator()(I i) {
      bool result = !empty(last_value) && r(source(last_value), source(i));
      last_value = i;
      return result;
    }
  };

  template<typename I, typename R, typename S>
    requires(Readable(I) && Iterator(I) && Relation(R) &&
    ValueType(I) == Domain(R) && I == IteratorType(S))
  std::pair<std::pair<I, I>, std::pair<I, I>>
  unique(I f, I l, R r, S set_link)
  {
    equals_last_predicate<I, R> elp(r);
    return split_linked(f, l, elp, set_link);
  }

  // Chapter 8.4 Linked Bifurcate Coordinates

  template<typename C>
  requires(EmptyLinkedBifurcateCoordinate(C))
  void tree_rotate(C& curr, C& prev)
  {
    C tmp = left_successor(curr);
    set_left_successor(curr, right_successor(curr));
    set_right_successor(curr, prev);
    if (empty(tmp)) { prev = tmp; return; }
    prev = curr;
    curr = tmp;
  }

  template<typename C, typename Proc>
    requires(EmptyLinkedBifurcateCoordinate(C) && Procedure(Proc) && 
    Arity(Proc) == 1 && C == InputType(Proc))
  Proc traverse_rotating(C c, Proc proc)
  {
    visit v = visit::pre;
    // Precondition: tree(c)
    if (empty(c)) return proc;
    C curr = c;
    C prev;
    do {
      proc(curr);
      tree_rotate(curr, prev);
    } while (curr != c);
    do {
      proc(curr);
      tree_rotate(curr, prev);
    } while (curr != c);
    proc(curr);
    tree_rotate(curr, prev);
    return proc;
  }

  template<typename T, typename N>
  struct counter
  {
    N n;
    counter(N n = N(0)) : n(n) {}
    void operator()(T const&) { n = successor(n); }
  };

  template<typename C>
    requires(EmptyLinkedBifurcateCoordinate(C))
  WeightType(C) weight_rotating(C c)
  {
    return traverse_rotating(c, counter<C, WeightType(C)>()).n / 3;
  }

  template<typename N, typename Proc>
  requires(Integer(N) && Procedure(Proc) && Arity(Proc) == 1)
  struct phased_applicator 
  {
    N period;
    N phase;
    N n;
    // Invariant: n, phase E [0, period)
    Proc proc;
    phased_applicator(N period, N phase, N n, Proc proc) 
    : period(period), phase(phase), n(n), proc(proc) {}
    void operator()(InputType(Proc, 0) c) {
      if (n == phase) proc(c);
      n = successor(n);
      if (n == period) n = 0;
    }
  };

  template<typename C, typename Proc>
    requires(EmptyLinkedBifurcateCoordinate(C) && Procedure(Proc) && Arity(Proc) == 1 &&
      InputType(Proc, 0) == C)
  Proc traverse_phased_rotating(C c, int phase, Proc proc)
  {
    // Precondition: tree(c)
    phased_applicator<int, Proc> applicator(3, phase, 0, proc);
    return traverse_rotating(c, applicator).proc;
  }

  // *******************************************************
  // Chapter 9 - Copying
  // *******************************************************

  template<typename I, typename O>
    requires(Readable(I) && Iterator(I) &&
	     Writable(O) && Iterator(O) &&
	     ValueType(I) == ValueType(O))
  void copy_step(I & f_i, O & f_o)
  {
    // Precondition: source(f_i) and sink(f_0) are defined
    sink(f_o) = source(f_i);
    f_i = successor(f_i);
    f_o = successor(f_o);
  }

  template<typename I, typename O>
    requires(Readable(I) && Iterator(I) &&
	     Writable(O) && Iterator(O) &&
	     ValueType(I) == ValueType(O))
  O copy(I f_i, I l_i, O f_o)
  {
    // Precondition: not_overlapped_forward(fi, li, fo, fo+(li-fi))
    while (f_i != l_i) copy_step(f_i, f_o);
    return f_o;
  }

  template<typename I, typename O>
    requires(Readable(I) && Iterator(I) &&
	     Readable(O) && Iterator(O) &&
	     ValueType(I) == ValueType(O))
  std::pair<I, O> copy_bounded(I f_i, I l_i, O f_o, O l_o)
  {
    // Precondition: not_overlapped_forward(fi, li, fo, lo)
    while(f_i != l_i && f_o != l_o) copy_step(f_i, f_o);
    return std::make_pair(f_i, f_o);
  }

  template<typename N>
    requires(Integer(N))
  bool count_down(N& n)
  {
    // Precondition: n >= 0
    if (zero(n)) return false;
    n = predecessor(n);
    return true;
  }

  template<typename I, typename O, typename N>
    requires(Readable(I) && Iterator(I) &&
	     Readable(O) && Iterator(O) &&
	     ValueType(I) == ValueType(O))
  std::pair<I, O> copy_n(I f_i, N n, O f_o)
  {
    // Precondition: non_overlapped_forward(fi, fi+n, fo, fo+n)
    while(count_down(n)) copy_step(f_i, f_o);
    return std::make_pair(f_i, f_o);
  }

  template<typename I, typename O>
    requires(Readable(I) && BidirectionalIterator(I) &&
	     Readable(O) && BidirectionalIterator(O) &&
	     ValueType(I) == ValueType(O))
  void copy_backward_step(I& l_i, O& l_o)
  {
    // Precondition: source(predeccessor(l_i)) and sink(predecessor(l_o)) are defined
    l_i = predecessor(l_i);
    l_o = predecessor(l_o);
    sink(l_o) = source(l_i);
  }

  template<typename I, typename O>
    requires(Readable(I) && BidirectionalIterator(I) &&
	     Readable(O) && BidirectionalIterator(O) &&
	     ValueType(I) == ValueType(O))
  O copy_backward(I f_i, I l_i, O l_o)
  {
    // Precondition: not_overlapped_backward(fi, li, lo - (li - fi), lo)
    while(f_i != l_i) copy_backward_step(l_i, l_o);
    return l_o;
  }

  template<typename I, typename O, typename N>
    requires(Readable(I) && BidirectionalIterator(I) &&
	     Readable(O) && BidirectionalIterator(O) &&
	     ValueType(I) == ValueType(O) && Integer(N))
  std::pair<I, O> copy_backward_n(I l_i, N n, O l_o)
  {
    // Precondition: not_overlapped_backward(fi, li, lo - (li - fi), lo)
    while(count_down(n)) copy_backward_step(l_i, l_o);
    return std::make_pair(l_i, l_o);
  }

  template<typename I, typename O>
    requires(Readable(I) && BidirectionalIterator(I) &&
	     Readable(O) && Iterator(O) &&
	     ValueType(I) == ValueType(O))
  void reverse_copy_step(I& l_i, O& f_o)
  {
    // Precondition: source(predecessor(l_i)) and sink(f_o) are defined
    l_i = predecessor(l_i);
    sink(f_o) = source(l_i);
    f_o = successor(f_o);
  }

  template<typename I, typename O>
    requires(Readable(I) && Iterator(I) &&
	     Readable(O) && BidirectionalIterator(O) &&
	     ValueType(I) == ValueType(O))
  void reverse_copy_backward_step(I& f_i, O& l_o)
  {
    // Precondition: source(f_i) and sink(predecessor(l_o)) are defined
    l_o = predecessor(l_o);
    sink(l_o) = source(f_i);
    f_i = successor(f_i);
  }

  template<typename I, typename O>
    requires(Readable(I) && BidirectionalIterator(I) &&
	     Readable(O) && Iterator(O) &&
	     ValueType(I) == ValueType(O))
  O reverse_copy(I f_i, I l_i, O f_o)
  {
    // Precondition: not_overlapped(fi, li, fo, fo + (li - fi))
    while(f_i != l_i) reverse_copy_step(l_i, f_o);
    return f_o;
  }

  template<typename I, typename O>
    requires(Readable(I) && Iterator(I) &&
	     Readable(O) && BidirectionalIterator(O) &&
	     ValueType(I) == ValueType(O))
  O reverse_copy_backward(I f_i, I l_i, O l_o)
  {
    // Precondition: not_overlapped(fi, li, lo - (li - fi), lo)
    while(f_i != l_i) reverse_copy_backward_step(f_i, l_o);
    return l_o;
  }
  
  template<typename I, typename O, typename P>
    requires(Readable(I) && Iterator(I) &&
	     Writeable(O) && Iterator(O) &&
	     ValueType(I) == ValueType(O) &&
	     UnaryPredicate(P) && I == Domain(P))
  O copy_select(I f_i, I l_i, O f_t, P p)
  {
    // Precondtion: not_overlapped_forward(fi, li, f_t, f_t + nt)
    // where nt is the upper bound for the nuumber of iterators satisfying p
    while (f_i != l_i)
      if (p(f_i)) copy_step(f_i, f_t);
      else f_i = successor(f_i);
    return f_t;
  }

  template<typename I, typename O, typename P>
    requires(Readable(I) && Iterator(I) &&
       Writeable(O) && Iterator(O) &&
       ValueType(I) == ValueType(O) &&
       UnaryPredicate(P) && ValueType(I) == Domain(P))
  O copy_if(I f_i, I l_i, O f_t, P p)
  {
    // Precondtion: not_overlapped_forward(fi, li, f_t, f_t + n_t)
    // where n_t is the upper bound for the number of iterators satisfying p
    predicate_source<I, P> ps(p);
    return copy_select(f_i, l_i, f_t, ps);
  }

  template<typename I, typename O_f, typename O_t, typename P>
    requires(Readable(I) && Iterator(I) &&
      Writeable(O_f) && Iterator(O_f) &&
      Writeable(O_t) && Iterator(O_t) &&
      ValueType(I) == ValueType(O_f) &&
      ValueType(I) == ValueType(O_t) &&
      UnaryPredicate(P) && I == Domain(P))
  std::pair<O_f, O_t> split_copy(I f_i, I l_i, O_f f_f, O_t f_t, P p)
  {
    // Precondition: not_overlapped(f_t, f_t + n_t, f_f, f_f + n_f) &&
    //               (not_overlapped_forward(f_i, l_i, f_f, f_f + n_f) && not_overlapped(f_i, l_i, f_f, f_t + n_t)) ||
    //               (not_overlapped(f_i, l_i, f_f, f_f + n_f) && not_overlapped_forward(f_i, l_i, f_f, f_t + n_t)) ||
    // where n_t is the upper bound for the number of iterators satisfying p
    // where n_f is the upper bound for the number of iterators not satisfying p
    while(f_i != l_i)
      if (p(f_i)) copy_step(f_i, f_t);
      else        copy_step(f_i, f_f);
    return std::pair<O_f, O_t>(f_f, f_t);
  }

  template<typename I, typename O_f, typename O_t, typename P>
    requires(Readable(I) && Iterator(I) &&
      Writeable(O_f) && Iterator(O_f) &&
      Writeable(O_t) && Iterator(O_t) &&
      ValueType(I) == ValueType(O_f) &&
      ValueType(I) == ValueType(O_t) &&
      UnaryPredicate(P) && I == Domain(P))
  std::pair<O_f, O_t> split_copy_n(I f_i, DistanceType(I) n_i, O_f f_f, O_t f_t, P p)
  {
    // Precondition: not_overlapped(f_t, f_t + n_t, f_f, f_f + n_f) &&
    //               (not_overlapped_forward(f_i, l_i, f_f, f_f + n_f) && not_overlapped(f_i, l_i, f_f, f_t + n_t)) ||
    //               (not_overlapped(f_i, l_i, f_f, f_f + n_f) && not_overlapped_forward(f_i, l_i, f_f, f_t + n_t)) ||
    // where n_t is the upper bound for the number of iterators satisfying p
    // where n_f is the upper bound for the number of iterators not satisfying p
    while(!zero(n_i)) {
      if (p(f_i)) copy_step(f_i, f_t);
      else        copy_step(f_i, f_f);
      n_i = predecessor(n_i);
    }
    return std::pair<O_f, O_t>(f_f, f_t);
  }

  template<typename I, typename O_f, typename O_t, typename P>
    requires(Readable(I) && Iterator(I) &&
       Writeable(O_f) && Iterator(O_f) &&
       Writeable(O_t) && Iterator(O_t) &&
       ValueType(I) == ValueType(O_f) &&
       ValueType(I) == ValueType(O_t) &&
       UnaryPredicate(P) && I == Domain(P))
  std::pair<O_f, O_t> partition_copy(I f_i, I l_i, O_f f_f, O_t f_t, P p)
  {
    // Precondition: same as split_copy
    predicate_source<I, P> ps(p);
    return split_copy(f_i, l_i, f_f, f_t, ps);
  }

  template<typename I, typename O_f, typename O_t, typename P>
    requires(Readable(I) && Iterator(I) &&
       Writeable(O_f) && Iterator(O_f) &&
       Writeable(O_t) && Iterator(O_t) &&
       ValueType(I) == ValueType(O_f) &&
       ValueType(I) == ValueType(O_t) &&
       UnaryPredicate(P) && I == Domain(P))
  std::pair<O_f, O_t> partition_copy_n(I f_i, DistanceType(I) n_i, O_f f_f, O_t f_t, P p)
  {
    // Precondition: same as split_copy
    predicate_source<I, P> ps(p);
    return split_copy_n(f_i, n_i, f_f, f_t, ps);
  }

  template<typename I0, typename I1,  typename O, typename R>
    requires(Readable(I0) && Iterator(I0) &&
       Readable(I1) && Iterator(I1) &&
       Writable(O) && Iterator(O) &&
       BinaryPredicate(R) &&
       ValueType(I0) == ValueType(0) &&
       ValueType(I1) == ValueType(0) &&
       IO == InputType(R, 1) && I1 == InputType(R, O))
  O combine_copy(I0 f_i0, I0 l_i0, I1 f_i1, I1 l_i1, O f_o, R r)
  {
    // Precondition: (backward_offset(f_i0, l_i0, f_o, l_o, l_i1 - f_i1) && not_overlapped(f_i1, l_i1, f_o, l_o) ||
    //                backward_offset(f_i1, l_i1, f_o, l_o, l_i0 - f_i0) && not_overlapped(f_i0, l_i0, f_o, l_o) )
    while(f_i0 != l_i0 && f_i1 != l_i1) {
      if (r(f_i1, f_i0)) copy_step(f_i1, f_o);
      else               copy_step(f_i0, f_o);
    }
    return eop::copy(f_i1, l_i1, eop::copy(f_i0, l_i0, f_o));
    // Postcondition:
  }

  template<typename I0, typename I1,  typename O, typename R>
    requires(Readable(I0) && Iterator(I0) &&
       Readable(I1) && Iterator(I1) &&
       Writable(O) && Iterator(O) &&
       BinaryPredicate(R) &&
       ValueType(I0) == ValueType(0) &&
       ValueType(I1) == ValueType(0) &&
       IO == InputType(R, 1) && I1 == InputType(R, O))
  O combine_copy_backward(I0 f_i0, I0 l_i0, I1 f_i1, I1 l_i1, O f_o, R r)
  {
    while(f_i0 != l_i0 && f_i1 != l_i1)
      if (r(predecessor(l_i1), predecessor(f_i0)))
        copy_backward_step(f_i0, f_o);
      else
        copy_backward_step(f_i1, f_o);
    return copy_backward(f_i0, l_i0, copy_backward(f_i1, l_i1, f_o));
    // Postcondition: Each element in one of the input ranges appears in the output
    // range and the they are ordered by R.
  }

  template<typename I0, typename I1, typename O, typename R>
    requires(Readable(I0) && Iterator(I0) &&
      Readable(I1) && Iterator(I1) &&
      Writable(O) && Iterator(O) &&
      Relation(R) &&
      ValueType(I0) == ValueType(I1) &&
      ValueType(I0) == ValueType(O) &&
      ValueType(I0) == Domain(R))
  O merge_copy(I0 f_i0, I0 l_i0, I1 f_i1, I1 l_i1, O f_o, R r)
  {
      // Precondition: in addition to that for combine_copy
      //   weak_ordering(r) &&
      //   increasing_order(f_i0, l_i0, r) && increasing_order(f_i1, l_i1, r)
      relation_source<I0, I1, R> rs(r);
      return combine_copy(f_i0, l_i0, f_i1, l_i1, f_o, rs);
  }

  template<typename I0, typename I1, typename O, typename R>
    requires(Readable(I0) && Iterator(I0) &&
      Readable(I1) && Iterator(I1) &&
      Writable(O) && Iterator(O) &&
      Relation(R) &&
      ValueType(I0) == ValueType(I1) &&
      ValueType(I0) == ValueType(O) &&
      ValueType(I0) == Domain(R))
  O merge_copy_backward(I0 f_i0, I0 l_i0, I1 f_i1, I1 l_i1, O f_o, R r)
  {
      // Precondition: in addition to that for combine_copy_backward
      //  weak_ordering(r) &&
      //  increasing_order(f_i0, l_i0, r) && increasing_order(f_i1, l_i1, r)
      relation_source<I0, I1, R> rs(r);
      return combine_copy_backward(f_i0, l_i0, f_i1, l_i0, f_o, rs);
  }

  template<typename N, typename I0, typename I1,  typename O, typename R>
    requires(Readable(I0) && Iterator(I0) &&
       Readable(I1) && Iterator(I1) &&
       Writable(O) && Iterator(O) &&
       BinaryPredicate(R) &&
       ValueType(I0) == ValueType(0) &&
       ValueType(I1) == ValueType(0) &&
       IO == InputType(R, 1) && I1 == InputType(R, O))
  std::tuple<I0, I1, O> combine_copy_n(I0 f_i0, N n_0, I1 f_i1, N n_1, O f_o, R r)
  {
    while(!zero(n_0) && !zero(n_1))
      if (r(f_i1, f_i0)) { copy_step(f_i1, f_o); n_1 = predecessor(n_1); }
      else               { copy_step(f_i0, f_o); n_0 = predecessor(n_0); }
    std::pair<I1, O> l1 = copy_n(f_i1, n_1, f_o);
    std::pair<I0, O> l0 = copy_n(f_i0, n_0, l1.second);
    return std::tuple<I0, I1, O>(l0.first, l1.first, l0.second);
  }

  template<typename Num, typename InputIterator0, typename InputIterator1,  typename OutputIterator, typename Relation>
    requires(Readable(I0) && Iterator(I0) &&
       Readable(I1) && Iterator(I1) &&
       Writable(O) && Iterator(O) &&
       BinaryPredicate(R) &&
       ValueType(I0) == ValueType(0) &&
       ValueType(I1) == ValueType(0) &&
       IO == InputType(R, 1) && I1 == InputType(R, O))
    std::tuple<InputIterator0, InputIterator1, OutputIterator> merge_copy_n(
                InputIterator0 f_i0, Num n_0, InputIterator1 f_i1, Num n_1, OutputIterator f_o, Relation r)
  {
    relation_source<InputIterator0, InputIterator1, Relation> rs(r);
    return combine_copy_n(f_i0, n_0, f_i1, n_1, f_o, rs);
  }

  template<typename N, typename I0, typename I1, typename O, typename R>
    requires(Readable(InputIterator0) && Iterator(InputIterator0) &&
	     Readable(InputIterator1) && Iterator(InputIterator1) &&
	     Writable(OutputIterator) && Iterator(OutpuIterator) &&
	     BinaryPredicate(R) &&
	     ValueType(InputIterator0) == ValueType(InputIterator1) &&
	     ValueType(InputIterator0) == ValueType(OutputIterator) &&
	     InputIterator0 == InputType(Relation, 1) &&
	     InputIterator1 == InputType(Relation, 0))
  std::tuple<I0, I1, O> combine_copy_backward_n(I0 f_i0, N n0, I1 f_i1, N n1, O l_o, R r)
  {
    // Precondition: in addition to that for combine
    while(!zero(n0) && !zero(n1))
      if (r(predecessor(f_i1), predecessor(f_i0)))
	{ copy_backward_step(f_i1, l_o); n1 = predecessor(n1); }
      else
        { copy_backward_step(f_i0, l_o); n0 = predecessor(n0); }
    std::pair<I1, O> l1 = copy_backward_n(f_i1, n1, l_o);
    std::pair<I0, O> l0 = copy_backward_n(f_i0, n0, l1.second);
    return std::tuple<I0, I1, O>(l0.first, l1.first, l0.second);
  }

  template<typename N, typename I0, typename I1, typename O, typename R>
  std::tuple<I0, I1, O> merge_copy_backward_n(I0 f_i0, N n0, I1 f_i1, N n1, O l_o, R r)
  {
    relation_source<I0, I1, R> rs(r);
    return combine_copy_backward_n(f_i0, n0, f_i1, n1, l_o, rs);
  }

/*
 * Project 9.1 Modern computing systems include highly optimized library
 * procedures for copying memory; for example, memmove and memcpy, which use
 * optimization techniques not discussed in this book. Study the procedures
 * provided on your platform, determine the techniques they use (for example,
 * loop unrolling and software pipelining), and design abstract procedures
 * expressing as many of these techniques as possible. What type requirements and
 * preconditions are necessary for each technique? What language extensions would
 * allow a compiler full flexibility to carry out these optimizations?
 */
  
 // 9.4 Swapping ranges

  template<typename I0, typename I1>
    requires(Mutable(I0) && Mutable(I1) &&
	     ValueType(I0) == ValueType(I1))
  void exchange_values(I0 x, I1 y) {
    // Precondition: deref(x) and deref(y) are defined
    ValueType(I0) t = source(x);
            sink(x) = source(y);
            sink(y) = t;
    // Postcondition: x's and y's values has been exchanged
  }

  template<typename I0, typename I1>
    requires(Mutable(I0) && ForwardIterator(I0) &&
             Mutable(I1) && ForwardIterator(I1) &&
             ValueType(I0) == ValueType(I1))
  void swap_step(I0& f0, I1& f1)
  {
    // Precondition: deref(f0) and deref(f1) are defined
    exchange_values(f0, f1);
    f0 = successor(f0);
    f1 = successor(f1);
  }

  template<typename I0, typename I1>
    requires(Mutable(I0) && Forwarditerator(I0) &&
             Mutable(I1) && ForwardIterator(I1) &&
             ValueType(I0) == ValueType(I1))
  I1 swap_ranges(I0 f0, I0 l0, I1 f1)
  {
    // Precondition: mutable_bounded_range(f0, l0)
    // Precondition: mutable_counted_range(f1, l0-f0)
    while(f0 != l0)
      swap_step(f0, f1);
    return f1;
  }

  template<typename I0, typename I1>
    requires(Mutable(I0) && Forwarditerator(I0) &&
             Mutable(I1) && ForwardIterator(I1) &&
             ValueType(I0) == ValueType(I1))
  std::pair<I0, I1> swap_ranges_bounded(I0 f0, I0 l0, I1 f1, I1 l1)
  {
    // Precondition: mutable_bounded_range(f0, l0)
    // Precondition: mutable_bounded_range(f1, l1)
    while(f0 != l0 && f1 != l1) swap_step(f0, f1);
    return std::make_pair(f0, f1);
  }

  template<typename I0, typename I1, typename N>
    requires(Mutable(I0) && Forwarditerator(I0) &&
             Mutable(I1) && ForwardIterator(I1) &&
             ValueType(I0) == ValueType(I1) &&
	     Integer(N))
  std::pair<I0, I1> swap_ranges_n(I0 f0, I1 f1, N n)
  {
    // Precondition: mutable_counted_range(f0, n)
    // Precondition: mutable_counted_range(f1, n)
    while(count_down(n)) swap_step(f0, f1);
    return std::make_pair(f0, f1);
  }

  template<typename I0, typename I1>
    requires(Mutable(IO) && BidirectionalIterator(I0) &&
	     Mutable(I1) && ForwardIterator(I1) &&
	     ValueType(I0) == ValueType(I1))
  void reverse_swap_step(I0& l0, I1& f1)
  {
    l0 = predecessor(l0);
    exchange_values(l0, f1);
    f1 = successor(f1);
  }

  template<typename I0, typename I1>
    requires(Mutable(IO) && BidirectionalIterator(I0) &&
	     Mutable(I1) && ForwardIterator(I1) &&
	     ValueType(I0) == ValueType(I1))
   I1 reverse_swap_ranges(I0 f0, I0 l0, I1 f1)
   {
     // Precondition: mutable_bounded_range(f0, l0)
     // Precondition: mutable_counted_range(f1, l0 - f0)
     while(f0 != l0) reverse_swap_step(l0, f1);
     return f1;
   }

  template<typename I0, typename I1>
    requires(Mutable(IO) && BidirectionalIterator(I0) &&
	     Mutable(I1) && ForwardIterator(I1) &&
	     ValueType(I0) == ValueType(I1))
   std::pair<I0, I1> reverse_swap_ranges_bounded(I0 f0, I0 l0, I1 f1, I1 l1)
   {
     // Precondition: mutable_bounded_range(f0, l0)
     // Precondition: mutable_bounded_range(f1, l1)
     while(f0 != l0 && f1 != l1)
       reverse_swap_step(l0, f1);
     return std::make_pair(l0, f1);
   }

  template<typename I0, typename I1, typename N>
    requires(Mutable(I0) && Forwarditerator(I0) &&
             Mutable(I1) && ForwardIterator(I1) &&
             ValueType(I0) == ValueType(I1) &&
	     Integer(N))
  std::pair<I0, I1> reverse_swap_ranges_n(I0 l0, I1 f1, N n)
  {
    // Precondition: mutable_counted_range(l0 - n, n)
    // Precondition: mutable_counted_range(f1, n)
    while(count_down(n)) reverse_swap_step(l0, f1);
    return std::make_pair(l0, f1);
  }

  
  // *******************************************************
  // Chapter 10 - Rearrangements
  // *******************************************************

  template<typename I, typename F>
    requires(Mutable(I) && Transformation(F) && I == ValueType(F))
  void cycle_to(I i, F f)
  {
    // Precondition: The orbit of i under f is circular.
    // Precondition: For n in N deref(f^n(i)) is defined.
    I k = f(i);
    while(i != k) {
      exchange_values(i, k);
      k = f(k);
    }
  }

  template<typename T>
  void cycle_to_step(T& k, const T& t0, T& t1)
  {
    t1 = k;
    k = t0;
  }

  template<typename I, typename F>
    requires(Mutable(I) && Transformation(F) && I == ValueType(F))
  void cycle_to_2n(I i, F f)
  {
    // Precondition: The orbit of i under f is circular.
    // Precondition: For n in N deref(f^n(i)) is defined.
    I k = f(i);
    ValueType(I) t;
    while(i != k) {
      cycle_to_step(sink(k), source(i), t);
      k = f(k);
      if (i == k) break;
      cycle_to_step(sink(k), t, sink(i));
      k = f(k);
    }
  }

  template<typename I, typename F>
    requires(Mutable(I) && Transformation(F) && I == ValueType(F))
  void cycle_from(I i, F f)
  {
    // Precondition: The orbit of i under f is circular.
    // Precondition: For n in N deref(f^n(i)) is defined.
    auto tmp = source(i);
    I j = i;
    I k = f(i);
    while (i != k) {
      sink(j) = source(k);
      j = k;
      k = f(k);
    }
    sink(j) = tmp;
  }

  template<typename I, typename F>
    requires(Mutable(I) && Transformation(F) && I == ValueType(F))
  void cycle_to_marking(I i, F f, std::vector<bool>& marked)
  {
    // Precondition: The orbit of i under f is circular.
    // Precondition: For n in N deref(f^n(i)) is defined.
    I k = f(i);
    while(i != k) {
      exchange_values(i, k);
      k = f(k);
    }
  }

  template<typename I>
  struct marker
  {
    std::vector<bool> marked;
    const I first;
    marker(I first, DistanceType(I) n) : marked(std::vector<bool>(n, false)), first(first) {}
    void mark(I i) { marked[std::distance(first, i)] = true; }
    auto next() const { return find(begin(), end(), false); }
    auto begin() const { return marked.begin(); };
    auto end() const { return marked.end(); }
  };

  template<typename I>
  std::vector<bool>::iterator begin(marker<I> const& m) { return m.begin(); }

  template<typename I>
  std::vector<bool>::iterator end(marker<I> const& m) { return m.end(); }

  /*
   * Exercise 10.4 
   * Implement an algorithm that performs an arbitrary rearrangement
   * of a range of indexed iterators. Use an array of n Boolean values
   * to mark elements as they are placed, and scan this array for an
   * unmarked value to determine a representive of the next cycle.
   */
  template<typename I, typename F>
    requires()
  void cycle(I i, DistanceType(I) n, F f)
  {
    // Precondition:
    marker<I> marked(i, n);
    auto next = begin(marked);
    do {
      I j = i + std::distance(begin(marked), next);
      marked.mark(j);
      I k = f(j);
      while(k != j) {
        marked.mark(k);
        exchange_values(j, k);
        k = f(k);
      }

      next = marked.next();
    } while(next != end(marked));    
  }

  /* Exercise 10.5
   * Assuming iterators with total ordering, design an algorithm
   * that uses constant storage to determine whether an iterator
   * is a representative for a cycle; use this algorithm
   * to implement an arbitrary rearrangement.
   */
  template<typename I, typename F>
  bool is_representative(I i, F f)
  {
    // TODO: 
  }

  // 10.3 Reverse Algorithms

  template<typename I>
    requires(Mutabel(I) && RandomAccessIterator(I))
  void reverse_n_random_access(I f, DistanceType(I) n)
  {
    //Precondition: mutable_counted_range(f, n)
    DistanceType(I) h = half_nonnegative(n);
    I l = f + n;
    while(count_down(h)) exchange_values(f + h, l - 1 - h);
  }

  template<typename I>
    requires(Mutabel(I) && IndexedIterator(I))
  void reverse_n_indexed(I f, DistanceType(I) n)
  {
    //Precondition: mutable_counted_range(f, n)
    DistanceType(I) i = 0;
    n = predecessor(n);
    while(i < n) {
      exchange_values(f + i, f + n);
      i = successor(i);
      n = predecessor(n);
    }
  }

  template<typename I>
    requires(Mutable(I) && BidirectionalIterator(I))
  void reverse_bidirectional(I f, I l)
  {
    //Precondtion: mutable_bounde_range(f, l)
    while(f != l) {
      l = predecessor(l);
      if (f == l) return;
      exchange_values(f, l);
      f = successor(f);
    }
  }

  template<typename I>
    requires(Mutable(I) && BidirectionIterator(I))
  void reverse_n_bidirectional(I f, I l, DistanceType(I) n)
  {
    //Precondition: mutable_bounded_range(f, l) && 0 <= n <= l-f
    reverse_swap_ranges_n(l, f, half_nonnegative(n));
  }

  template<typename I, typename B>
    requires(Mutable(I) && ForwardIterator(I) &&
             Mutable(B) && BidirectionalIterator(B) &&
             ValueType(I) == ValueType(B))
  I reverse_n_with_buffer(I f_i, DistanceType(I) n, B f_b)
  {
    //Precondition: mutable_counted_range(f_i, n)
    //Precondition: mutable_counted_range(f_b, n)
    return reverse_copy(f_b, copy_n(f_i, n, f_b).second, f_i);
  }

  template<typename I>
    requires(Mutable(I) && ForwardIterator(I))
  I reverse_n_forward(I f, DistanceType(I) n)
  {
    //Precondition: mutable_counted_range(f, n)
    typedef DistanceType(I) N;
    if (n < N(2)) return f + n;
    N h = half_nonnegative(n);
    N n_mod_2 = n - twice(h);
    I m = reverse_n_forward(f, h) + n_mod_2;
    I l = reverse_n_forward(m, h);
    swap_ranges_n(f, m, h);
    return l;
  } 

  template<typename I, typename B>
    requires(Mutable(I) && ForwardIterator(I) &&
             Mutable(B) && BidirectionalIterator(B) &&
             ValueType(I) == ValueType(B))
  I reverse_n_adaptive(I f_i, DistanceType(I) n_i,
                       B f_b, DistanceType(I) n_b)
  {
    // Precondition: mutable_counted_range(f_i, n_i)
    // Precondition: mutable_counted_range(f_b, n_b)
    typedef DistanceType(I) N;
    if (n_i < N(2))
      return f_i + n_i;
    if (n_i <= n_b)
      return reverse_n_with_buffer(f_i, n_i, f_b);
    N h_i = half_nonnegative(n_i);
    N n_mod_2 = n_i - twice(h_i);
    I m_i = reverse_n_adaptive(f_i, h_i, f_b, n_b) + n_mod_2;
    I l_i = reverse_n_adaptive(m_i, h_i, f_b, n_b);
    swap_ranges_n(f_i, m_i, h_i);
    return l_i;
  }

  // 10.4 Rotate Algorithms
  template<typename I>
    requires(RandomAccessIterator(I))
  struct k_rotate_from_permutation_random_access
  {
    DistanceType(I) k;
    DistanceType(I) n_minus_k;
    I m_prime;
    k_rotate_from_permutation_random_access(I f, I m, I l)
     : k(l - m), n_minus_k(m - f), m_prime(f + (l-m))
    {
      //Precondition: bounded_range(f, l) && m in [f, l)
    }
    I operator()(I x)
    {
      //Precondition: x in [f, l)
      if (x < m_prime) return x + n_minus_k;
      else             return x - k;
    }
  };

  template<typename I>
    requires(IndexedIterator(I))
  struct k_rotate_from_permutation_indexed
  {
    DistanceType(I) k;
    DistanceType(I) n_minus_k;
    I f;
    k_rotate_from_permutation_indexed(I f, I m, I l) :
      k(l - m), n_minus_k(m - f), f(f)
    {
      //Precondition: bounded_range(f, l) && m in [f, l)
    }
    I operator()(I x)
    {
      //Precondition: x in [f, l)
      DistanceType(I) i = x - f;
      if (i < k) return x + n_minus_k;
      else       return f + (i - k);
    }
  };

  template<typename I, typename F>
    requires(Mutable(I) && IndexedIterator(I) &&
             Transformation(F) && I == Domain(F))
  I rotate_cycles(I f, I m, I l, F from)
  {
    //Precondition: mutable_bounded_range(f, l) && m in [f, l]
    //Precondition: from  is a from-permutaion on [f, l)
    typedef DistanceType(I) N;
    N d = gcd<N>(m - f, l - m);
    while(count_down(d)) cycle_from(f + d, from);
    return f + (l - m);
  }

  template<typename I>
    requires(Mutable(I) && RandomAccessIterator(I))
  I rotate_random_access_nontrivial(I f, I m, I l)
  {
    // Precondition: mutable_bounded_range(f, l) && f < m < l
    k_rotate_from_permutation_random_access<I> from(f, m, l);
    return rotate_cycles(f, m, l, from);
  }

  template<typename I>
    requires(Mutable(I) && IndexedIterator(I))
  I rotate_indexed_nontrivial(I f, I m, I l)
  {
    // Precondition: mutable_bounded_range(f, l) && f < m < l
    k_rotate_from_permutation_indexed<I> from(f, m, l);
    return rotate_cycles(f, m, l, from);
  }

  template<typename I>
    requires(Mutable(I) && BidirectionalIterator(I))
  I rotate_bidirectional_nontrivial(I f, I m, I l)
  {
    // Precondition: mutable_bounded_range(f, l) && f < m < l
    reverse_bidirectional(f, m);
    reverse_bidirectional(m, l);
    std::pair<I, I> p = reverse_swap_ranges_bounded(m, l, f, m);
    reverse_bidirectional(p.second, p.first);
    if (m == p.first) return p.second;
    else              return p.first;
  }

  template<typename I>
   requires(Mutable(I) && ForwardIterator(I))
  void rotate_forward_annotated(I f, I m, I l)
  {
    // Precondition: mutable_bounded_range(f, l) && f < m < l
    DistanceType(I) a = m - f;
    DistanceType(I) b = l - m;
    while (true) {
      std::pair<I, I> p = swap_ranges_bounded(f, m, m, l);
      if (p.first == m && p.second == l) { assert(a == b);
        return;
      }
      f = p.first;
      if (f == m) {          assert(b > a);
        m = p.second;        b = b - a;
      } else {               assert(a > b);
                             a = a - b;
      }
    }  
  }

  template<typename I>
    requires(Mutable(I) && ForwardIterator(I))
  void rotate_forward_step(I& f, I& m, I l)
  {
    // Precondition: mutable_bounded_range(f, m, l) && f < m < l
    I c = m;
    do {
      swap_step(f, c);
      if (f == m) m = c;
    } while (c != l);
  }

  template<typename I>
    requires(Mutable(I) && ForwardIterator(I))
  I rotate_forward_nontrivial(I f, I m, I l)
  {
    // Precondition: mutable_bounded_range(f, l) && f < m < l
    rotate_forward_step(f, m, l);
    I m_prime = f;
    while (m != l) rotate_forward_step(f, m, l);
    return m_prime;
  }

  template<typename I>
    requires(Mutable(I) && ForwardIterator(I))
  I rotate_partial_nontrivial(I f, I m, I l)
  {
    return swap_ranges(m, l, f); 
  }

  template<typename I, typename B>
    requires(Mutable(I) && ForwardIterator(I) &&
             Mutable(B) && ForwardIterator(B))
  I rotate_with_buffer_nontrivial(I f, I m, I l, B f_b)
  {
    // Precondition: mutable_bounded_range(f, l) && f < m < l
    // Precondition: mutable_bounded_range(f_b, l-f)
    B l_b = eop::copy(f, m, f_b);
    I m_prime = eop::copy(m, l, f);
    eop::copy(f_b, l_b, m_prime);
    return m_prime;
  }

  template<typename I, typename B>
    requires(Mutable(I) && ForwardIterator(I) &&
             Mutable(B) && ForwardIterator(B))
  I rotate_with_buffer_backward_nontrivial(I f, I m, I l, B f_b)
  {
    // Precondition: mutable_bounded_range(f, l) && f < m < l
    // Precondition: mutable_bounded_range(f_b, l-f)
    B l_b = eop::copy(m, l, f_b);
    copy_backward(f, m, l);
    return eop::copy(f_b, l_b, f);
  }

  // 10.5 Algorithm Selection

  template<typename I>
    requires(Mutable(I) && IndexedIterator(I))
  void reverse_indexed(I f, I l)
  {
    // Precondition: mutable_bounded_range(f, l)
    reverse_n_indexed(f, l - f);
  }

  template<typename I>
  struct temporary_buffer
  {
  };

  template<typename I>
    requires(Mutable(I) && ForwardIterator(I))
  void reverse_n_with_temporary_buffer(I f, DistanceType(I) n)
  {
    // Precondition: mutable_counted_range(f, n)
    temporary_buffer<ValueType(I)> b(n);
    reverse_n_apadtive(f, n, begin(b), size(b));
  }

  template<typename I>
    requires(Mutable(I) && ForwardIterator(I))
  I rotate_nontrivial(I f, I m, I l, forward_iterator_tag)
  {
    return rotate_forward_nontrivial(f, m, l);
  }

  template<typename I>
    requires(Mutable(I) && BidirectionalIterator(I))
  I rotate_nontrivial(I f, I m, I l, bidirectional_iterator_tag)
  {
    return rotate_bidirectional_nontrivial(f, m, l);
  }

  template<typename I>
    requires(Mutable(I) && IndexedIterator(I))
  I rotate_nontrivial(I f, I m, I l, indexed_iterator_tag)
  {
    return rotate_indexed_nontrivial(f, m, l);
  }

  template<typename I>
    requires(Mutable(I) && RandomAccessIterator(I))
  I rotate_nontrivial(I f, I m, I l, random_access_iterator_tag)
  {
    return rotate_random_access_nontrivial(f, m, l);
  }

  template<typename I>
    requires(Mutable(I) && ForwardIterator(I))
  I rotate(I f, I m, I l)
  {
    // Precondition: mutable_bounded_range(f, l) && m E [f, l]
    if (m == f) return l;
    if (m == l) return f;
    return rotate_nontrivial(f, m, l, IteratorConcept<I>());
  }

  // *******************************************************
  // Chapter 11 - Partition and merging
  // *******************************************************

  // Exercise 11.1
  // Implement an algorithm partitioned_at_point that checks
  // whether a given bounded range is partitioned at a
  // specified iterator

  template<typename I, typename P>
    requires(Readable(I) && Iterator(I) && 
             UnaryPredicate(P) && ValueType(I) == Domain(P))
  bool partitioned_at_point(I f, I m, I l, P p)
  {
    // Preconditions: bounded_range(f, l) && m in [f, l)
    return m == find_if(f, l, p) && l == find_if_not(m, l, p);
  }

  // Exercise 11.2
  // Implement an algorithm potential_partition_point returning
  // the iterator where the partition point would occur after
  // partitioning
  template<typename I, typename P>
    requires(Readable(I) && Iterator(I) && 
             UnaryPredicate(P) && ValueType(I) == Domain(P))
  I potential_partition_point(I f, I l, P p)
  {
    // Precondition: bounded_range(f,l)
    return f + count_if_not(f, l, p);
  }

  template<typename I, typename P>
    requires(Mutable(I) &&  ForwardIterator(I) &&
             UnaryPredicate(P) && ValueType(I) == Domain(P))
  I partition_semistable(I f, I l, P p)
  {
    // Precondition: mutable_bounded_ragne(f, l)
    I i = find_if(f, l, p);
    if (i == l) return i;
    I j = successor(i);
    while(true) {
      j = find_if_not(j, l, p);
      assert(none(f, i, p) && all(i, j, p));
      if (j == l) return i;
      assert(p(source(i)) && ! p(source(j)));
      assert(none(f, i, p) && all(i, j, p));
      swap_step(i, j);
    }
  }

  // Exercise 11.3
  // Rewrite partition_semistable, expanding the call of find_if_not
  // inline and eliminating the extra test against l. 

  template<typename I, typename P>
    requires(Mutable(I) &&  ForwardIterator(I) &&
             UnaryPredicate(P) && ValueType(I) == Domain(P))
  I partition_semistable_expanded(I f, I l, P p)
  {
    // Precondition: mutable_bounded_ragne(f, l)
    I i = find_if(f, l, p);
    if (i == l) return i;
    I j = successor(i);
    while(true) {
      while (true) {
        if (j == l) return i;
        if (!p(source(j))) break;
        j = successor(j);
      }
      swap_step(i, j);
    }
  }

  // Exercise 11.4
  // Give the postcondition of the algorithm that results from replacing
  // swap_step(i, j) with copy_step(i, j) in partition_semistable, suggest
  // an appropriate name, and compare its use with the use of partition_semistable

  template<typename I, typename P>
    requires(Mutable(I) &&  ForwardIterator(I) &&
             UnaryPredicate(P) && ValueType(I) == Domain(P))
  I semipartition(I f, I l, P p)
  {
    // Precondition: mutable_bounded_ragne(f, l)
    I i = find_if(f, l, p);
    if (i == l) return i;
    I j = successor(i);
    while(true) {
      j = find_if_not(j, l, p);
      if (j == l) return i;
      copy_step(i, j);
    }
    // Postcondition: All elements statisfying the predicate is within [i ,l)
  }

  // Exercise 11.5
  // Implement a partition rearrangement for nonempty ranges that performs
  // n - 1 predicate applications
  template<typename I, typename P>
    requires(Mutable(I) && ForwardIterator(I) &&
             UnaryPredicate(P) && ValueType(I) == Domain(P))
  void partition(I f, I l, P p)
  {
    if (f == l) return;
    I n = successor(f);
    I m = partition_semistable_expanded(n, l, p);
    if (m == l || m == n) return;
    rotate_forward_nontrivial(f, n, m);
  }

  template<typename I, typename P>
    requires(Mutable(I) && BidirectionalIterator(I) &&
             UnaryPredicate(P) && ValueType(I) == Domain(P))
  I partition_bidirectional(I f, I l, P p)
  {
    // Precondition: mutable_bounded_range(f, l)
    while (true) {
      f = find_if(f, l, p);
      l = find_backward_if_not(f, l, p);
      if (f == l) return f;
      reverse_swap_step(l, f);
    }
  }

  // Exercise 11.6 
  // Implement a partition rearrangement for forward iterators that calls
  // exchange_values the same number of times as partition_bidirectional
  // by first computing the potential partition point

  template<typename I, typename P>
  requires(Mutable(I) && ForwardIterator(I) &&
           UnaryPredicate(P) && ValueType(I) == Domain(P))
  I partition_forward(I f, I l, P p)
  {
    //Precondition: mutable_bounded_range(f, l)
    I m = potential_partition_point(f, l, p);
    I i;
    while (true) {
      f = find_if(f, m, p);
      i = find_if_not(m, l, p);
      if (f == m || i == l) return m;
      swap_step(f, i);
    }
  }

  // Exercise 11.7
  // Implement partition_single_cycle
  template<typename I, typename P>
    requires(Mutable(I) && BidirectionalIterator(I) &&
             UnaryPredicate(P) && ValueType(I) == Domain(P))
  I partition_single_cycle(I f, I l, P p)
  {
    //Precondition: mutable_bounded_range(f, l)
    I m = potential_partition_point(f, l, p);
    // if either all element statisfy the predicate or none
    if (f == m || l == m) return m;

    f = find_if(f, m, p);
    // If there isn't any misplaced element
    if (f == m) return m;

    ValueType(I) hole = source(f);
    I i;
    while(true) {
      i = find_if_not(m, l, p);
      sink(f) = source(i);
      f = find_if(f, m, p);
      if (f == m) break;
      sink(i) = source(f);
    }
    sink(i) = hole;
    return m;
  }

  // Exercise 11.8
  // Implement a partition rearrangement for bidirectional iterators
  // that finds appropriate sentinel elements and then uses find_if_ungarded
  // and an ungarded version of find_backward_if_not.
  template<typename I, typename P>
    requires(Mutable(I) && BidirectionalIterator(I) &&
             UnaryPredicate(P) && ValueType(I) == Domain(P))
  I partition_backward_unguarded(I f, I l, P p)
  {
    // TODO: read Quicksort
    return f;
  }

  template<typename I, typename B, typename P>
    requires(Mutable(I) && ForwardIterator(I) &&
             Mutable(B) && ForwardIterator(B) &&
             ValueType(I) == ValueType(B) &&
             UnaryPredicate(P) && ValueType(I) == Domain(P))
  I partition_stable_with_buffer(I f, I l, B f_b, P p)
  {
    // Precondition: mutable_bounded_range(f, l)
    // Precondition: mutable_counted_range(f_b, l-f)
    std::pair<I, B> x = partition_copy(f, l, f, f_b, p);
    eop::copy(f_b, x.second, x.first);
    return x.first;
  }

  template<typename I, typename B, typename P>
    requires(Mutable(I) && ForwardIterator(I) &&
             Mutable(B) && ForwardIterator(B) &&
             ValueType(I) == ValueType(B) &&
             UnaryPredicate(P) && ValueType(I) == Domain(P))
  std::pair<I, I> partition_stable_with_buffer_n(I f, DistanceType(I) n, B f_b, P p)
  {
    // Precondition: mutable_counted_range(f, n)
    // Precondition: mutable_counted_range(f_b, n)
    std::pair<I, B> x = partition_copy_n(f, n, f, f_b, p);
    I l = eop::copy(f_b, x.second, x.first);
    return std::pair<I, I>(x.first, l);
  }
  
  template<typename I, typename P>
    requires(Mutable(I) && ForwardIterator(I) &&
             UnaryPredicate(P) && ValueType(I) == Domain(P))
  std::pair<I, I> partition_stable_singleton(I f, P p)
  {
    // Precondition: readable_bounded_range(f, successor(f))
    I l = successor(f);
    if (!p(source(f))) f = l;
    return std::pair<I, I>(f, l);
  }

  template<typename I>
    requires(Mutable(I) && ForwardIterator(I))
  std::pair<I, I> combine_ranges(const std::pair<I, I>& x,
                                 const std::pair<I, I>& y)
  {
    // Precondition: mutable_bounded_range(x.first, x.second)
    // Precondtiion: x.second is in [x.first, y.first]
    return std::pair<I, I>(rotate(x.first, x.second, y.first), y.second);
  }

  template<typename I, typename P>
    requires(Mutable(I) && ForwardIterator(I) &&
             UnrayPredicate(P) && ValueType(I) == Domain(P))
  // returns the first and last iterator for the elements statisfying the predicate
  std::pair<I, I> partition_stable_n_nonempty(I f, DistanceType(I) n, P p)
  {
    // Precondition: mutable_bounded_range(f, n)
    if (one(n)) return partition_stable_singleton(f, p);
    DistanceType(I) h = half_nonnegative(n);
    std::pair<I, I> x = partition_stable_n_nonempty(f, h, p);
    std::pair<I, I> y = partition_stable_n_nonempty(x.second, n - h, p);
    return combine_ranges(x, y);
  }

  template<typename I, typename P>
    requires(Mutable(I) && ForwardIterator(I) &&
             UnrayPredicate(P) && ValueType(I) == Domain(P))
  std::pair<I, I> partition_stable_n(I f, DistanceType(I) n, P p)
  {
    // Precondition: mutbale_counted_range
    if (zero(n)) return std::pair<I, I>(f, f);
    return partition_stable_n_nonempty(f, n, p);
  }
  
  // Exercise 11.10
  // Use techniques from the previous chapter to produce
  // a memory-adaptive version of partition stable_n
  template<typename I, typename B, typename P>
    requires(Mutable(I) && ForwardIterator(I) &&
             Mutable(B) && ForwardIterator(B) &&
             ValueType(I) == ValueType(B) &&
             UnaryPredicate(P) && ValueType(I) == Domain(P))
  std::pair<I, I>  partition_stable_n_adaptive_nonempty(I f_i, DistanceType(I) n_i,
                                                        B f_b, DistanceType(I) n_b, P p)
  {
    if (one(n_i)) return partition_stable_singleton(f_i, p);
    if (n_i <= n_b) return partition_stable_with_buffer_n(f_i, n_i, f_b, p);
    DistanceType(I) h = half_nonnegative(n_i);
    std::pair<I, I> x = partition_stable_n_nonempty(f_i, h, p);
    std::pair<I, I> y = partition_stable_n_nonempty(x.second, n_i - h, p);
    return combine_ranges(x, y);
  }

  template<typename I, typename B, typename P>
    requires(Mutable(I) && ForwardIterator(I) &&
             Mutable(B) && ForwardIterator(B) &&
             ValueType(I) == ValueType(B) &&
             UnaryPredicate(P) && ValueType(I) == Domain(P))
  std::pair<I, I>  partition_stable_n_adaptive(I f_i, DistanceType(I) n_i,
                                               B f_b, DistanceType(I) n_b, P p)
  {
    if (zero(n_i)) return std::pair<I, I>(f_i, f_i);
    return partition_stable_n_adaptive_nonempty(f_i, n_i, f_b, n_b, p);
  }

  // 11.2 Balanced Reduction

  template<typename I, typename P>
    requires(ForwardIterator(I) &&
             UnaryPredicate(P) && ValueType(I) == Domain(P))
  struct partition_trivial
  {
    P p;
    partition_trivial(P p) : p(p) {} 
    std::pair<I, I> operator()(I i)
    {
      return partition_stable_singleton(i, p);
    }
  };

  template<typename I, typename Op>
    requires(Mutable(I) && ForwardIterator(I) &&
             BinaryOperation(Op) && ValueType(I) == Domain(Op))
  Domain(Op) add_to_counter(I f, I l, Op op, Domain(Op) x, const Domain(Op)& z)
  {
    if (x == z) return z;
    while (f != l) {
      if (source(f) == z) {
        sink(f) = x;
        return z;
      }
      x = op(source(f), x);
      sink(f) = z;
      f = successor(f);
    }
    return x;
  }

  // Storage for the counter is provided by the following type, which
  // handles overflows from add_to_counter by extending the counter
  template<typename Op>
    requires(BinaryOperation(Op))
  struct counter_machine
  {
    typedef Domain(Op) T;
    Op op;
    T z;
    // 64 is chosen for 64-bit architectures
    T f[64];
    DistanceType(pointer(T)) n;

    counter_machine(Op op, const Domain(Op)& z) :
      op(op), z(z), n(0) {}
    void operator()(const T& x)
    {
      // Precondition: must not be called more than 2^64 -1 times
      T tmp = add_to_counter(f, f + n, op, x, z);
      if (tmp != z) {
        sink(f+n) = tmp;
        n = successor(n);
      }
    }
  };

  template<typename Op>
    requires(BinaryOperation(Op))
  struct transpose_operation
  {
    Op op;
    transpose_operation(Op op) : op(op) {}
    typedef Domain(Op) T;
    typedef T first_argument_type;
    typedef T second_argument_type;
    T operator() (const T& x, const T& y)
    {
      return op(y, x);
    }
  };

  template<typename I, typename Op, typename F>
    requires(Iterator(I) && BinaryOperation(Op) &&
             UnaryFunction(F) && I == Domain(F) &&
             Codomain(F) == Domain(Op))
  Domain(Op) reduce_balanced(I f, I l, Op op, F fun, const Domain(Op)& z)
  {
    // Precondition: bounded_range(f, l) && l-f < 2^64
    // Precondition: partially_assosiative(op)
    // Precondition: (for all x is in [f, l) fun(x) is defined
    counter_machine<Op> c(op, z);
    while (f != l) {
      c(fun(f));
      f = successor(f);
    }
    transpose_operation<Op> t_op(op);
    return reduce_nonzeros(c.f, c.f + c.n, t_op, eop::deref<Domain(Op)>, z);
  }

  template<typename I, typename P>
    requires(ForwardIterator(I) && UnaryPredicate(P) &&
             ValueType(I) == Domain(P))
  I partition_stable_iterative(I f, I l, P p)
  {
    return reduce_balanced(
      f, l,
      combine_ranges<I>,
      partition_trivial<I, P>(p),
      std::pair<I, I>(f, f)
    ).first;
  }


} // namespace eop
