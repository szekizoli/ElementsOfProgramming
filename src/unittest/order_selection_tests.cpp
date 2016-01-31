#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "common.h"
#include "eop.h"

namespace eoptest
{
	using std::vector;

	int less(int a, int b) {
		return a < b;
	}

	template<typename T>
		requires(Numeric(T))
	std::string to_string(const vector<T> & v) {
		return std::accumulate(v.begin() + 1, v.end(), std::to_string(v[0]),
			[](const std::string& a, int b) {
			return a + ", " + std::to_string(b);
		});
	}

	void expect_select_0_3(const vector<int>& numbers, size_t expected_index) {
		const int& r = eop::select_0_3(numbers[0], numbers[1],
			numbers[2], less);
		EXPECT_EQ(&numbers[expected_index], &r) << to_string(numbers);
	}

	void expect_select_1_3(const vector<int>& numbers, size_t expected_index) {
		const int& r = eop::select_1_3(numbers[0], numbers[1],
			numbers[2], less);
		EXPECT_EQ(&numbers[expected_index], &r) << to_string(numbers);
	}

	void expect_select_2_3(const vector<int>& numbers, size_t expected_index) {
		const int& r = eop::select_2_3(numbers[0], numbers[1],
			numbers[2], less);
		EXPECT_EQ(&numbers[expected_index], &r) << to_string(numbers);
	}

	void expect_select_1_4(const vector<int>& numbers, size_t expected_index) {
		ASSERT_EQ(4, numbers.size()) << "size of numbers should be 4!!!";
		const int& r = eop::select_1_4(numbers[0], numbers[1],
			numbers[2], numbers[3], less);

		EXPECT_EQ(&numbers[expected_index], &r) << to_string(numbers);
	}

	void expect_median_5(const vector<int>& numbers, size_t expected_index) {
		ASSERT_EQ(5, numbers.size()) << "size of numbers should be 5!!!";
		const int& r = eop::median_5(numbers[0], numbers[1],
			numbers[2], numbers[3], numbers[4], less);

		EXPECT_EQ(&numbers[expected_index], &r) << to_string(numbers);
	}

	TEST(orderselectiontest, testselect02)
	{
		int a = 2;
		int b = 3;
		int r_0 = eop::select_0_2(a, b, less);
		EXPECT_EQ(2, r_0);
		
		a = 3;
		const int& r_1 = eop::select_0_2(a, b, less);
		EXPECT_EQ(&a, &r_1);
		EXPECT_NE(&b, &r_1);

		a = -10;
		b = 3;
		int r_2 = eop::select_0_2(a, b, less);
		EXPECT_EQ(-10, r_2);
	}

	TEST(orderselectiontest, testselect12)
	{
		int a = 2;
		int b = 3;
		int r_0 = eop::select_1_2(a, b, less);
		EXPECT_EQ(3, r_0);

		a = 3;
		const int& r_1 = eop::select_1_2(a, b, less);
		EXPECT_NE(&a, &r_1);
		EXPECT_EQ(&b, &r_1);
	}

	TEST(orderselectiontest, testselect03)
	{
		expect_select_0_3({ 0, 1, 2 }, 0);
		expect_select_0_3({ 2, 1, 0 }, 2);
		expect_select_0_3({ 0, 1, 1 }, 0);
		expect_select_0_3({ 0, 0, 0 }, 0);
	}

	TEST(orderselectiontest, testselect23)
	{
		expect_select_2_3({ 0, 1, 2 }, 2);
		expect_select_2_3({ 2, 1, 0 }, 0);
		expect_select_2_3({ 1, 0, 0 }, 0);
		expect_select_2_3({ 0, 0, 1 }, 2);
		expect_select_2_3({ 1, 1, 0 }, 1);
		expect_select_2_3({ 1, 0, 1 }, 2);
		expect_select_2_3({ 0, 1, 1 }, 2);
		expect_select_2_3({ 0, 0, 0 }, 2);
	}

	TEST(orderselectiontest, testselect13)
	{
		expect_select_1_3({ 0, 1, 2 }, 1);
		expect_select_1_3({ 2, 1, 0 }, 1);
		expect_select_1_3({ 1, 0, 0 }, 2);
		expect_select_1_3({ 0, 0, 1 }, 1);
		expect_select_1_3({ 1, 1, 0 }, 0);
		expect_select_1_3({ 1, 0, 1 }, 0);
		expect_select_1_3({ 0, 1, 1 }, 1);
		expect_select_1_3({ 0, 0, 0 }, 1);
	}
	
	TEST(orderselectiontest, testselect14)
	{
		expect_select_1_4({ 0, 1, 2, 3 }, 1);
		expect_select_1_4({ 3, 2, 1, 0 }, 2);
		expect_select_1_4({ 0, 0, 1, 1 }, 1);
		expect_select_1_4({ 0, 1, 1, 0 }, 3);
		expect_select_1_4({ 0, 1, 0, 0 }, 2);
	}

	TEST(orderselectiontest, testmedian5)
	{
		expect_median_5({ 0, 1, 2, 3, 4 }, 2);
		expect_median_5({ 4, 3, 2, 1, 0 }, 2);
		expect_median_5({ 0, 0, 0, 0, 0 }, 2);
		expect_median_5({ 2, 1, 2, 1, 0 }, 3);

		expect_median_5({ 0, 0, 0, 0, 1 }, 2);
		expect_median_5({ 0, 0, 0, 1, 0 }, 2);
		expect_median_5({ 0, 0, 1, 0, 0 }, 3);
		expect_median_5({ 0, 1, 0, 0, 0 }, 3);
		expect_median_5({ 1, 0, 0, 0, 0 }, 3);

		expect_median_5({ 1, 0, 0, 0, 1 }, 3);
		expect_median_5({ 0, 1, 0, 0, 1 }, 3);
		expect_median_5({ 0, 0, 1, 0, 1 }, 3);
		expect_median_5({ 0, 0, 0, 1, 1 }, 2);

		expect_median_5({ 1, 0, 0, 1, 0 }, 4);
		expect_median_5({ 0, 1, 0, 1, 0 }, 4);
		expect_median_5({ 0, 0, 1, 1, 0 }, 4);

		expect_median_5({ 1, 0, 1, 0, 0 }, 4);
		expect_median_5({ 0, 1, 1, 0, 0 }, 4);

		expect_median_5({ 1, 1, 0, 0, 0 }, 4);

		expect_median_5({ 0, 1, 1, 1, 0 }, 1);
		expect_median_5({ 1, 0, 1, 1, 0 }, 0);
		expect_median_5({ 1, 1, 0, 1, 0 }, 0);
		expect_median_5({ 1, 1, 1, 0, 0 }, 0);

		expect_median_5({ 0, 1, 1, 0, 1 }, 1);
		expect_median_5({ 1, 0, 1, 0, 1 }, 0);
		expect_median_5({ 1, 1, 0, 0, 1 }, 0);

		expect_median_5({ 0, 1, 0, 1, 1 }, 1);
		expect_median_5({ 1, 0, 0, 1, 1 }, 0);

		expect_median_5({ 0, 0, 1, 1, 1 }, 2);
	}
}
