#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "eop.h"
#include "intrinsics.h"
#include "pointers.h"
#include "tree.h"

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
		EXPECT_EQ(begin (v_one_element), r_one_element_0);
		auto r_one_element_1 = eop::upper_bound_n(begin(v_one_element), 1, 4, std::less<int>());
		EXPECT_EQ(end(v_one_element), r_one_element_1);
		auto r_one_element_2 = eop::upper_bound_n(begin(v_one_element), 1, 5, std::less<int>());
		EXPECT_EQ(end(v_one_element), r_one_element_2);
	}

	TEST(iteratorstest, test_equal_range_n)
	{
		vector<int> v{ 1, 2, 3, 4, 5, 5, 5, 5, 6, 7, 8, 9 };
		auto r0 = eop::equal_range_n(begin(v), size(v), 5, std::less<int>());
		ASSERT_NE(end(v), r0.first);
		ASSERT_NE(end(v), r0.second);
		EXPECT_EQ(5, eop::source(r0.first));
		EXPECT_EQ(6, eop::source(r0.second));
		EXPECT_EQ(begin(v) + 4, r0.first);
		EXPECT_EQ(begin(v) + 8, r0.second);
	}

	TEST(iteratorstest, test_operator_minus)
	{
		vector<int> v{ 1, 2, 3, 4, 5, 5, 5, 5, 6, 7, 8, 9 };
		auto i0 = end(v) - 2;
		EXPECT_EQ(8, eop::source(i0));
		auto i1 = i0 - 3;
		EXPECT_EQ(5, eop::source(i1));
		auto i2 = i1 - 7;
		EXPECT_EQ(begin(v), i2);
	}

	TEST(iteratorstest, test_find_backward_if)
	{
		vector<int> v{ 1, 3, 4, 3, 2 };
		auto r = eop::find_backward_if(v.begin(), v.end(), equals_To(3));
		EXPECT_EQ(3, eop::source(eop::predecessor(r)));
		EXPECT_EQ(2, eop::source(r));
		r = eop::predecessor(r);
		r = eop::find_backward_if(v.begin(), r, equals_To(3));
		EXPECT_EQ(3, eop::source(eop::predecessor(r)));
		EXPECT_EQ(4, eop::source(r));
		r = eop::find_backward_if(v.begin(), v.end(), equals_To(-1));
		EXPECT_EQ(begin(v), r) << "error in not found";
	}

	TEST(iteratorstest, test_find_backward_if_empty)
	{
		vector<int> v{};
		auto r = eop::find_backward_if(v.begin(), v.end(), equals_To(2));
		EXPECT_EQ(begin(v), r) << "error in not found";
	}

	TEST(iteratorstest, test_reverse_iterator_adapter_source)
	{
		vector<int> v = { 1, 2, 3, 4, 5, 6 };
		eop::reverse_iterator<vector<int>::iterator> ria_0( begin(v) + 1 );
		EXPECT_EQ(1, eop::source(ria_0));
		eop::reverse_iterator<vector<int>::iterator> ria_1(end(v));
		EXPECT_EQ(6, eop::source(ria_1));
		eop::reverse_iterator<vector<int>::iterator> begin(begin(v));
		eop::reverse_iterator<vector<int>::iterator> end(end(v));
		vector<int> reverse;
		while (begin != end) {
			reverse.push_back(eop::source(end));
			end = eop::successor(end);
		}
		auto it_rbegin = reverse.rbegin();
		for (int i : v) {
			EXPECT_EQ(i, *it_rbegin);
			++it_rbegin;
		}
	}

	TEST(iteratorstest, test_is_palindrom)
	{
		vector<int> v_0 = { };
		EXPECT_TRUE(eop::is_palindrom(begin(v_0), end(v_0)));
		vector<int> v_1 = { 1 };
		EXPECT_TRUE(eop::is_palindrom(begin(v_1), end(v_1)));
		vector<int> v_2 = { 1, 2, 2, 1 };
		EXPECT_TRUE(eop::is_palindrom(begin(v_2), end(v_2)));
		vector<int> v_3 = { 1, 2, 3, 2, 1 };
		EXPECT_TRUE(eop::is_palindrom(begin(v_3), end(v_3)));
		vector<int> v_4 = { 1, 2, 3, 4, 2, 1 };
		EXPECT_FALSE(eop::is_palindrom(begin(v_4), end(v_4)));
	}

	using eop::begin;

	typedef eop::stree_node<int> SNode;
	typedef eop::stree_coordinate<int> SCoordinate;
	typedef eop::stree<int> STree;
	typedef eop::tree<int> Tree;

	STree create_stree() {
		return STree{ 0,
			STree{ 11 , STree{}, STree{ 13 } },
			STree{ 12 , STree{ 14 }, STree{} } };
	}

	Tree create_tree() {
		return Tree{ 0,
			Tree{ 11 , Tree{}, Tree{ 13 } },
			Tree{ 12 , Tree{ 14 }, Tree{} } };
	}

	TEST(coordinatestest, test_weight_recursive_stree_coordinate)
	{
		//    n_2
		//   /   \
		// n_0   n_1
		//  \     /
		//  n_3 n_4
		typedef eop::stree_node<int> Node;
		typedef eop::stree_coordinate<int> Coordinate;
		Node n_0{ 1 };
		Node n_1{ 2 };
		Node n_2{ 3 , &n_0 , &n_1};
		Coordinate c{ &n_2 };
		auto weight_0 = eop::weight_recursive(c);
		EXPECT_EQ(3, weight_0) << "weight calculation wrong";
		Node n_3{ 4 };
		eop::set_right_successor(Coordinate{ addressof(n_0) }, Coordinate{ addressof(n_3) });
		auto weight_1 = eop::weight_recursive(c);
		EXPECT_EQ(4, weight_1) << "weight calculation wrong";

		Node n_4{ 4 };
		eop::set_left_successor(Coordinate{ addressof(n_1) }, Coordinate{ addressof(n_4) });
		auto weight_2 = eop::weight_recursive(c);
		EXPECT_EQ(5, weight_2) << "weight calculation wrong";
	}

	TEST(coordinatestest, test_weight_recursive_stree)
	{
		EXPECT_EQ(0, eop::stree_node_count) << "Before construct test EOP";
		{
			STree x = create_stree();
			EXPECT_EQ(5, eop::weight_recursive(begin(x)));
		}
		EXPECT_EQ(0, eop::stree_node_count) << "Before construct test EOP";
	}

	TEST(coordinatestest, test_height_recursive)
	{
		//    n_2
		//   /   \
		// n_0   n_1
		//  \     /
		//  n_3 n_4
		typedef eop::stree_node<int> Node;
		typedef eop::stree_coordinate<int> Coordinate;
		Node n_0{ 1 };
		Node n_1{ 2 };
		Node n_2{ 3 , &n_0 , &n_1 };
		Coordinate c{ &n_2 };
		auto weight_0 = eop::height_recursive(c);
		EXPECT_EQ(2, weight_0) << "height calculation wrong";
		Node n_3{ 4 };
		eop::set_right_successor(Coordinate{ addressof(n_0) }, Coordinate{ addressof(n_3) });
		auto weight_1 = eop::height_recursive(c);
		EXPECT_EQ(3, weight_1) << "height calculation wrong";

		Node n_4{ 4 };
		eop::set_left_successor(Coordinate{ addressof(n_1) }, Coordinate{ addressof(n_4) });
		auto weight_2 = eop::height_recursive(c);
		EXPECT_EQ(3, weight_2) << "height calculation wrong";
	}

	TEST(coordinatestest, test_height_recursive_stree)
	{
		EXPECT_EQ(0, eop::stree_node_count) << "Before construct test EOP";
		{
			STree x = create_stree();
			EXPECT_EQ(3, eop::height_recursive(begin(x)));
		}
		EXPECT_EQ(0, eop::stree_node_count) << "Before construct test EOP";
	}

	struct traverse_result 
	{
		typedef eop::stree_coordinate<int> Coordinate;
		vector<int> preorder;
		vector<int> inorder;
		vector<int> postorder;
		void operator()(eop::visit v, Coordinate c)
		{
			if (v == eop::visit::pre)
				preorder.push_back(sink(c.ptr).value);
			else if (v == eop::visit::in)
				inorder.push_back(sink(c.ptr).value);
			else if (v == eop::visit::post)
				postorder.push_back(sink(c.ptr).value);
		}
	};

	TEST(coordinatestest, test_traverse_nonempty)
	{
		// TODO test with a complex tree
		//    n_2
		//   /   \
		// n_0   n_1
		//  \     /
		//  n_3 n_4
		typedef eop::stree_node<int> Node;
		typedef eop::stree_coordinate<int> Coordinate;
		Node n_3{ 4 };
		Node n_4{ 5 };
		Node n_0{ 1, 0, addressof(n_3) };
		Node n_1{ 2, addressof(n_4), 0 };
		Node n_2{ 3 , &n_0 , &n_1 };
		Coordinate c{ &n_2 };
		traverse_result r = eop::traverse_nonempty(c, traverse_result());
		vector<int> pre_expected{ 3, 1, 4, 2, 5 };
		EXPECT_EQ(pre_expected, r.preorder) << "pre order not as expected";
		vector<int> in_expected{ 1, 4, 3, 5, 2 };
		EXPECT_EQ(in_expected, r.inorder) << "in order not as expected";
		vector<int> post_expected{ 4, 1, 5, 2, 3 };
		EXPECT_EQ(post_expected, r.postorder) << "post order not as expected";
	}


	TEST(tree_tests, empty_coordinate_stree)
	{
		SCoordinate c{ 0 };
		EXPECT_TRUE(eop::empty(c));
	}

	TEST(tree_tests, empty_coordinate_tree)
	{
		eop::tree_coordinate<int> c{ 0 };
		EXPECT_TRUE(eop::empty(c));
	}

	TEST(tree_tests, stree_node_construct)
	{
		SNode n0{ 0 };
		EXPECT_EQ(0, n0.value);
		SCoordinate c0{ addressof(n0) };
		EXPECT_FALSE(eop::has_left_successor(c0));
		EXPECT_FALSE(eop::has_right_successor(c0));

		SNode n1{ 1, addressof(n0) };
		SCoordinate c1{ addressof(n1) };
		ASSERT_TRUE(eop::has_left_successor(c1));
		EXPECT_EQ(c0, eop::left_successor(c1));
		EXPECT_FALSE(eop::has_right_successor(c1));

		eop::set_right_successor(c1, c0);
		ASSERT_TRUE(eop::has_right_successor(c1));
		EXPECT_EQ(c0, eop::right_successor(c1));

		SNode n2{ 2, 0, addressof(n0) };
		SCoordinate c2{ addressof(n2) };
		EXPECT_FALSE(eop::has_left_successor(c2));
		ASSERT_TRUE(eop::has_right_successor(c2));
		EXPECT_EQ(c0, eop::right_successor(c2));

		eop::set_left_successor(c2, c0);
		ASSERT_TRUE(eop::has_left_successor(c2));
		EXPECT_EQ(c0, eop::left_successor(c2));
	}

	TEST(tree_tests, tree_node_construct)
	{
		typedef eop::tree_node<int> Node;
		typedef eop::tree_coordinate<int> Coordinate;
		Node n0{ 0 };
		EXPECT_EQ(0, n0.value);
		Coordinate c0{ addressof(n0) };
		EXPECT_FALSE(eop::has_left_successor(c0));
		EXPECT_FALSE(eop::has_right_successor(c0));

		Node n1{ 1, addressof(n0) };
		Coordinate c1{ addressof(n1) };
		ASSERT_TRUE(eop::has_left_successor(c1));
		EXPECT_EQ(c0, eop::left_successor(c1));
		EXPECT_FALSE(eop::has_right_successor(c1));

		eop::set_right_successor(c1, c0);
		ASSERT_TRUE(eop::has_right_successor(c1));
		EXPECT_EQ(c0, eop::right_successor(c1));

		Node n2{ 2, 0, addressof(n0) };
		Coordinate c2{ addressof(n2) };
		EXPECT_FALSE(eop::has_left_successor(c2));
		ASSERT_TRUE(eop::has_right_successor(c2));
		EXPECT_EQ(c0, eop::right_successor(c2));

		eop::set_left_successor(c2, c0);
		ASSERT_TRUE(eop::has_left_successor(c2));
		EXPECT_EQ(c0, eop::left_successor(c2));
	}

	TEST(tree_tests, source)
	{
		SNode n0{ 42 };
		SCoordinate c0{ addressof(n0) };
		EXPECT_EQ(42, eop::source(c0));
	}

	TEST(tree_tests, construct_stree)
	{
		EXPECT_EQ(0, eop::stree_node_count) << "Before construct test";
		{
			STree t = create_stree();
			EXPECT_EQ(5, eop::stree_node_count);
			EXPECT_FALSE(eop::empty(t));
		}
		EXPECT_EQ(0, eop::stree_node_count);
	}

	TEST(tree_tests, construct_tree)
	{
		EXPECT_EQ(0, eop::tree_node_count) << "Before construct test";
		{
			Tree t = create_tree();
			EXPECT_EQ(5, eop::tree_node_count);
			EXPECT_FALSE(eop::empty(t));
		}
		EXPECT_EQ(0, eop::tree_node_count);
	}
}
