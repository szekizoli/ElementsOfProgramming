#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "lecture_5.h"

namespace epwctest {
	using namespace epwc;

	TEST(epwc_lectur_5_test, empty) {
		std::vector<int> v = { };
		auto result = epwc::minmax_element(std::begin(v), std::end(v), std::less<int>());
		EXPECT_EQ(std::end(v), result.first) << "Iterator should be end";
		EXPECT_EQ(std::end(v), result.second) << "Iterator should be end";
	}

	TEST(epwc_lectur_5_test, one_element) {
		std::vector<int> v = { 2 };
		auto result = epwc::minmax_element(std::begin(v), std::end(v), std::less<int>());
		EXPECT_EQ(2, *result.first) << "Min is 2";
		EXPECT_EQ(2, *result.second) << "Max is 2";
	}

	TEST(epwc_lectur_5_test, two_elements) {
		std::vector<int> v = { 6, 2 };
		auto result = epwc::minmax_element(std::begin(v), std::end(v), std::less<int>());
		EXPECT_EQ(2, *result.first) << "Min is 2";
		EXPECT_EQ(6, *result.second) << "Max is 6";
	}

	TEST(epwc_lectur_5_test, even_elements) {
		std::vector<int> v = { 5, 32, 1, 9, 2, 10, 32, 2, 1, 14 };
		auto result = epwc::minmax_element(std::begin(v), std::end(v), std::less<int>());
		EXPECT_EQ(1, *result.first) << "Min is 1";
		EXPECT_EQ(2, std::distance(std::begin(v), result.first));
		EXPECT_EQ(32, *result.second) << "Max is 32";
		EXPECT_EQ(6, std::distance(std::begin(v), result.second));
	}

	TEST(epwc_lectur_5_test, odd_elements) {
		std::vector<int> v = { 5, 3, 32, 1, 9, 2, 10, 32, 2, 1, 14 };
		auto result = epwc::minmax_element(std::begin(v), std::end(v), std::less<int>());
		EXPECT_EQ(1, *result.first) << "Min is 1";
		EXPECT_EQ(3, std::distance(std::begin(v), result.first));
		EXPECT_EQ(32, *result.second) << "Max is 32";
		EXPECT_EQ(7, std::distance(std::begin(v), result.second));
	}
}