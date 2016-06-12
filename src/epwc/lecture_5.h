#pragma once

#include<utility>


namespace epwc {
	template<typename I, typename Comparator>
	// requires I is a ForwardIterator
	// and Compare is StrictWeakOrdering on ValueType(I)
	std::pair<I, I> minmax_element(I first, I last, Comparator cmp) {
		std::pair<I, I> result = std::make_pair(first, first);
		
		if (first == last) return result; // empty
		++first;
		if (first == last) return result; // one element
		if (cmp(*first, *result.first)) {
			result.first = first;
		}
		else {
			result.second = first;
		}

		++first;

		while (first != last) {
			// invariants:
			// result.first points to the current minimum
			// result.second points to the current maximum
			I potential_min = first;
			++first;
			I potential_max = first;
			if (first == last) potential_max = potential_min;
			else {
				++first;
				if (cmp(*potential_max, *potential_min)) {
					std::swap(potential_min, potential_max);
				}
			}

			if (cmp(*potential_min, *result.first)) {
				result.first = potential_min;
			}

			if (!cmp(*potential_max, *result.second)) {
				result.second = potential_max;
			}
		}
		return result;
	}
}