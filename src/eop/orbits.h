#pragma once

#include "common.h"
#include "type_functions.h"

namespace eop {

	#define Domain(T) typename value_type<T>::type;

	template<typename F, typename P>
		//requires(Transformation(F) && UnaryPredicate(P) && Domain(F) == Domain(P))
	Domain(F) collision_point(const Domain(F)& x, F f, P p) {
		if (!p(x)) return x;
		Domain(F) slow = x;
		Domain(F) fast = f(x);

		while (fast != slow) {
			slow = f(slow);
			if (!p(fast)) return fast;
			fast = f(fast);
			if (!p(fast)) return fast;
			fast = f(fast);
		}

		return fast;
	}

} // namespace eop
