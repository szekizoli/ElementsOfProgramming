#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "binary_counter.h"
#include "lecture_5.h"

namespace epwctest {
	using namespace epwc;
	using std::vector;

	TEST(epwc_lecture_5_test, empty) {
		std::vector<int> v = { };
		auto result = epwc::minmax_element(std::begin(v), std::end(v), std::less<int>());
		EXPECT_EQ(std::end(v), result.first) << "Iterator should be end";
		EXPECT_EQ(std::end(v), result.second) << "Iterator should be end";
	}

	TEST(epwc_lecture_5_test, one_element) {
		std::vector<int> v = { 2 };
		auto result = epwc::minmax_element(std::begin(v), std::end(v), std::less<int>());
		EXPECT_EQ(2, *result.first) << "Min is 2";
		EXPECT_EQ(2, *result.second) << "Max is 2";
	}

	TEST(epwc_lecture_5_test, two_elements) {
		std::vector<int> v = { 6, 2 };
		auto result = epwc::minmax_element(std::begin(v), std::end(v), std::less<int>());
		EXPECT_EQ(2, *result.first) << "Min is 2";
		EXPECT_EQ(6, *result.second) << "Max is 6";
	}

	TEST(epwc_lecture_5_test, even_elements) {
		std::vector<int> v = { 5, 32, 1, 9, 2, 10, 32, 2, 1, 14 };
		auto result = epwc::minmax_element(std::begin(v), std::end(v), std::less<int>());
		EXPECT_EQ(1, *result.first) << "Min is 1";
		EXPECT_EQ(2, std::distance(std::begin(v), result.first));
		EXPECT_EQ(32, *result.second) << "Max is 32";
		EXPECT_EQ(6, std::distance(std::begin(v), result.second));
	}

	TEST(epwc_lecture_5_test, odd_elements) {
		std::vector<int> v = { 5, 3, 32, 1, 9, 2, 10, 32, 2, 1, 14 };
		auto result = epwc::minmax_element(std::begin(v), std::end(v), std::less<int>());
		EXPECT_EQ(1, *result.first) << "Min is 1";
		EXPECT_EQ(3, std::distance(std::begin(v), result.first));
		EXPECT_EQ(32, *result.second) << "Max is 32";
		EXPECT_EQ(7, std::distance(std::begin(v), result.second));
	}


	template<typename T>
	// requires Regular(T)
	struct adder
	{
		T operator()(T const& l, T const& r)
		{
			return l + r;
		}
	};

	TEST(binary_counter, add_elements) 
	{
		std::vector<int> elements {8, 3, 1, 7, 9, 6, 2, 5 };
		std::vector<int> counter(4);
		adder<int> add;
		for (int i : elements) {
			add_to_counter(begin(counter), end(counter), add, 0, i);
		}
		int result = reduce_counter(begin(counter), end(counter), add, 0);
		EXPECT_EQ(41, result);
	}

	TEST(binary_counter, add_elements_w_counter) 
	{
		std::vector<int> elements {8, 3, 1, 7, 9, 6, 2, 5 };
		binary_counter<int, adder<int>> counter(adder<int>(), 0);
		for (int i : elements) counter.add(i);
		int result = counter.reduce();
		EXPECT_EQ(41, result);
	}

	template<typename T>
	// requires Regular(T)
	struct min
	{
		T operator()(T const& l, T const& r)
		{
			return r < l ? r : l; 
		}
	};

	TEST(binary_counter, min_elements) 
	{
		constexpr int ZERO = 0;
		std::vector<int> elements {8, 3, 1, 7, 9, 6, 2, 5 };
		std::vector<int> counter(4,	 ZERO);
		min<int> m;
		for (int i : elements) {
			add_to_counter(begin(counter), end(counter), m, ZERO, i);
		}
		int result = reduce_counter(begin(counter), end(counter), m, ZERO);
		EXPECT_EQ(1, result);
	}

	TEST(binary_counter, min_elements_w_counter) 
	{
		std::vector<int> elements {8, 3, 1, 7, 9, 6, 2, 5 };
		auto result = min_element_binary(begin(elements), end(elements), std::less<int>());
		ASSERT_NE(result, end(elements));
		EXPECT_EQ(1, *result);
	}
}