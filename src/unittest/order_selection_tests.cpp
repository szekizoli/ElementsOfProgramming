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

	TEST(iteratorstest, testsource)
	{
		vector<int> v{ 1 };
		auto r = eop::source(begin(v));
		EXPECT_EQ(1, r);
	}

	TEST(iteratorstest, testincrement)
	{
		vector<int> v{ 1, 2 };
		auto r = eop::increment(begin(v));
		EXPECT_EQ(2, eop::source(r));
	}

	TEST(iteratorstest, test_operator_plus)
	{
		vector<int> v{ 1, 2, 3, 4 };
		auto r = begin(v) + 2;
		EXPECT_EQ(3, eop::source(r));
	}

	struct sum {
		int _sum = 0;
		void operator()(int value) {
			_sum += value;
		}
	};

	TEST(iteratorstest, test_for_each)
	{
		vector<int> v{ 1, 2, 3, 4 };
		sum r = eop::for_each(v.begin(), v.end(), sum());
		EXPECT_EQ(10, r._sum);
	}

	TEST(iteratorstest, test_find)
	{
		vector<int> v{ 1, 2, 3, 4 };
		auto r = eop::find(v.begin(), v.end(), 2);
		EXPECT_EQ(2, eop::source(r));
		r = eop::find(v.begin(), v.end(), -1);
		EXPECT_EQ(end(v), r) << "error in not found";
	}

	TEST(iteratorstest, test_find_empty)
	{
		vector<int> v{ };
		auto r = eop::find(v.begin(), v.end(), 2);
		EXPECT_EQ(end(v), r) << "error in not found";
	}

	struct equals_To {
		const int value;
		equals_To(int v) : value(v) {};
		bool operator()(int x) {
			return value == x;
		}
	};

	TEST(iteratorstest, test_find_if)
	{
		vector<int> v{ 1, 4, 3, 2 };
		auto r = eop::find_if(v.begin(), v.end(), equals_To(3));
		EXPECT_EQ(3, eop::source(r));
		r = eop::find_if(v.begin(), v.end(), equals_To(-1));
		EXPECT_EQ(end(v), r) << "error in not found";
	}

	TEST(iteratorstest, test_find_if_empty)
	{
		vector<int> v{};
		auto r = eop::find_if(v.begin(), v.end(), equals_To(2));
		EXPECT_EQ(end(v), r) << "error in not found";
	}

	struct less_Than {
		const int value;
		less_Than(int v) : value(v) {};
		bool operator()(int x) {
			return x < value;
		}
	};

	TEST(iteratorstest, test_find_if_not)
	{
		vector<int> v{ 1, 4, 3, 2 };
		auto r = eop::find_if_not(v.begin(), v.end(), less_Than(3));
		EXPECT_EQ(4, eop::source(r));
		r = eop::find_if(v.begin(), v.end(), less_Than(-1));
		EXPECT_EQ(end(v), r) << "error in not found";
	}

	TEST(iteratorstest, test_find_if_not_empty)
	{
		vector<int> v{};
		auto r = eop::find_if(v.begin(), v.end(), less_Than(2));
		EXPECT_EQ(end(v), r) << "error in not found";
	}

	TEST(iteratorstest, test_all)
	{
		vector<int> v{ 1, 4, 3, 2 };
		bool all = eop::all(v.begin(), v.end(), less_Than(10));
		EXPECT_EQ(true, all);
		all = eop::all(v.begin(), v.end(), less_Than(4));
		EXPECT_EQ(false, all);
	}

	TEST(iteratorstest, test_all_empty)
	{
		vector<int> v{ };
		bool all = eop::all(v.begin(), v.end(), less_Than(10));
		EXPECT_EQ(true, all);
	}

	TEST(iteratorstest, test_none)
	{
		vector<int> v{ 1, 4, 3, 2 };
		bool none = eop::none(v.begin(), v.end(), less_Than(0));
		EXPECT_EQ(true, none);
		none = eop::none(v.begin(), v.end(), equals_To(0));
		EXPECT_EQ(true, none);
		none = eop::none(v.begin(), v.end(), equals_To(3));
		EXPECT_EQ(false, none);
	}

	TEST(iteratorstest, test_none_empty)
	{
		vector<int> v{};
		bool none = eop::none(v.begin(), v.end(), less_Than(10));
		EXPECT_EQ(true, none);
	}

	TEST(iteratorstest, test_not_all)
	{
		vector<int> v{ 1, 4, 3, 2 };
		bool not_all = eop::not_all(v.begin(), v.end(), less_Than(10));
		EXPECT_EQ(false, not_all);
		not_all = eop::not_all(v.begin(), v.end(), less_Than(4));
		EXPECT_EQ(true, not_all);
	}

	TEST(iteratorstest, test_not_all_empty)
	{
		vector<int> v{};
		bool not_all = eop::not_all(v.begin(), v.end(), less_Than(10));
		EXPECT_EQ(false, not_all);
	}

	TEST(iteratorstest, test_some)
	{
		vector<int> v{ 1, 4, 3, 2 };
		bool some = eop::some(v.begin(), v.end(), less_Than(0));
		EXPECT_EQ(false, some);
		some = eop::some(v.begin(), v.end(), equals_To(0));
		EXPECT_EQ(false, some);
		some = eop::some(v.begin(), v.end(), equals_To(3));
		EXPECT_EQ(true, some);
	}

	TEST(iteratorstest, test_some_empty)
	{
		vector<int> v{};
		bool some = eop::some(v.begin(), v.end(), less_Than(10));
		EXPECT_EQ(false, some);
	}

	TEST(iteratorstest, test_count_if)
	{
		vector<int> v{ 1, 2, 3, 4, 5};
		int count_even = eop::count_if(v.begin(), v.end(), eop::is_Even<int>(), 0);
		EXPECT_EQ(2, count_even);
		int count_odd = eop::count_if(v.begin(), v.end(), eop::is_Odd<int>(), 0);
		EXPECT_EQ(3, count_odd);
		int count_less_than_3 = eop::count_if(v.begin(), v.end(), less_Than(3), 0);
		EXPECT_EQ(2, count_less_than_3);
		int count_less_than_1 = eop::count_if(v.begin(), v.end(), less_Than(1), 0);
		EXPECT_EQ(0, count_less_than_1);
		int count_less_than_7 = eop::count_if(v.begin(), v.end(), less_Than(7), 0);
		EXPECT_EQ(5, count_less_than_7);
	}
}
