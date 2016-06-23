#pragma once

#include<utility>

template<typename I, typename Comparator>
// requires I is a ForwardIterator
// and Compare is StrictWeakOrdering on ValueType(I)
std::pair<I, I> minmax_element(I first, I last, Comparator cmp) {
	if (first == last) return std::make_pair(last, last);

	I min = first;
	++first;
	if (first == last) return std::make_pair(min, min);
	I max = first;
	++first;

	while (first != last) {
		// invariants:
		// min points to the current minimum
		// max points to the current maximum
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

		if (cmp(*potential_min, *min)) {
			min = potential_min;
		}

		if (!cmp(*potential_max, *max)) {
			max = potential_max;
		}
	}
	return std::make_pair(min, max);
}
