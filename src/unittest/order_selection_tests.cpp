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
		typedef int first_argument_type;
		typedef int input_type;
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
		count_even = eop::count_if(v.begin(), v.end(), eop::is_Even<int>());
		EXPECT_EQ(2, count_even);
		int count_odd = eop::count_if(v.begin(), v.end(), eop::is_Odd<int>(), 0);
		EXPECT_EQ(3, count_odd);
		int count_less_than_3 = eop::count_if(v.begin(), v.end(), less_Than(3), 0);
		EXPECT_EQ(2, count_less_than_3);
		count_less_than_3 = eop::count_if(v.begin(), v.end(), less_Than(3));
		EXPECT_EQ(2, count_less_than_3);
		int count_less_than_1 = eop::count_if(v.begin(), v.end(), less_Than(1), 0);
		EXPECT_EQ(0, count_less_than_1);
		int count_less_than_7 = eop::count_if(v.begin(), v.end(), less_Than(7), 0);
		EXPECT_EQ(5, count_less_than_7);
	}

	TEST(iteratorstest, test_reduce_nonempty)
	{
		vector<int> v{ 1, 2, 3, 4, 5 };
		const int sum = eop::reduce_nonempty(begin(v), end(v), std::plus<int>(), eop::source_function<vector<int>::iterator>());
		EXPECT_EQ(15, sum);
		const int product = eop::reduce_nonempty(begin(v), end(v), std::multiplies<int>(), eop::source_function<vector<int>::iterator>());
		EXPECT_EQ(120, product);
		vector<int> v2{ 0, 0, 0, 1, 2, 0, 3, 0, 4, 0, 0, 5 };
		const int product2 = eop::reduce_nonempty(begin(v2), end(v2), std::multiplies<int>(), eop::source_function<vector<int>::iterator>());
		EXPECT_EQ(0, product2);
	}

	TEST(iteratorstest, test_reduce_nonzeros)
	{
		vector<int> v{ 0, 0, 0, 1, 2, 0, 3, 0, 4, 0, 0, 5 };
		int product = eop::reduce_nonzeros(begin(v), end(v), eop::multiplies<int>(), eop::source_function<vector<int>::iterator>(), 0);
		EXPECT_EQ(120, product);
	}

	TEST(iteratorstest, test_for_each_n)
	{
		vector<int> v{ 1, 2, 3, 4 };
		auto r0 = eop::for_each_n(v.begin(), 2, sum());
		EXPECT_EQ(3, r0.first._sum);
		auto r1 = eop::for_each_n(r0.second, 2, sum());
		EXPECT_EQ(7, r1.first._sum);
	}

	TEST(iteratorstest, test_for_each_n_empty)
	{
		vector<int> v{};
		auto r0 = eop::for_each_n(v.begin(), 0, sum());
		EXPECT_EQ(0, r0.first._sum);
	}

	TEST(iteratorstest, test_find_n)
	{
		vector<int> v = { 2, 3, 1, 3, 5, 2, 1, 2, 8 };
		auto r0 = eop::find_n(begin(v), v.size(), 1);
		EXPECT_EQ(1, *r0.first);
		auto distance = std::distance(begin(v), r0.first);
		EXPECT_EQ(2, distance);
		EXPECT_EQ(7, r0.second);

		auto r1 = eop::find_n(eop::successor(r0.first), eop::predecessor(r0.second), 1);
		EXPECT_EQ(1, *r1.first);
		auto distance1 = std::distance(begin(v), r1.first);
		EXPECT_EQ(6, distance1);
		EXPECT_EQ(3, r1.second);

		auto r2 = eop::find_n(eop::successor(r1.first), eop::predecessor(r1.second), 1);
		EXPECT_EQ(v.end(), r2.first);
		auto distance2 = std::distance(begin(v), r2.first);
		EXPECT_EQ(9, distance2);
		EXPECT_EQ(0, r2.second);
	}

	TEST(iteratorstest, test_find_n_weak_range)
	{
		vector<int> v{ 1, 2, 3, 4 };
		auto r = eop::find_n(v.begin(), v.size(), 2);
		EXPECT_EQ(2, eop::source(r.first));
		r = eop::find_n(v.begin(), v.size(), -1);
		EXPECT_EQ(end(v), r.first) << "error in not found";
	}

	TEST(iteratorstest, test_find_n_empty_weak_range)
	{
		vector<int> v{};
		auto r = eop::find_n(v.begin(), v.size(), 2);
		EXPECT_EQ(end(v), r.first) << "error in not found";
	}

	TEST(iteratorstest, test_find_if_not_n_weak_range)
	{
		vector<int> v{ 1, 2, 3, 4 };
		auto r = eop::find_if_not_n(v.begin(), v.size(), 1);
		EXPECT_EQ(2, eop::source(r.first));
		r = eop::find_if_not_n(r.first, r.second, 2);
		EXPECT_EQ(3, eop::source(r.first));
	}

	TEST(iteratorstest, test_find_if_not_n_empty_weak_range)
	{
		vector<int> v{};
		auto r = eop::find_if_not_n(v.begin(), v.size(), 2);
		EXPECT_EQ(end(v), r.first) << "error in not found";
	}

	TEST(iteratorstest, test_all_n)
	{
		vector<int> v{ 1, 4, 3, 2 };
		bool all = eop::all_n(v.begin(), v.size(), less_Than(10));
		EXPECT_EQ(true, all);
		all = eop::all_n(v.begin(), v.size(), less_Than(4));
		EXPECT_EQ(false, all);
	}

	TEST(iteratorstest, test_all_n_empty)
	{
		vector<int> v{};
		bool all = eop::all_n(v.begin(), v.size(), less_Than(10));
		EXPECT_EQ(true, all);
	}

	TEST(iteratorstest, test_none_n)
	{
		vector<int> v{ 1, 4, 3, 2 };
		bool none = eop::none_n(v.begin(), v.size(), less_Than(0));
		EXPECT_EQ(true, none);
		none = eop::none_n(v.begin(), v.size(), equals_To(0));
		EXPECT_EQ(true, none);
		none = eop::none_n(v.begin(), v.size(), equals_To(3));
		EXPECT_EQ(false, none);
	}

	TEST(iteratorstest, test_none_n_empty)
	{
		vector<int> v{};
		bool none = eop::none_n(v.begin(), v.size(), less_Than(10));
		EXPECT_EQ(true, none);
	}

	TEST(iteratorstest, test_not_all_n)
	{
		vector<int> v{ 1, 4, 3, 2 };
		bool not_all = eop::not_all_n(v.begin(), v.size(), less_Than(10));
		EXPECT_EQ(false, not_all);
		not_all = eop::not_all_n(v.begin(), v.size(), less_Than(4));
		EXPECT_EQ(true, not_all);
	}

	TEST(iteratorstest, test_not_all_n_empty)
	{
		vector<int> v{};
		bool not_all = eop::not_all_n(v.begin(), v.size(), less_Than(10));
		EXPECT_EQ(false, not_all);
	}

	TEST(iteratorstest, test_some_n)
	{
		vector<int> v{ 1, 4, 3, 2 };
		bool some = eop::some_n(v.begin(), v.size(), less_Than(0));
		EXPECT_EQ(false, some);
		some = eop::some_n(v.begin(), v.size(), equals_To(0));
		EXPECT_EQ(false, some);
		some = eop::some_n(v.begin(), v.size(), equals_To(3));
		EXPECT_EQ(true, some);
	}

	TEST(iteratorstest, test_some_n_empty)
	{
		vector<int> v{};
		bool some = eop::some_n(v.begin(), v.size(), less_Than(10));
		EXPECT_EQ(false, some);
	}

	TEST(iteratorstest, test_find_if_ungarded)
	{
		vector<int> v{ 1, 2, 3, 4 };
		auto r = eop::find_if_ungarded(v.begin(), equals_To(3));
		EXPECT_EQ(3, eop::source(r));
	}

	TEST(iteratorstest, test_find_if_not_ungarded)
	{
		vector<int> v{ 1, 2, 3, 4 };
		auto r = eop::find_if_not_ungarded(v.begin(), equals_To(1));
		EXPECT_EQ(2, eop::source(r));
	}

	TEST(iteratorstest, test_find_mismatch_same)
	{
		vector<int> v0{ 1, 2, 3, 4 };
		vector<int> v1{ 1, 2, 3, 4 };
		auto r = eop::find_mismatch(v0.cbegin(), v0.cend(), v1.begin(), v1.end(), std::equal_to<int>());
		EXPECT_EQ(v0.end(), r.first);
		EXPECT_EQ(v1.end(), r.second);
	}

	TEST(iteratorstest, test_find_mismatch_differ)
	{
		vector<int> v0{ 1, 2, 5, 4, 3 };
		vector<int> v1{ 1, 2, 3, 4, 5 };
		{
			auto r = eop::find_mismatch(v0.cbegin(), v0.cend(), v1.begin(), v1.end(), std::equal_to<int>());
			EXPECT_EQ(5, eop::source(r.first));
			EXPECT_EQ(3, eop::source(r.second));
		}
		{
			auto r = eop::find_mismatch(v0.cbegin(), v0.size(), v1.begin(), v1.end(), std::equal_to<int>());
			EXPECT_EQ(5, eop::source(r.first.first));
			EXPECT_EQ(3, r.first.second);
			EXPECT_EQ(3, eop::source(r.second));
		}
		{
			auto r = eop::find_mismatch(v0.cbegin(), v0.cend(), v1.begin(), v1.size(), std::equal_to<int>());
			EXPECT_EQ(5, eop::source(r.first));
			EXPECT_EQ(3, eop::source(r.second.first));
			EXPECT_EQ(3, r.second.second);
		}
		{
			auto r = eop::find_mismatch(v0.cbegin(), v0.size(), v1.begin(), v1.size(), std::equal_to<int>());
			EXPECT_EQ(5, eop::source(r.first.first));
			EXPECT_EQ(3, r.first.second);
			EXPECT_EQ(3, eop::source(r.second.first));
			EXPECT_EQ(3, r.second.second);
		}
	}

	TEST(iteratorstest, test_find_adjacent_mismatch)
	{
		vector<int> v{ 1, 2, 3, 4, 5, 6, 8, 7 };
		auto i = eop::find_adjacent_mismatch(v.cbegin(), v.cend(), std::less<int>());
		ASSERT_NE(v.cend(), i);
		EXPECT_EQ(7, eop::source(i));
	}

	TEST(iteratorstest, test_strictly_increasing_range)
	{
		{
			vector<int> v{};
			bool r = eop::strictly_increasing_range(begin(v), end(v), std::less<int>());
			EXPECT_EQ(true, r) << "empty case failed";
		}
		{
			vector<int> v{ 1 };
			bool r = eop::strictly_increasing_range(begin(v), end(v), std::less<int>());
			EXPECT_EQ(true, r) << "one element case failed";
		}
		{
			vector<int> v{ 1, 2 };
			bool r = eop::strictly_increasing_range(begin(v), end(v), std::less<int>());
			EXPECT_EQ(true, r) << "two elements strictly increasing case failed";
		}
		{
			vector<int> v{ 1, 1 };
			bool r = eop::strictly_increasing_range(begin(v), end(v), std::less<int>());
			EXPECT_EQ(false, r) << "two elements increasing case failed";
		}
		{
			vector<int> v{ 2, 1 };
			bool r = eop::strictly_increasing_range(begin(v), end(v), std::less<int>());
			EXPECT_EQ(false, r) << "two elements non increasing case failed";
		}
	}

	TEST(iteratorstest, test_increasing_range)
	{
		{
			vector<int> v{};
			bool r = eop::increasing_range(begin(v), end(v), std::less<int>());
			EXPECT_EQ(true, r) << "empty case failed";
		}
		{
			vector<int> v{ 1 };
			bool r = eop::increasing_range(begin(v), end(v), std::less<int>());
			EXPECT_EQ(true, r) << "one element case failed";
		}
		{
			vector<int> v{ 1, 2 };
			bool r = eop::increasing_range(begin(v), end(v), std::less<int>());
			EXPECT_EQ(true, r) << "two elements strictly increasing case failed";
		}
		{
			vector<int> v{ 1, 1 };
			bool r = eop::increasing_range(begin(v), end(v), std::less<int>());
			EXPECT_EQ(true, r) << "two elements increasing case failed";
		}
		{
			vector<int> v{ 2, 1 };
			bool r = eop::increasing_range(begin(v), end(v), std::less<int>());
			EXPECT_EQ(false, r) << "two elements non increasing case failed";
		}
	}

	TEST(iteratorstest, test_partitioned)
	{
		vector<int> v{ 7, 9, 6, 8, 3, 4, 1, 5, 2 };
		auto r0 = eop::partitioned(begin(v), end(v), less_Than(6));
		EXPECT_EQ(true, r0);
	}

	TEST(iteratorstest, test_partition_point_n) 
	{
		vector<int> v{ 9, 8, 7, 6, 5, 4, 3, 2, 1 };
		auto r0 = eop::partition_point_n(begin(v), size(v), less_Than(6));
		EXPECT_EQ(5, eop::source(r0));
		auto r1 = eop::partition_point_n(begin(v), size(v), less_Than(4));
		EXPECT_EQ(3, eop::source(r1));
		auto r2 = eop::partition_point_n(begin(v), size(v), less_Than(9));
		EXPECT_EQ(8, eop::source(r2));
		auto r3 = eop::partition_point_n(begin(v), size(v), less_Than(1));
		EXPECT_EQ(end(v), r3);
		auto r4 = eop::partition_point_n(begin(v), size(v), less_Than(0));
		EXPECT_EQ(end(v), r4);
		auto r5 = eop::partition_point_n(begin(v), size(v), less_Than(10));
		EXPECT_EQ(begin(v), r5);
		EXPECT_EQ(9, eop::source(r5));

		vector<int> v_empty {};
		auto r_empty = eop::partition_point_n(begin(v_empty), 0, less_Than(0));
		EXPECT_EQ(end(v_empty), r_empty);

		vector<int> v_one_element{4};
		auto r_one_element_0 = eop::partition_point_n(begin(v_one_element), 1, less_Than(4));
		EXPECT_EQ(end(v_one_element), r_one_element_0);
		auto r_one_element_1 = eop::partition_point_n(begin(v_one_element), 1, less_Than(5));
		EXPECT_EQ(begin(v_one_element), r_one_element_1);
	}

	TEST(iteratorstest, test_partition_point)
	{
		vector<int> v{ 9, 8, 7, 6, 5, 4, 3, 2, 1 };
		auto r0 = eop::partition_point(begin(v), end(v), less_Than(6));
		EXPECT_EQ(5, eop::source(r0));
		auto r1 = eop::partition_point(begin(v), end(v), less_Than(4));
		EXPECT_EQ(3, eop::source(r1));
		auto r2 = eop::partition_point(begin(v), end(v), less_Than(9));
		EXPECT_EQ(8, eop::source(r2));
		auto r3 = eop::partition_point(begin(v), end(v), less_Than(1));
		EXPECT_EQ(end(v), r3);
		auto r4 = eop::partition_point(begin(v), end(v), less_Than(0));
		EXPECT_EQ(end(v), r4);
		auto r5 = eop::partition_point(begin(v), end(v), less_Than(10));
		EXPECT_EQ(begin(v), r5);
		EXPECT_EQ(9, eop::source(r5));
	}

	TEST(iteratorstest, test_lower_bound_n)
	{
		vector<int> v{ 1, 2, 3, 4, 5, 5, 5, 5, 6, 7, 8, 9 };
		auto r0 = eop::lower_bound_n(begin(v), size(v), 5, std::less<int>());
		ASSERT_NE(end(v), r0);
		EXPECT_EQ(5, eop::source(r0));
		EXPECT_EQ(begin(v) + 4, r0);
		auto r1 = eop::lower_bound_n(begin(v), size(v), 4, std::less<int>());
		EXPECT_EQ(4, eop::source(r1));
		auto r2 = eop::lower_bound_n(begin(v), size(v), 9, std::less<int>());
		EXPECT_EQ(9, eop::source(r2));
		auto r3 = eop::lower_bound_n(begin(v), size(v), 1, std::less<int>());
		EXPECT_EQ(begin(v), r3);
		auto r4 = eop::lower_bound_n(begin(v), size(v), 0, std::less<int>());
		EXPECT_EQ(begin(v), r4);
		auto r5 = eop::lower_bound_n(begin(v), size(v), 10, std::less<int>());
		EXPECT_EQ(end(v), r5);

		vector<int> v_empty{};
		auto r_empty = eop::lower_bound_n(begin(v_empty), 0, 0, std::less<int>());
		EXPECT_EQ(end(v_empty), r_empty);

		vector<int> v_one_element{ 4 };
		auto r_one_element_0 = eop::lower_bound_n(begin(v_one_element), 1, 4, std::less<int>());
		EXPECT_EQ(begin(v_one_element), r_one_element_0);
		auto r_one_element_1 = eop::lower_bound_n(begin(v_one_element), 1, 5, std::less<int>());
		EXPECT_EQ(end(v_one_element), r_one_element_1);
	}

	TEST(iteratorstest, test_upper_bound_n)
	{
		vector<int> v{ 1, 2, 3, 4, 5, 5, 5, 5, 6, 7, 8, 9 };
		auto r0 = eop::upper_bound_n(begin(v), size(v), 5, std::less<int>());
		ASSERT_NE(end(v), r0);
		EXPECT_EQ(6, eop::source(r0));
		EXPECT_EQ(begin(v) + 8, r0);
		auto r1 = eop::upper_bound_n(begin(v), size(v), 4, std::less<int>());
		EXPECT_EQ(5, eop::source(r1));
		auto r2 = eop::upper_bound_n(begin(v), size(v), 9, std::less<int>());
		EXPECT_EQ(end(v), r2);
		auto r3 = eop::upper_bound_n(begin(v), size(v), 1, std::less<int>());
		EXPECT_EQ(begin(v) + 1, r3);
		auto r4 = eop::upper_bound_n(begin(v), size(v), 0, std::less<int>());
		EXPECT_EQ(begin(v), r4);
		auto r5 = eop::upper_bound_n(begin(v), size(v), 10, std::less<int>());
		EXPECT_EQ(end(v), r5);

		vector<int> v_empty{};
		auto r_empty = eop::upper_bound_n(begin(v_empty), 0, 0, std::less<int>());
		EXPECT_EQ(end(v_empty), r_empty);

		vector<int> v_one_element{ 4 };
		auto r_one_element_0 = eop::upper_bound_n(begin(v_one_element), 1, 3, std::less<int>());
		EXPECT_EQ(begin(v_one_element), r_one_element_0);
		auto r_one_element_1 = eop::upper_bound_n(begin(v_one_element), 1, 4, std::less<int>());
		EXPECT_EQ(end(v_one_element), r_one_element_1);
		auto r_one_element_2 = eop::upper_bound_n(begin(v_one_element), 1, 5, std::less<int>());
		EXPECT_EQ(end(v_one_element), r_one_element_2);
	}
}
