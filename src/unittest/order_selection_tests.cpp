#include <array>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <string>
#include <sstream>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "coordinate_iterator_adpater.h"
#include "eop.h"
#include "intrinsics.h"
#include "list.h"
#include "pointers.h"
#include "tree.h"
#include "type_functions.h"

#include "project_7_1.h"

#include "testutils.h"

namespace eop
{	
  using std::vector;

  template<typename T>
  struct value_type<typename std::vector<T>>
  {
    typedef T type;
  };
  
  template<typename T>
  struct value_type<typename std::back_insert_iterator< std::vector<T> > >
  {
    // ValueType of the back_insert_iterator is back_insert_iterator
    typedef std::back_insert_iterator< std::vector<T> > type;
  };

  template<>
  struct iterator_concept<typename std::vector<int>::iterator> 
  {
    typedef random_access_iterator_tag concept;
  };

  template<>
  struct iterator_concept<typename std::vector<std::pair<int, int>>::iterator> 
  {
    typedef random_access_iterator_tag concept;
  };
}

namespace eoptest
{
  using std::array;
  using std::list;
  using std::vector;
  using std::pair;
  using std::swap;
  using std::back_inserter;

  using namespace eop;

  int less(int a, int b) {
    return a < b;
  }

  template<typename T>
    requires(Regular(T))
  auto size(vector<T> const& v) 
  {
    return v.size();
  }

  template<typename T, std::size_t N>
  std::size_t size(array<T, N> const& a)
  {
    return N;
  }

  template<typename T>
    requires(Regular(T))
  auto begin(vector<T> const& v) 
  {
    return v.begin();
  }

  template<typename T>
    requires(Regular(T))
  auto begin(vector<T>& v) 
  {
    return v.begin();
  }

        template<typename T>
                requires(Regular(T))
        auto end(vector<T> const& v) 
        {
                return v.end();
        }

        template<typename T>
                requires(Regular(T))
        auto end(vector<T>& v) 
        {
                return v.end();
        }

        TEST(orbit_tests, test_collision_point)
        {
                // todo implement a linked structure;
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
                const vector<int> v{ 1, 2 };
                auto it = begin(v);
                auto r = eop::increment(it);
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
                EXPECT_TRUE(all);
                all = eop::all(v.begin(), v.end(), less_Than(4));
                EXPECT_FALSE(all);
        }

        TEST(iteratorstest, test_all_empty)
        {
                vector<int> v{ };
                bool all = eop::all(v.begin(), v.end(), less_Than(10));
                EXPECT_TRUE(all);
        }

        TEST(iteratorstest, test_none)
        {
                vector<int> v{ 1, 4, 3, 2 };
                bool none = eop::none(v.begin(), v.end(), less_Than(0));
                EXPECT_TRUE(none);
                none = eop::none(v.begin(), v.end(), equals_To(0));
                EXPECT_TRUE(none);
                none = eop::none(v.begin(), v.end(), equals_To(3));
                EXPECT_FALSE(none);
        }

        TEST(iteratorstest, test_none_empty)
        {
                vector<int> v{};
                bool none = eop::none(v.begin(), v.end(), less_Than(10));
                EXPECT_TRUE(none);
        }

        TEST(iteratorstest, test_not_all)
        {
                vector<int> v{ 1, 4, 3, 2 };
                bool not_all = eop::not_all(v.begin(), v.end(), less_Than(10));
                EXPECT_FALSE(not_all);
                not_all = eop::not_all(v.begin(), v.end(), less_Than(4));
                EXPECT_TRUE(not_all);
        }

        TEST(iteratorstest, test_not_all_empty)
        {
                vector<int> v{};
                bool not_all = eop::not_all(v.begin(), v.end(), less_Than(10));
                EXPECT_FALSE(not_all);
        }

        TEST(iteratorstest, test_some)
        {
                vector<int> v{ 1, 4, 3, 2 };
                bool some = eop::some(v.begin(), v.end(), less_Than(0));
                EXPECT_FALSE(some);
                some = eop::some(v.begin(), v.end(), equals_To(0));
                EXPECT_FALSE(some);
                some = eop::some(v.begin(), v.end(), equals_To(3));
                EXPECT_TRUE(some);
        }

        TEST(iteratorstest, test_some_empty)
        {
                vector<int> v{};
                bool some = eop::some(v.begin(), v.end(), less_Than(10));
                EXPECT_FALSE(some);
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
                EXPECT_TRUE(all);
                all = eop::all_n(v.begin(), v.size(), less_Than(4));
                EXPECT_FALSE(all);
        }

        TEST(iteratorstest, test_all_n_empty)
        {
                vector<int> v{};
                bool all = eop::all_n(v.begin(), v.size(), less_Than(10));
                EXPECT_TRUE(all);
        }

        TEST(iteratorstest, test_none_n)
        {
                vector<int> v{ 1, 4, 3, 2 };
                bool none = eop::none_n(v.begin(), v.size(), less_Than(0));
                EXPECT_TRUE(none);
                none = eop::none_n(v.begin(), v.size(), equals_To(0));
                EXPECT_TRUE(none);
                none = eop::none_n(v.begin(), v.size(), equals_To(3));
                EXPECT_FALSE(none);
        }

        TEST(iteratorstest, test_none_n_empty)
        {
                vector<int> v{};
                bool none = eop::none_n(v.begin(), v.size(), less_Than(10));
                EXPECT_TRUE(none);
        }

        TEST(iteratorstest, test_not_all_n)
        {
                vector<int> v{ 1, 4, 3, 2 };
                bool not_all = eop::not_all_n(v.begin(), v.size(), less_Than(10));
                EXPECT_FALSE(not_all);
                not_all = eop::not_all_n(v.begin(), v.size(), less_Than(4));
                EXPECT_TRUE(not_all);
        }

        TEST(iteratorstest, test_not_all_n_empty)
        {
                vector<int> v{};
                bool not_all = eop::not_all_n(v.begin(), v.size(), less_Than(10));
                EXPECT_FALSE(not_all);
        }

        TEST(iteratorstest, test_some_n)
        {
                vector<int> v{ 1, 4, 3, 2 };
                bool some = eop::some_n(v.begin(), v.size(), less_Than(0));
                EXPECT_FALSE(some);
                some = eop::some_n(v.begin(), v.size(), equals_To(0));
                EXPECT_FALSE(some);
                some = eop::some_n(v.begin(), v.size(), equals_To(3));
                EXPECT_TRUE(some);
        }

        TEST(iteratorstest, test_some_n_empty)
        {
                vector<int> v{};
                bool some = eop::some_n(v.begin(), v.size(), less_Than(10));
                EXPECT_FALSE(some);
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
                        EXPECT_TRUE(r) << "empty case failed";
                }
                {
                        vector<int> v{ 1 };
                        bool r = eop::strictly_increasing_range(begin(v), end(v), std::less<int>());
                        EXPECT_TRUE(r) << "one element case failed";
                }
                {
                        vector<int> v{ 1, 2 };
                        bool r = eop::strictly_increasing_range(begin(v), end(v), std::less<int>());
                        EXPECT_TRUE(r) << "two elements strictly increasing case failed";
                }
                {
                        vector<int> v{ 1, 1 };
                        bool r = eop::strictly_increasing_range(begin(v), end(v), std::less<int>());
                        EXPECT_FALSE(r) << "two elements increasing case failed";
                }
                {
                        vector<int> v{ 2, 1 };
                        bool r = eop::strictly_increasing_range(begin(v), end(v), std::less<int>());
                        EXPECT_FALSE(r) << "two elements non increasing case failed";
                }
        }

        TEST(iteratorstest, test_increasing_range)
        {
                {
                        vector<int> v{};
                        bool r = eop::increasing_range(begin(v), end(v), std::less<int>());
                        EXPECT_TRUE(r) << "empty case failed";
                }
                {
                        vector<int> v{ 1 };
                        bool r = eop::increasing_range(begin(v), end(v), std::less<int>());
                        EXPECT_TRUE(r) << "one element case failed";
                }
                {
                        vector<int> v{ 1, 2 };
                        bool r = eop::increasing_range(begin(v), end(v), std::less<int>());
                        EXPECT_TRUE(r) << "two elements strictly increasing case failed";
                }
                {
                        vector<int> v{ 1, 1 };
                        bool r = eop::increasing_range(begin(v), end(v), std::less<int>());
                        EXPECT_TRUE(r) << "two elements increasing case failed";
                }
                {
                        vector<int> v{ 2, 1 };
                        bool r = eop::increasing_range(begin(v), end(v), std::less<int>());
                        EXPECT_FALSE(r) << "two elements non increasing case failed";
                }
        }

        TEST(iteratorstest, test_partitioned)
        {
                vector<int> v{ 7, 9, 6, 8, 3, 4, 1, 5, 2 };
                auto r0 = eop::partitioned(begin(v), end(v), less_Than(6));
                EXPECT_TRUE(r0);
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
                eop::reverse_iterator<vector<int>::iterator> begin(v.begin());
                eop::reverse_iterator<vector<int>::iterator> end(v.end());
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
        typedef eop::stree_node<int> Node;
        typedef eop::tree_coordinate<int> Coordinate;
        typedef eop::tree<int> Tree;
        typedef eop::slist<int> SList;

        STree create_stree() {
                return STree{ 0,
                        STree{ 11 , STree{}, STree{ 13 } },
                        STree{ 12 , STree{ 14 }, STree{} } };
        }

        STree create_stree7() {
                //      1
                //    /   \
                //   2     3 
                //  / \   / \
                // 4   5 6   7 
                return STree { 1,
                        STree{ 2 , STree{ 4 }, STree{ 5 } },
                        STree{ 3 , STree{ 6 }, STree{ 7 } } };
        }

        //     3
        //   /   \
        //  1     2
        //  \     /
        //   4   5
        Tree create_tree() {
                return Tree{ 3,
                        Tree{ 1 , Tree{}, Tree{ 4 } },
                        Tree{ 2 , Tree{ 5 }, Tree{} } };
        }

        //        3
        //     /     \
        //    1       2
        //   /  \    /  \
        //  4    5  6    7
        Tree create_medium_tree() {
                return Tree{ 3,
                        Tree{ 1 , Tree{ 4}, Tree{ 5, create_tree(), Tree{} } },
                        Tree{ 2 , Tree{ 6 }, Tree{ 7} } };
        }

        TEST(coordinatestest, test_weight_recursive_stree_coordinate)
        {
                //    n_2
                //   /   \
                // n_0   n_1
                //  \     /
                //  n_3 n_4
                typedef eop::tree_node<int> Node;
                typedef eop::tree_coordinate<int> Coordinate;
                Node n_0{ 1 };
                Node n_1{ 2 };
                Node n_2{ 3 , &n_0 , &n_1};
                Coordinate c( &n_2 );
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

        TEST(coordinatestest, test_weight_recursive_tree)
        {
                EXPECT_EQ(0, eop::tree_node_count) << "Before construct test";
                {
                        Tree x = create_tree();
                        EXPECT_EQ(5, eop::weight_recursive(begin(x)));
                }
                EXPECT_EQ(0, eop::tree_node_count) << "After construct test";
        }

        TEST(coordinatestest, test_height_recursive)
        {
                //    n_2
                //   /   \
                // n_0   n_1
                //  \     /
                //  n_3 n_4
                typedef eop::tree_node<int> Node;
                typedef eop::tree_coordinate<int> Coordinate;
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
                EXPECT_EQ(0, eop::stree_node_count) << "Before construct test";
                {
                        STree x = create_stree();
                        EXPECT_EQ(3, eop::height_recursive(begin(x)));
                }
                EXPECT_EQ(0, eop::stree_node_count) << "After construct test";
        }

        TEST(coordinatestest, test_height_recursive_tree)
        {
                EXPECT_EQ(0, eop::tree_node_count) << "Before construct test";
                {
                        Tree x = create_tree();
                        EXPECT_EQ(3, eop::height_recursive(begin(x)));
                }
                EXPECT_EQ(0, eop::tree_node_count) << "After construct test";
        }

        std::ostream& operator<<(std::ostream& s, visit v) {
                switch(v) {
                        case visit::pre  : s << "pre " ; break;
                        case visit::in   : s << "in  "  ; break;
                        case visit::post : s << "post"; break;
                }
                return s;
        }

        template<typename C>
                requires(BifurcateCoordinate(C))
        struct traverse_result
        {
                vector<int> preorder;
                vector<int> inorder;
                vector<int> postorder;
                void operator()(eop::visit v, C c)
                {
                        switch(v) {
                                case eop::visit::pre:  preorder.push_back(source(c));  break;
                                case eop::visit::in:   inorder.push_back(source(c));   break;
                                case eop::visit::post: postorder.push_back(source(c)); break; 
                        }
                }
        };

        TEST(coordinatestest, test_traverse_nonempty)
        {
                //     3
                //   /   \
                //  1     2 
                //  \     /
                //   4   5 
                Tree t { 3,
                        Tree{ 1 , Tree{}, Tree{ 4 } },
                        Tree{ 2 , Tree{ 5 }, Tree{} } };
                traverse_result<Coordinate> r = eop::traverse_nonempty(begin(t), traverse_result<CoordinateType<Tree>>());
                vector<int> pre_expected{ 3, 1, 4, 2, 5 };
                EXPECT_EQ(pre_expected, r.preorder) << "pre order not as expected";
                vector<int> in_expected{ 1, 4, 3, 5, 2 };
                EXPECT_EQ(in_expected, r.inorder) << "in order not as expected";
                vector<int> post_expected{ 4, 1, 5, 2, 3 };
                EXPECT_EQ(post_expected, r.postorder) << "post order not as expected";
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
                eop::set_predecessor(c0, c1);
                ASSERT_TRUE(eop::has_predecessor(c0));
                EXPECT_EQ(c1, eop::predecessor(c0));
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
                        auto root = begin(t);                   // root
                        EXPECT_FALSE(eop::has_predecessor(root));

                        ASSERT_TRUE(eop::has_left_successor(root));
                        auto l = eop::left_successor(root);     // left successor of root
                        ASSERT_TRUE(eop::has_predecessor(l));
                        EXPECT_EQ(root, eop::predecessor(l));
                        EXPECT_TRUE(eop::is_left_successor(l));
                        EXPECT_FALSE(eop::is_right_successor(l));

                        ASSERT_TRUE(eop::has_right_successor(l));
                        auto l_r = eop::right_successor(l);  // right successor of the left successor of root
                        ASSERT_TRUE(eop::has_predecessor(l_r));
                        EXPECT_EQ(l, eop::predecessor(l_r));
                        EXPECT_FALSE(eop::is_left_successor(l_r));
                        EXPECT_TRUE(eop::is_right_successor(l_r));

                        ASSERT_TRUE(eop::has_right_successor(root));
                        auto r = eop::right_successor(root);    // right successor of root
                        ASSERT_TRUE(eop::has_predecessor(r));
                        EXPECT_EQ(root, eop::predecessor(r));
                        EXPECT_FALSE(eop::is_left_successor(r));
                        EXPECT_TRUE(eop::is_right_successor(r));

                        ASSERT_TRUE(eop::has_left_successor(r));
                        auto r_l = eop::left_successor(r);   // left successor of the right successor of root
                        ASSERT_TRUE(eop::has_predecessor(r_l));
                        EXPECT_EQ(r, eop::predecessor(r_l));
                        EXPECT_TRUE(eop::is_left_successor(r_l));
                        EXPECT_FALSE(eop::is_right_successor(r_l));
                }
                EXPECT_EQ(0, eop::tree_node_count);
        }

        TEST(tree_tests, test_traverse_step)
        {
                Tree t = create_tree();
                auto c = begin(t);
                eop::visit v = eop::visit::pre;
                vector<std::pair<eop::CoordinateType<Tree>, eop::visit>> result;
                result.push_back(std::make_pair(c, v));
                do {
                        eop::traverse_step(c, v);
                        result.push_back(std::make_pair(c, v));
                } while (c != begin(t) || v != eop::visit::post);
                EXPECT_EQ(15, result.size());
        }

        TEST(tree_tests, test_reachable)
        {
                typedef eop::tree_coordinate<int> C;
                C x{};
                C y{};
                EXPECT_FALSE(eop::reachable(x, y));
                Tree t = create_tree();
                x = begin(t);
                EXPECT_FALSE(eop::reachable(x, y));
                y = x;
                EXPECT_TRUE(eop::reachable(x, y));
                y = left_successor(x);
                EXPECT_TRUE(eop::reachable(x, y));
                y = right_successor(left_successor(x));
                EXPECT_TRUE(eop::reachable(x, y));
                y = right_successor(x);
                EXPECT_TRUE(eop::reachable(x, y));
                y = left_successor(right_successor(x));
                EXPECT_TRUE(eop::reachable(x, y));
        }

        TEST(tree_tests, test_weight)
        {
                typedef eop::tree_coordinate<int> C;
                C x{};
                EXPECT_EQ(0, eop::weight(x));
                Tree t{ 42 };
                x = begin(t);
                EXPECT_EQ(1, eop::weight(x));
                t = create_tree();
                x = begin(t);
                EXPECT_EQ(5, eop::weight(x));
                t = create_medium_tree();
                x = begin(t);
                EXPECT_EQ(12, eop::weight(x));
        }

        TEST(tree_tests, test_height)
        {
                typedef eop::tree_coordinate<int> C;
                C x{};
                EXPECT_EQ(0, eop::height(x));
                Tree t{ 42 };
                x = begin(t);
                EXPECT_EQ(1, eop::height(x));
                t = create_tree();
                x = begin(t);
                EXPECT_EQ(3, eop::height(x));
                t = create_medium_tree();
                x = begin(t);
                EXPECT_EQ(6, eop::height(x));
        }

        TEST(tree_tests, test_traverse)
        {
                Tree t = create_tree();
                eop::CoordinateType<Tree> c = begin(t);
                traverse_result<eop::CoordinateType<Tree>> r = eop::traverse(c, traverse_result<eop::CoordinateType<Tree>>());
                vector<int> pre_expected{ 3, 1, 4, 2, 5 };
                EXPECT_EQ(pre_expected, r.preorder) << "pre order not as expected";
                vector<int> in_expected{ 1, 4, 3, 5, 2 };
                EXPECT_EQ(in_expected, r.inorder) << "in order not as expected";
                vector<int> post_expected{ 4, 1, 5, 2, 3 };
                EXPECT_EQ(post_expected, r.postorder) << "post order not as expected";
        }

        TEST(tree_tests, test_is_dag_false)
        {
                Tree t = create_tree();
                EXPECT_FALSE(eop::is_dag(begin(t)));
                EXPECT_FALSE(eop::is_dag(begin(t), eop::transformation_trait{}));
        }

        TEST(tree_tests, test_is_dag_true)
        {
                //    n_2 _____
                //   /   \     \
                // n_0   n_1   |
                //  \     \____/
                //  n_3 
                typedef eop::tree_node<int> Node;
                typedef eop::tree_coordinate<int> Coordinate;
                Node n_3{ 4 };  Coordinate c_3{ addressof(n_3) };
                Node n_0{ 1 };  Coordinate c_0{ addressof(n_0) };
                eop::set_right_successor(c_0, c_3);
                Node n_1{ 2 }; Coordinate c_1{ addressof(n_1) };
                Node n_2{ 3 , &n_0 , &n_1 }; Coordinate c_2{ &n_2 };
                eop::set_right_successor(c_1, c_2);
                eop::set_predecessor(c_3, c_0);
                eop::set_predecessor(c_2, c_1);
                eop::set_predecessor(c_1, c_2);
                eop::set_predecessor(c_0, c_2);
                
                EXPECT_TRUE(eop::is_dag(c_2));
                EXPECT_TRUE(eop::is_dag(c_2, eop::transformation_trait{}));
        }

        TEST(project_7_1_tests, test_find)
        {
                Tree t = create_tree();
                for (int i = 1; i <= 5; ++i) {
                        auto c = eop::find(begin(t), i);
                        EXPECT_EQ(i, source(c));
                }
                EXPECT_TRUE(eop::empty(eop::find(begin(t), 0)));
        }

        TEST(project_7_1_tests, test_find_if)
        {
                Tree t = create_tree();
                for (int i = 1; i <= 5; ++i) {
                        auto c = eop::find_if(begin(t), equals_To{ i });
                        EXPECT_EQ(i, source(c));
                }
                EXPECT_TRUE(eop::empty(eop::find_if(begin(t), equals_To{ 0 })));
        }

        TEST(project_7_1_tests, test_find_if_not)
        {
                // 3, 1, 4, 2, 5
                Tree t = create_tree();
                auto c = eop::find_if_not(begin(t), less_Than{ 4 });
                EXPECT_EQ(4, eop::source(c));
                EXPECT_TRUE(eop::empty(eop::find_if_not(begin(t), less_Than{ 10 })));
        }

        TEST(project_7_1_tests, test_all)
        {
                // 3, 1, 4, 2, 5
                Tree t = create_tree();
                EXPECT_FALSE(eop::all(begin(t), less_Than{ 4 }));
                EXPECT_TRUE(eop::all(begin(t), less_Than{ 10 }));
        }

        TEST(project_7_1_tests, test_none)
        {
                // 3, 1, 4, 2, 5
                Tree t = create_tree();
                EXPECT_TRUE(eop::none(begin(t), less_Than{ 0 }));
                EXPECT_FALSE(eop::none(begin(t), less_Than{ 4 }));
                EXPECT_FALSE(eop::none(begin(t), less_Than{ 10 }));
        }

        TEST(project_7_1_tests, test_not_all)
        {
                // 3, 1, 4, 2, 5
                Tree t = create_tree();
                EXPECT_TRUE(eop::not_all(begin(t), less_Than{ 4 }));
                EXPECT_FALSE(eop::not_all(begin(t), less_Than{ 10 }));
        }

        TEST(project_7_1_tests, test_some)
        {
                // 3, 1, 4, 2, 5
                Tree t = create_tree();
                EXPECT_TRUE(eop::some(begin(t), less_Than{ 4 }));
                EXPECT_TRUE(eop::some(begin(t), less_Than{ 10 }));
                EXPECT_FALSE(eop::some(begin(t), less_Than{ 0 }));
        }

        TEST(project_7_1_tests, test_count_if)
        {
                // 3, 1, 4, 2, 5
                Tree t = create_tree();
                EXPECT_EQ(3, eop::count_if(begin(t), less_Than{ 4 }));
                EXPECT_EQ(5, eop::count_if(begin(t), less_Than{ 10 }));
                EXPECT_FALSE(eop::some(begin(t), less_Than{ 0 }));
        }

        TEST(project_7_2_tests, test_construct)
        {
                Tree t{ 1 };
                Coordinate c = begin(t);
                EXPECT_EQ(1, source(c));
                auto i = begin(c);
                EXPECT_EQ(1, source(i));
                auto e = end(c);
                EXPECT_NE(i, e);
                ++i;
                EXPECT_EQ(i, e);
        }

        TEST(project_7_2_tests, test_iterator_pre)
        {
                Tree t = create_tree();
                vector<int> pre_order;
                Coordinate c = begin(t);
                for (int i : c) {
                        pre_order.push_back(i);
                }
                
                vector<int> pre_expected{ 3, 1, 4, 2, 5 };
                EXPECT_EQ(pre_expected, pre_order) << "pre order not as expected";
        }

        TEST(project_7_2_tests, test_iterator_in)
        {
                Tree t = create_tree();
                vector<int> in_order;
                auto root = begin(t);
                auto l = end(root, eop::visit::in);
                for (auto i = begin(root, eop::visit::in); i != l; ++i) {
                        in_order.push_back(source(i));
                }

                vector<int> in_expected{ 1, 4, 3, 5, 2 };
                EXPECT_EQ(in_expected, in_order) << "in order not as expected";
        }

        TEST(project_7_2_tests, test_iterator_post)
        {
                Tree t = create_tree();
                vector<int> post_order;
                auto root = begin(t);
                eop::visit order = eop::visit::post;
                auto l = end(root, order);
                for (auto i = begin(root, order); i != l; ++i) {
                        post_order.push_back(source(i));
                }

                vector<int> post_expected{ 4, 1, 5, 2, 3 };
                EXPECT_EQ(post_expected, post_order) << "post order not as expected";
        }

        TEST(project_7_2_tests, test_iterator_with_find_adjacent_mismatch)
        {
                Tree t = create_tree();
                vector<int> post_order;
                auto root = begin(t);
                eop::visit order = eop::visit::in;
                auto f = begin(root, order);
                auto l = end(root, order);
                auto r = eop::find_adjacent_mismatch(f, l, std::less<int>());
                EXPECT_FALSE(empty(r));
                EXPECT_EQ(3, source(r));
        }

        TEST(bifurcate_isomorphic_tests, test_bifurcate_isomorphic_nonempty)
        {
                Tree t0{ 0 };
                Tree t1{ 1, Tree{}, Tree{} };
                EXPECT_TRUE(eop::bifurcate_isomorphic_nonempty(eop::begin(t0), eop::begin(t1)));

                Tree t2{ 1, t0, Tree{} };
                EXPECT_FALSE(eop::bifurcate_isomorphic_nonempty(eop::begin(t0), eop::begin(t2)));
                EXPECT_FALSE(eop::bifurcate_isomorphic_nonempty(eop::begin(t1), eop::begin(t2)));

                Tree t3{ 3, t1, Tree{} };
                EXPECT_TRUE(eop::bifurcate_isomorphic_nonempty(eop::begin(t2), eop::begin(t3)));

                Tree t4{ 4, t0, t1 };
                EXPECT_FALSE(eop::bifurcate_isomorphic_nonempty(eop::begin(t2), eop::begin(t4)));
                EXPECT_FALSE(eop::bifurcate_isomorphic_nonempty(eop::begin(t3), eop::begin(t4)));
        }

        TEST(bifurcate_isomorphic_tests, test_bifurcate_isomorphic)
        {
                Tree t0{ 0 };
                Tree t1{ 1, Tree{}, Tree{} };
                EXPECT_TRUE(eop::bifurcate_isomorphic(eop::begin(t0), eop::begin(t1)));

                EXPECT_TRUE(eop::bifurcate_isomorphic(eop::begin(Tree{}), eop::begin(Tree{})));
                EXPECT_FALSE(eop::bifurcate_isomorphic(eop::begin(Tree{}), eop::begin(t0)));
                EXPECT_FALSE(eop::bifurcate_isomorphic(eop::begin(t0), eop::begin(Tree{})));

                Tree t2{ 1, t0, Tree{} };
                EXPECT_FALSE(eop::bifurcate_isomorphic(eop::begin(t0), eop::begin(t2)));
                EXPECT_FALSE(eop::bifurcate_isomorphic(eop::begin(t1), eop::begin(t2)));

                Tree t3{ 3, t1, Tree{} };
                EXPECT_TRUE(eop::bifurcate_isomorphic(eop::begin(t2), eop::begin(t3)));

                Tree t4{ 4, t0, t1 };
                EXPECT_FALSE(eop::bifurcate_isomorphic(eop::begin(t2), eop::begin(t4)));
                EXPECT_FALSE(eop::bifurcate_isomorphic(eop::begin(t3), eop::begin(t4)));
        }

        TEST(lexicographical_compare_test, test_lexicographical_equal)
        {
                vector<int> v0{ 1, 2, 3 };
                vector<int> v1{ 1, 2, 3 };
                EXPECT_TRUE(eop::lexicographical_equal(begin(v0), end(v0), begin(v1), end(v1)));
                
                vector<int> v2{ 1, 2, 4 };
                EXPECT_FALSE(eop::lexicographical_equal(begin(v0), end(v0), begin(v2), end(v2)));

                vector<int> v3{ 1, 2, 3, 4 };
                EXPECT_FALSE(eop::lexicographical_equal(begin(v0), end(v0), begin(v3), end(v3)));
        }

        TEST(bifurcate_equivalent_tests, test_bifurcate_equal_nonempty)
        {
                STree t0 = STree{ 3, 
                        STree{1, STree{0}, STree{2}},
                        STree{5, STree{4}, STree{}} };
                STree t1 = STree{ 3,
                        STree{ 1, STree{ 0 }, STree{ 2 } },
                        STree{ 5, STree{ 4 }, STree{} } };
                EXPECT_TRUE(eop::bifurcate_equal_nonempty(begin(t0), begin(t1)));

                STree t2 = STree{ 3,
                        STree{ 1, STree{ 0 }, STree{ 2 } },
                        STree{ 6, STree{ 4 }, STree{} } };
                EXPECT_FALSE(eop::bifurcate_equal_nonempty(begin(t0), begin(t2)));

                STree t3 = STree{ 3,
                        STree{ 1, STree{ 0 }, STree{ 2 } },
                        STree{ 5, STree{ 4 }, STree{ 6 } } };
                EXPECT_FALSE(eop::bifurcate_equal_nonempty(begin(t0), begin(t3)));
        }

        TEST(bifurcate_equivalent_tests, test_bifurcate_equal)
        {
                Tree t0 = Tree{ 3,
                        Tree{ 1, Tree{ 0 }, Tree{ 2 } },
                        Tree{ 5, Tree{ 4 }, Tree{} } };
                Tree t1 = Tree{ 3,
                        Tree{ 1, Tree{ 0 }, Tree{ 2 } },
                        Tree{ 5, Tree{ 4 }, Tree{} } };
                EXPECT_TRUE(eop::bifurcate_equal(begin(t0), begin(t1)));

                Tree t2 = Tree{ 3,
                        Tree{ 1, Tree{ 0 }, Tree{ 2 } },
                        Tree{ 6, Tree{ 4 }, Tree{} } };
                EXPECT_FALSE(eop::bifurcate_equal(begin(t0), begin(t2)));

                Tree t3 = Tree{ 3,
                        Tree{ 1, Tree{ 0 }, Tree{ 2 } },
                        Tree{ 5, Tree{ 4 }, Tree{ 6 } } };
                EXPECT_FALSE(eop::bifurcate_equal(begin(t0), begin(t3)));

                Tree e0{};
                Tree e1{};
                Tree s{ 1 };
                EXPECT_TRUE(eop::bifurcate_equal(begin(e0), begin(e1)));
                EXPECT_FALSE(eop::bifurcate_equal(begin(e0), begin(s)));
                EXPECT_FALSE(eop::bifurcate_equal(begin(s), begin(e1)));
        }

        TEST(bifurcate_equivalent_tests, test_bifurcate_less_nonempty)
        {
                STree t0 = STree{ 3,
                        STree{ 1, STree{ 0 }, STree{ 2 } },
                        STree{ 5, STree{ 4 }, STree{} } };
                STree t1 = STree{ 3,
                        STree{ 1, STree{ 0 }, STree{ 2 } },
                        STree{ 5, STree{ 4 }, STree{} } };
                EXPECT_FALSE(eop::bifurcate_less_nonempty(begin(t0), begin(t1)));
                EXPECT_FALSE(eop::bifurcate_less_nonempty(begin(t1), begin(t0)));

                STree t2 = STree{ 3,
                        STree{ 1, STree{ 0 }, STree{ 2 } },
                        STree{ 6, STree{ 4 }, STree{} } };
                // t0 has smaller value
                EXPECT_TRUE(eop::bifurcate_less_nonempty(begin(t0), begin(t2)));
                EXPECT_FALSE(eop::bifurcate_less_nonempty(begin(t2), begin(t0)));

                STree t3 = STree{ 3,
                        STree{ 1, STree{ 0 }, STree{ 2 } },
                        STree{ 5, STree{ 4 }, STree{ 6 } } };
                // t0 without a right successor
                EXPECT_TRUE(eop::bifurcate_less_nonempty(begin(t0), begin(t3)));
                EXPECT_FALSE(eop::bifurcate_less_nonempty(begin(t3), begin(t0)));

                STree t4 = STree{ 3,
                        STree{ 1, STree{ 0 }, STree{ 2, STree{ 2 }, STree{} } },
                        STree{ 5, STree{ 4 }, STree{} } };
                // t4 has an extra left successor
                EXPECT_TRUE(eop::bifurcate_less_nonempty(begin(t0), begin(t4)));
                EXPECT_FALSE(eop::bifurcate_less_nonempty(begin(t4), begin(t0)));
        }

        TEST(bifurcate_equivalent_tests, test_bifurcate_less)
        {
                Tree t0 = Tree{ 3,
                        Tree{ 1, Tree{ 0 }, Tree{ 2 } },
                        Tree{ 5, Tree{ 4 }, Tree{} } };
                Tree t1 = Tree{ 3,
                        Tree{ 1, Tree{ 0 }, Tree{ 2 } },
                        Tree{ 5, Tree{ 4 }, Tree{} } };
                EXPECT_FALSE(eop::bifurcate_less(begin(t0), begin(t1)));
                EXPECT_FALSE(eop::bifurcate_less(begin(t1), begin(t0)));

                Tree t2 = Tree{ 3,
                        Tree{ 1, Tree{ 0 }, Tree{ 2 } },
                        Tree{ 6, Tree{ 4 }, Tree{} } };
                // t0 has smaller value
                EXPECT_TRUE(eop::bifurcate_less(begin(t0), begin(t2)));
                EXPECT_FALSE(eop::bifurcate_less(begin(t2), begin(t0)));

                Tree t3 = Tree{ 3,
                        Tree{ 1, Tree{ 0 }, Tree{ 2 } },
                        Tree{ 5, Tree{ 4 }, Tree{ 6 } } };
                // t0 without a right successor
                EXPECT_TRUE(eop::bifurcate_less(begin(t0), begin(t3)));
                EXPECT_FALSE(eop::bifurcate_less(begin(t3), begin(t0)));

                Tree t4 = Tree{ 3,
                        Tree{ 1, Tree{ 0 }, Tree{ 2, Tree{ 2 }, Tree{} } },
                        Tree{ 5, Tree{ 4 }, Tree{} } };
                // t4 has an extra left successor
                EXPECT_TRUE(eop::bifurcate_less(begin(t0), begin(t4)));
                EXPECT_FALSE(eop::bifurcate_less(begin(t4), begin(t0)));
        }

        //
        // Chapter 8 Coordinates with Mutable Successors
        //

        // 8.2 Link Rearrangements

        template<typename I>
                requires(Readable(I) && Iterator(I))
        struct iterator_parity_predicate
        {
                bool operator()(I const& x)
                {
                        return source(x) % 2 == 0;
                }
        };

        TEST(link_rearrangements, test_forward_linker)
        {
                eop::slist_node<int> n0{ 1 };
                eop::slist_iterator<int> i0{ addressof(n0) };
                EXPECT_FALSE(eop::empty(i0));
                EXPECT_FALSE(eop::has_successor(i0));

                eop::slist_node<int> n1{ 2 };
                eop::slist_iterator<int> i1{ addressof(n1) };
                EXPECT_FALSE(eop::empty(i1));
                EXPECT_FALSE(eop::has_successor(i1));

                eop::forward_linker<eop::slist_iterator<int>> linker;
                linker(i0, i1);
                EXPECT_TRUE(eop::has_successor(i0));
                EXPECT_FALSE(eop::has_successor(i1));
                EXPECT_EQ(i1, successor(i0));
        }

        TEST(link_rearrangements, test_split_linked)
        {
                test_slist<5> l;
                auto result = eop::split_linked(begin(l), 
                                                                                end(l), 
                                                                                iterator_parity_predicate<eop::IteratorType<SList>>(), 
                                                                                eop::forward_linker<eop::IteratorType<SList>>());

                auto size_odds = result.first.second - result.first.first + 1;
                EXPECT_EQ(2, size_odds);
                auto size_evens = result.second.second - result.second.first + 1;
                EXPECT_EQ(3, size_evens);

                std::vector<int> odds = list_to_vector(result.first.first, result.first.second);
                std::vector<int> expected_odds {1, 3};
                EXPECT_EQ(expected_odds, odds);

                std::vector<int> evens = list_to_vector(result.second.first, result.second.second);
                std::vector<int> expected_evens {0, 2, 4};
                EXPECT_EQ(expected_evens, evens);
        }

        template<typename I>
                requires(Readable(I) && Iterator(I))
        struct iterator_less
        {
                bool operator()(I const& a, I const& b)
                {
                        return source(a) < source(b);
                }
        };

  TEST(link_rearrangements, test_combine_linked_nonempty)
  {
    typedef eop::IteratorType<SList> I;
    test_slist<3> l0(0, 2);
    test_slist<3> l1(1, 2);
    ASSERT_EQ(0, eop::source(begin(l0)));
    ASSERT_EQ(1, eop::source(begin(l1)));

    iterator_less<I> relation;

    std::tuple<I, I, I> result = eop::combine_linked_nonempty(
      begin(l0), end(l0),
      begin(l1), end(l1),
      relation,
      eop::forward_linker<I>());

    auto actual = list_to_vector(std::get<0>(result));
    std::vector<int> expected = {0, 1, 2, 3, 4, 5};
    EXPECT_EQ(expected, actual);
  }

        TEST(link_rearrangements, test_combine_linked_first_empty)
        {
                {
                        SList list0 {};
                        SList list1 {1, 3, 5};
                        EXPECT_EQ(3, eop::slist_node_count());

                        typedef eop::IteratorType<SList> I;

                        auto result = eop::combine_linked(eop::begin(list0), eop::end(list0),
                                                                                          eop::begin(list1), eop::end(list1),
                                                                                          iterator_less<I>(),
                                                                                          eop::forward_linker<I>());

                        auto actual = list_to_vector(std::get<0>(result));
                        std::vector<int> expected = {1, 3, 5};
                        EXPECT_EQ(expected, actual);
                }
                EXPECT_EQ(0, eop::slist_node_count());
        }

        TEST(link_rearrangements, test_combine_linked_second_empty)
        {
                {
                        SList list0 {0, 2, 4};
                        SList list1 {};
                        EXPECT_EQ(3, eop::slist_node_count());

                        typedef eop::IteratorType<SList> I;

                        auto result = eop::combine_linked(eop::begin(list0), eop::end(list0),
                                                                                          eop::begin(list1), eop::end(list1),
                                                                                          iterator_less<I>(),
                                                                                          eop::forward_linker<I>());

                        auto actual = list_to_vector(std::get<0>(result));
                        std::vector<int> expected = {0, 2, 4};
                        EXPECT_EQ(expected, actual);
                }
                EXPECT_EQ(0, eop::slist_node_count());
        }

        TEST(link_rearrangements, test_combine_linked_both_empty)
        {
                {
                        typedef eop::IteratorType<SList> I;
                        SList list0;
                        SList list1;

                        auto result = eop::combine_linked(eop::begin(list0), eop::end(list0),
                                                                                          eop::begin(list1), eop::end(list1),
                                                                                          iterator_less<I>(),
                                                                                          eop::forward_linker<I>());

                        auto actual = list_to_vector(std::get<0>(result));
                        std::vector<int> expected;
                        EXPECT_EQ(expected, actual);
                }
                EXPECT_EQ(0, eop::slist_node_count());
        }

        TEST(link_rearrangements, test_reverse_append)
        {
                test_slist<4> l(1, 1);
                eop::slist_iterator<int> head = eop::reverse_append(begin(l), end(l), end(l), 
                                                                                                        eop::forward_linker<eop::IteratorType<SList>>());
                std::vector<int> actual = list_to_vector(head);
                std::vector<int> expected {4, 3, 2, 1};
                EXPECT_EQ(expected, actual);
        }

        // 8.3 Applications of Link Rearrangements

        template<typename T>
                requires(Integer(T))
        struct parity_predicate
        {
                bool operator()(T x)
                {
                        return x % 2 == 0;
                }
        };

        TEST(applications_of_link_rearrangements, test_partition_linked)
        {
                test_slist<5> list(1);
                auto result = eop::partition_linked(begin(list), 
                                                                                        end(list), 
                                                                                        parity_predicate<int>(), 
                                                                                        eop::forward_linker<eop::IteratorType<SList>>());

                auto size_odds = result.first.second - result.first.first + 1;
                EXPECT_EQ(3, size_odds);
                auto size_evens = result.second.second - result.second.first + 1;
                EXPECT_EQ(2, size_evens);

                std::vector<int> odds = list_to_vector(result.first.first, result.first.second);
                std::vector<int> expected_odds {1, 3, 5};
                EXPECT_EQ(expected_odds, odds);

                std::vector<int> evens = list_to_vector(result.second.first, result.second.second);
                std::vector<int> expected_evens {2, 4};
                EXPECT_EQ(expected_evens, evens);
        }

        TEST(applications_of_link_rearrangements, test_merge_linked_nonempty)
        {
                typedef slist_iterator<int> I;
                test_slist<3> list0(0, 2);
                test_slist<3> list1(1, 2);
                ASSERT_FALSE(eop::empty(begin(list0))) << "List0 is empty";
                ASSERT_FALSE(eop::empty(begin(list1))) << "List1 is empty";
                EXPECT_EQ(0, eop::source(begin(list0)));
                EXPECT_EQ(1, eop::source(begin(list1)));
                EXPECT_TRUE(eop::empty(end(list0)));
                EXPECT_TRUE(eop::empty(end(list1)));

                std::less<int> relation;

                auto result = eop::merge_linked_nonempty(begin(list0), end(list0),
                                                                                                 begin(list1), end(list1),
                                                                                                 relation,
                                                                                                 forward_linker<I>());
                auto actual = list_to_vector(result.first);
                std::vector<int> expected = {0, 1, 2, 3, 4, 5};
                EXPECT_EQ(expected, actual);
        }

        slist_iterator<int> common_test_sort_linked_nonempty_n(SList const& list, std::vector<int> const& expected, std::string const& message)
        {
                auto result = eop::sort_linked_nonempty_n(eop::begin(list), expected.size(),
                                                                                                  std::less<int>(),
                                                                                                  eop::forward_linker<eop::IteratorType<SList>>());                                                                                       
                auto actual = list_to_vector(result.first);
                EXPECT_EQ(expected, actual) << message;
                return result.first;
        }

        void common_test_sort_linked_nonempty_n(std::initializer_list<int> input, std::initializer_list<int> expected, std::string const& message)
        {
                SList l0(input);
                l0.root = common_test_sort_linked_nonempty_n(l0, std::vector<int>(expected), message);
        }

        TEST(applications_of_link_rearrangements, test_sort_linked_nonempty_n)
        {
                {
                        common_test_sort_linked_nonempty_n({4, 0, 2, 5, 1, 3}, {0, 1, 2, 3, 4, 5}, "sort faiiled");
                        common_test_sort_linked_nonempty_n({5, 4, 3, 2, 1, 0}, {0, 1, 2, 3, 4, 5}, "sort faiiled");
                        common_test_sort_linked_nonempty_n({0, 1, 2, 3, 4, 5}, {0, 1, 2, 3, 4, 5}, "sort faiiled");
                        common_test_sort_linked_nonempty_n({0, 1, 2, 5, 3, 4}, {0, 1, 2, 3, 4, 5}, "sort faiiled");
                }
                EXPECT_EQ(0, eop::slist_node_count());
        }

        TEST(applications_of_link_rearrangements, test_unique)
        {
                {
                        SList list{ 0, 0, 1, 2, 2, 1, 3, 3, 4, 4, 4};
                        EXPECT_EQ(11, eop::slist_node_count());
                        auto result = eop::unique(begin(list), 
                                                                                                end(list),
                                                                                                std::equal_to<int>(),
                                                                                                eop::forward_linker<eop::IteratorType<SList>>());

                        SList list_evens;
                        set_root(list_evens, result.second.first); // so it's cleaned uppercase
                        set_forward_link(result.first.second, slist_iterator<int>()); // close first list
                        set_forward_link(result.second.second, slist_iterator<int>()); // close second list                                                                     

                        auto size_odds = result.first.second - result.first.first + 1;
                        EXPECT_EQ(6, size_odds);
                        auto size_evens = result.second.second - result.second.first + 1;
                        EXPECT_EQ(5, size_evens);

                        std::vector<int> odds = list_to_vector(begin(list));
                        std::vector<int> expected_odds {0, 1, 2, 1, 3, 4};
                        EXPECT_EQ(expected_odds, odds);

                        std::vector<int> evens = list_to_vector(begin(list_evens));
                        std::vector<int> expected_evens {0, 2, 3, 4, 4};
                        EXPECT_EQ(expected_evens, evens);
                }
                EXPECT_EQ(0, eop::slist_node_count());
        }

        template<typename C>
        struct mono_traverse_result {
                typedef C first_argument_type;
                std::vector<ValueType(C)> order;
                mono_traverse_result() { order.reserve(16); }
                void operator()(C c) {
                        order.push_back(source(c));
                }
        };

        TEST(linked_bifurcate_coordinates, test_traverse_nonempty)
        {
                STree st = create_stree7();
                auto r = eop::traverse_rotating(begin(st), mono_traverse_result<CoordinateType<STree>>());
                vector<int> expected{ 1, 2, 4, 4, 4, 2, 5, 5, 5, 2, 1, 3, 6, 6, 6, 3, 7, 7, 7, 3, 1 };
                EXPECT_EQ(expected, r.order) << "order not as expected";
        }

        TEST(linked_bifurcate_coordinates, test_traverse_rotating_weight)
        {
                STree st = create_stree7();
                WeightType(CoordinateType<STree>) weight = eop::weight_rotating(begin(st));
                EXPECT_EQ(7, weight);
        }

        TEST(linked_bifurcate_coordinates, test_phased_traverse_nonempty)
        {
                STree st = create_stree7();

                auto result0 = eop::traverse_phased_rotating(begin(st), 0, mono_traverse_result<CoordinateType<STree>>());
                vector<int> expected0{ 1, 4, 5, 2, 6, 3, 7 };
                EXPECT_EQ(expected0, result0.order) << "order not as expected";
                auto result1 = eop::traverse_phased_rotating(begin(st), 1, mono_traverse_result<CoordinateType<STree>>());
                vector<int> expected1{ 2, 4, 5, 1, 6, 7, 3 };
                EXPECT_EQ(expected1, result1.order) << "order not as expected";
                auto result2 = eop::traverse_phased_rotating(begin(st), 2, mono_traverse_result<CoordinateType<STree>>());
                vector<int> expected2{ 4, 2, 5, 3, 6, 7, 1 };
                EXPECT_EQ(expected2, result2.order) << "order not as expected";
        }

  // *******************************************************
  // Chapter 9 - Copying
  // *******************************************************

  TEST(chapter_9_copying, test_copy_empty)
  {
    std::vector<int> x = {};
    std::vector<int> y;
    eop::copy(begin(x), end(x), std::back_inserter(y));
    EXPECT_EQ(0, y.size());
  }
  
  TEST(chapter_9_copying, test_copy)
  {
    std::vector<int> x = {1, 2, 3, 4};
    std::vector<int> y;
    eop::copy(begin(x), end(x), std::back_inserter(y));
    EXPECT_EQ(4, y.size());
    EXPECT_EQ(x, y);
  }

  TEST(chapter_9_copying, test_copy_bounded)
  {
    vector<int> x = {1, 2, 3};
    vector<int> y(3, 0);
    eop::copy_bounded(begin(x), end(x), begin(y), end(y));
    EXPECT_EQ(3, y.size());
    vector<int> expected{1, 2, 3};
    EXPECT_EQ(expected, y);
  }

  TEST(chapter_9_copying, test_copy_n)
  {
    vector<int> x = {1, 2, 3};
    vector<int> y;
    eop::copy_n(begin(x), 3, std::back_inserter(y));
    EXPECT_EQ(3, y.size());
    vector<int> expected{1, 2, 3};
    EXPECT_EQ(expected, y);
  }

  TEST(chapter_9_copying, test_copy_n_0)
  {
    vector<int> x = {1, 2, 3};
    vector<int> y;
    eop::copy_n(begin(x), 0, begin(y));
    EXPECT_EQ(0, y.size());
  }

  TEST(chapter_9_copying, test_copy_backward)
  {
    vector<int> x = {1, 2, 3};
    vector<int> y(3, 0);
    eop::copy_backward(begin(x), end(x), end(y));
    EXPECT_EQ(3, y.size());
    vector<int> expected{1, 2, 3};
    EXPECT_EQ(expected, y);
  }

  TEST(chapter_9_copying, test_copy_backward_n)
  {
    vector<int> x = {1, 2, 3};
    vector<int> y(3, 0);
    eop::copy_backward_n(end(x), 3, end(y));
    EXPECT_EQ(3, y.size());
    vector<int> expected{1, 2, 3};
    EXPECT_EQ(expected, y);
  }

  template<typename I>
  struct is_even_iterator
  {
    bool operator()(I const& i)
    {
      return eop::even(*i);
    }
  };
  
  TEST(chapter_9_copying, test_copy_select)
  {
    vector<int> x = {1, 2, 3, 4, 5};
    vector<int> y;
    eop::copy_select(begin(x), end(x), back_inserter(y),
                     is_even_iterator<vector<int>::iterator>());
    EXPECT_EQ(2, y.size());
    vector<int> expected{2, 4};
    EXPECT_EQ(expected, y);
  }

  TEST(chapter_9_copying, test_split_copy)
  {
    vector<int> x = {1, 2, 3, 4, 5};
    vector<int> y, z;
    eop::split_copy(begin(x), end(x), back_inserter(y), back_inserter(z),
                    is_even_iterator<vector<int>::iterator>());
    vector<int> expected_y{1, 3, 5};
    EXPECT_EQ(expected_y, y);
    vector<int> expected_z{2, 4};
    EXPECT_EQ(expected_z, z);
  }

  TEST(chapter_9_copying, test_split_copy_n)
  {
    vector<int> x = {1, 2, 3, 4, 5};
    vector<int> y, z;
    eop::split_copy_n(begin(x), x.size(), back_inserter(y), back_inserter(z),
                    is_even_iterator<vector<int>::iterator>());
    vector<int> expected_y{1, 3, 5};
    EXPECT_EQ(expected_y, y);
    vector<int> expected_z{2, 4};
    EXPECT_EQ(expected_z, z);
  }

  TEST(chapter_9_copying, test_merge_copy)
  {
    vector<int> y{1, 3, 7, 8};
    vector<int> z{2, 4, 5, 6};
    vector<int> r;
    eop::merge_copy(begin(y), end(y), begin(z), end(z),
                    back_inserter(r), std::less<int>());
    vector<int> expected_r{1, 2, 3, 4, 5, 6, 7, 8};
    EXPECT_EQ(expected_r, r);
  }

  TEST(chapter_9_copying, test_merge_copy_n)
  {
    vector<int> y{1, 3, 7, 8};
    vector<int> z{2, 4, 5, 6};
    vector<int> r;
    auto result = eop::merge_copy_n(begin(y), 4, begin(z), 4,
                      std::back_inserter(r), std::less<int>());
    vector<int> expected_r{1, 2, 3, 4, 5, 6, 7, 8};
    EXPECT_EQ(expected_r, r);
    EXPECT_EQ(end(y), std::get<0>(result));
    EXPECT_EQ(end(z), std::get<1>(result));
  }

  TEST(chapter_9_copying, test_merge_copy_backward_n)
  {
    vector<int> y{8, 7, 3, 1};
    vector<int> z{6, 5, 4, 2};
    vector<int> r(8, 0);
    auto result = eop::merge_copy_backward_n(end(y), 4, end(z), 4, end(r), std::less<int>());
    vector<int> expected_r{8, 7, 6, 5, 4, 3, 2, 1};
    EXPECT_EQ(expected_r, r);
    EXPECT_EQ(begin(y), std::get<0>(result));
    EXPECT_EQ(begin(z), std::get<1>(result));
    EXPECT_EQ(begin(r), std::get<2>(result));
  }

  TEST(chapter_9_copying, test_merge_copy_backward_n_one_empty)
  {
    vector<int> y{};
    vector<int> z{6, 5, 4, 2};
    vector<int> r(4, 0);
    auto result = eop::merge_copy_backward_n(end(y), 0, end(z), 4, end(r), std::less<int>());
    vector<int> expected_r{6, 5, 4, 2};
    EXPECT_EQ(expected_r, r);
    EXPECT_EQ(begin(y), std::get<0>(result));
    EXPECT_EQ(begin(z), std::get<1>(result));
    EXPECT_EQ(begin(r), std::get<2>(result));
  }

  TEST(chapter_9_4_swap_ranges, test_swap_ranges_empty)
  {
    vector<int> x, y;
    auto r = eop::swap_ranges(begin(x), end(x), begin(y));
    EXPECT_EQ(end(y), r);
    EXPECT_EQ(0, x.size());
    EXPECT_EQ(0, y.size());
  }

  TEST(chapter_9_4_swap_ranges, test_swap_ranges)
  {
    vector<int> x {1, 2, 3, 4};
    vector<int> y {5, 6, 7, 8};
    auto r = eop::swap_ranges(begin(x), end(x), begin(y));
    EXPECT_EQ(end(y), r);
    vector<int> e_x {5, 6, 7, 8};
    vector<int> e_y {1, 2, 3, 4};
    EXPECT_EQ(e_x, x);
    EXPECT_EQ(e_y, y);
  }

  TEST(chapter_9_4_swap_ranges, test_swap_ranges_bounded)
  {
    vector<int> x {1, 2, 3, 4};
    vector<int> y {5, 6, 7, 8};
    auto r = eop::swap_ranges_bounded(begin(x), end(x), begin(y), end(y));
    EXPECT_EQ(end(x), r.first);
    EXPECT_EQ(end(y), r.second);
    vector<int> e_x {5, 6, 7, 8};
    vector<int> e_y {1, 2, 3, 4};
    EXPECT_EQ(e_x, x);
    EXPECT_EQ(e_y, y);
  }

  TEST(chapter_9_4_swap_ranges, test_swap_ranges_bounded_not_all)
  {
    vector<int> x {1, 2, 3, 4};
    vector<int> y {5, 6, 7, 8};
    auto r = eop::swap_ranges_bounded(begin(x), begin(x) + 2, begin(y), begin(y) + 3);
    EXPECT_EQ(begin(x) + 2, r.first);
    EXPECT_EQ(begin(y) + 2, r.second);
    vector<int> e_x {5, 6, 3, 4};
    vector<int> e_y {1, 2, 7, 8};
    EXPECT_EQ(e_x, x);
    EXPECT_EQ(e_y, y);
  }

  TEST(chapter_9_4_swap_ranges, test_swap_ranges_n)
  {
    vector<int> x {1, 2, 3, 4};
    vector<int> y {5, 6, 7, 8};
    auto r = eop::swap_ranges_n(begin(x), begin(y), size(x));
    EXPECT_EQ(end(x), r.first);
    EXPECT_EQ(end(y), r.second);
    vector<int> e_x {5, 6, 7, 8};
    vector<int> e_y {1, 2, 3, 4};
    EXPECT_EQ(e_x, x);
    EXPECT_EQ(e_y, y);
  }

  TEST(chapter_9_4_swap_ranges, test_swap_ranges_n_not_all)
  {
    vector<int> x {1, 2, 3, 4};
    vector<int> y {5, 6, 7, 8};
    auto r = eop::swap_ranges_n(begin(x), begin(y), 2);
    EXPECT_EQ(begin(x) + 2, r.first);
    EXPECT_EQ(begin(y) + 2, r.second);
    vector<int> e_x {5, 6, 3, 4};
    vector<int> e_y {1, 2, 7, 8};
    EXPECT_EQ(e_x, x);
    EXPECT_EQ(e_y, y);
  }

  TEST(chapter_9_4_swap_ranges, test_reverse_swap_ranges_empty)
  {
    vector<int> x, y;
    auto r = eop::reverse_swap_ranges(begin(x), end(x), begin(y));
    EXPECT_EQ(end(y), r);
    EXPECT_EQ(0, x.size());
    EXPECT_EQ(0, y.size());
  }

  TEST(chapter_9_4_swap_ranges, test_reverse_swap_ranges)
  {
    vector<int> x {1, 2, 3, 4};
    vector<int> y {5, 6, 7, 8};
    auto r = eop::reverse_swap_ranges(begin(x), end(x), begin(y));
    EXPECT_EQ(end(y), r);
    vector<int> e_x {8, 7, 6, 5};
    vector<int> e_y {4, 3, 2, 1};
    EXPECT_EQ(e_x, x);
    EXPECT_EQ(e_y, y);
  }

  TEST(chapter_9_4_swap_ranges, test_reverse_swap_ranges_bounded)
  {
    vector<int> x {1, 2, 3, 4};
    vector<int> y {5, 6, 7, 8};
    auto r = eop::reverse_swap_ranges_bounded(begin(x), end(x), begin(y), end(y));
    EXPECT_EQ(begin(x), r.first);
    EXPECT_EQ(end(y), r.second);
    vector<int> e_x {8, 7, 6, 5};
    vector<int> e_y {4, 3, 2, 1};
    EXPECT_EQ(e_x, x);
    EXPECT_EQ(e_y, y);
  }

  TEST(chapter_9_4_swap_ranges, test_reverse_swap_ranges_bounded_not_all)
  {
    vector<int> x {0, 1, 2, 3, 4};
    vector<int> y {5, 6, 7, 8, 9};
    auto r = eop::reverse_swap_ranges_bounded(begin(x) + 3, end(x), begin(y), begin(y) + 3);
    EXPECT_EQ(begin(x) + 3, r.first);
    EXPECT_EQ(begin(y) + 2, r.second);
    vector<int> e_x {0, 1, 2, 6, 5};
    vector<int> e_y {4, 3, 7, 8, 9};
    EXPECT_EQ(e_x, x);
    EXPECT_EQ(e_y, y);
  }

  TEST(chapter_9_4_swap_ranges, test_reverse_swap_ranges_n)
  {
    vector<int> x {1, 2, 3, 4};
    vector<int> y {5, 6, 7, 8};
    auto r = eop::reverse_swap_ranges_n(end(x), begin(y), size(x));
    EXPECT_EQ(begin(x), r.first);
    EXPECT_EQ(end(y), r.second);
    vector<int> e_x {8, 7, 6, 5};
    vector<int> e_y {4, 3, 2, 1};
    EXPECT_EQ(e_x, x);
    EXPECT_EQ(e_y, y);
  }

  TEST(chapter_9_4_swap_ranges, test_reverse_swap_ranges_n_not_all)
  {
    vector<int> x {0, 1, 2, 3, 4};
    vector<int> y {5, 6, 7, 8, 9};
    auto r = eop::reverse_swap_ranges_n(end(x), begin(y), 2);
    EXPECT_EQ(begin(x) + 3, r.first);
    EXPECT_EQ(begin(y) + 2, r.second);
    vector<int> e_x {0, 1, 2, 6, 5};
    vector<int> e_y {4, 3, 7, 8, 9};
    EXPECT_EQ(e_x, x);
    EXPECT_EQ(e_y, y);
  }

  template<typename I0, typename I1>
  struct permutation
  {
    const I0 first_input;
    const I1 first_permutation;
    permutation(I0 first_input, I1 first_permutation) : first_input(first_input), first_permutation(first_permutation) {}
    I0 operator()(I0 i) const
    {
      size_t n = *(first_permutation + std::distance(first_input, i)) - 1;
      return first_input + n;
    }
  };

  TEST(chapter_10_2_rearrangements, test_cycle_to_even)
  {
    vector<int> i_v {1, 2, 3, 4};
    vector<int> p_v {3, 2, 4, 1};
    permutation<vector<int>::iterator, vector<int>::iterator> p(begin(i_v), begin(p_v));
    eop::cycle_to(begin(i_v), p);
    vector<int> e_v {4, 2, 1, 3};
    EXPECT_EQ(e_v, i_v);
  }

  TEST(chapter_10_2_rearrangements, test_cycle_to_2n_even)
  {
    vector<int> i_v {1, 2, 3, 4};
    vector<int> p_v {3, 2, 4, 1};
    permutation<vector<int>::iterator, vector<int>::iterator> p(begin(i_v), begin(p_v));
    eop::cycle_to_2n(begin(i_v), p);
    vector<int> e_v {4, 2, 1, 3};
    EXPECT_EQ(e_v, i_v);
  }

  TEST(chapter_10_2_rearrangements, test_cycle_to_odd)
  {
    vector<int> i_v {1, 2, 3, 4, 5, 6, 7};
    vector<int> p_v {3, 5, 2, 6, 7, 1, 4};
    permutation<vector<int>::iterator, vector<int>::iterator> p(begin(i_v), begin(p_v));
    eop::cycle_to(begin(i_v), p);
    vector<int> e_v {6, 3, 1, 7, 2, 4, 5};
    EXPECT_EQ(e_v, i_v);
  }

  TEST(chapter_10_2_rearrangements, test_cycle_to_2n_odd)
  {
    vector<int> i_v {1, 2, 3, 4, 5, 6, 7};
    vector<int> p_v {3, 5, 2, 6, 7, 1, 4};
    permutation<vector<int>::iterator, vector<int>::iterator> p(begin(i_v), begin(p_v));
    eop::cycle_to_2n(begin(i_v), p);
    vector<int> e_v {6, 3, 1, 7, 2, 4, 5};
    EXPECT_EQ(e_v, i_v);
  }

  TEST(chapter_10_2_rearrangements, test_cycle_from)
  {
    vector<int> i_v {1, 2, 3, 4};
    vector<int> p_v {3, 2, 4, 1};
    permutation<vector<int>::iterator, vector<int>::iterator> p(begin(i_v), begin(p_v));
    eop::cycle_from(begin(i_v), p);
    vector<int> e_v {3, 2, 4, 1};
    EXPECT_EQ(e_v, i_v);
  }

  TEST(chapter_10_2_rearrangements, test_cycle)
  {
    vector<int> i_v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    vector<int> p_v {3, 5, 2, 6, 7, 1, 4, 9, 8, 12, 13, 11, 10};
    permutation<vector<int>::iterator, vector<int>::iterator> p(begin(i_v), begin(p_v));
    eop::cycle(begin(i_v), size(i_v), p);
    vector<int> e_v {6, 3, 1, 7, 2, 4, 5, 9, 8, 13, 12, 10, 11};
    EXPECT_EQ(e_v, i_v);
  }

  TEST(chapter_10_3_rearangements, test_reverse_n_indexed)
  {
    vector<int> v0 = {1, 2, 3, 4, 5, 6};
    eop::reverse_n_indexed(begin(v0), size(v0));
    vector<int> e_v0 = {6, 5, 4, 3, 2, 1};
    EXPECT_EQ(e_v0, v0);
    vector<int> v1 = {1, 2, 3, 4, 5};
    eop::reverse_n_indexed(begin(v1), size(v1));
    vector<int> e_v1 = {5, 4, 3, 2, 1};
    EXPECT_EQ(e_v1, v1);
  }

  TEST(chapter_10_3_rearangements, test_reverse_n_random_access)
  {
    vector<int> v0 = {1, 2, 3, 4, 5, 6};
    eop::reverse_n_random_access(begin(v0), size(v0));
    vector<int> e_v0 = {6, 5, 4, 3, 2, 1};
    EXPECT_EQ(e_v0, v0);
    vector<int> v1 = {1, 2, 3, 4, 5};
    eop::reverse_n_random_access(begin(v1), size(v1));
    vector<int> e_v1 = {5, 4, 3, 2, 1};
    EXPECT_EQ(e_v1, v1);
  }

  TEST(chapter_10_3_rearangements, test_reverse_bidirectional)
  {
    vector<int> v0 = {1, 2, 3, 4, 5, 6};
    eop::reverse_bidirectional(begin(v0), end(v0));
    vector<int> e_v0 = {6, 5, 4, 3, 2, 1};
    EXPECT_EQ(e_v0, v0);
    vector<int> v1 = {1, 2, 3, 4, 5};
    eop::reverse_bidirectional(begin(v1), end(v1));
    vector<int> e_v1 = {5, 4, 3, 2, 1};
    EXPECT_EQ(e_v1, v1);
  }

  TEST(chapter_10_3_rearangements, test_reverse_n_bidirectional)
  {
    vector<int> v0 = {1, 2, 3, 4, 5, 6};
    eop::reverse_n_bidirectional(begin(v0), end(v0), size(v0));
    vector<int> e_v0 = {6, 5, 4, 3, 2, 1};
    EXPECT_EQ(e_v0, v0);
    vector<int> v1 = {1, 2, 3, 4, 5};
    eop::reverse_n_bidirectional(begin(v1), end(v1), size(v1));
    vector<int> e_v1 = {5, 4, 3, 2, 1};
    EXPECT_EQ(e_v1, v1);
  }

  TEST(chapter_10_3_rearangements, test_reverse_n_forward)
  {
    vector<int> v0 = {1, 2, 3, 4, 5, 6};
    eop::reverse_n_forward(begin(v0), size(v0));
    vector<int> e_v0 = {6, 5, 4, 3, 2, 1};
    EXPECT_EQ(e_v0, v0);
    vector<int> v1 = {1, 2, 3, 4, 5};
    eop::reverse_n_forward(begin(v1), size(v1));
    vector<int> e_v1 = {5, 4, 3, 2, 1};
    EXPECT_EQ(e_v1, v1);
  }

  TEST(chapter_10_3_rearrangements, test_reverse_n_adaptive)
  {
    array<int, 4> buffer;
    vector<int> v0 = {1, 2, 3, 4, 5, 6, 7, 8};
    eop::reverse_n_adaptive(begin(v0), size(v0), begin(buffer), buffer.size());
    vector<int> e_v0 = {8, 7,  6, 5, 4, 3, 2, 1};
    EXPECT_EQ(e_v0, v0);
    vector<int> v1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    eop::reverse_n_adaptive(begin(v1), size(v1), begin(buffer), buffer.size());
    vector<int> e_v1 = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    EXPECT_EQ(e_v1, v1);    
  }

  TEST(chapter_10_4_rotate, test_rotate_cycles)
  {
    array<int, 9> a0 {1, 2, 3, 4, 5, 6, 7, 8, 9};
    array<int, 9> expected_a0 {7, 8, 9, 1, 2, 3, 4, 5, 6};
    const int k = 6;
    eop::k_rotate_from_permutation_random_access<int*> from(begin(a0), begin(a0) + k, end(a0));
    auto m_prime = eop::rotate_cycles<pointer(int)>(begin(a0), begin(a0) + k, end(a0), from);
    int a = source(m_prime);
    EXPECT_EQ(source(m_prime), 1);
    EXPECT_EQ(a0, expected_a0);
  }

  TEST(chapter_10_4_rotate, test_rotate_random_access_nontrivial)
  {
    array<int, 9> a0 {1, 2, 3, 4, 5, 6, 7, 8, 9};
    array<int, 9> expected_a0 {7, 8, 9, 1, 2, 3, 4, 5, 6};
    const int k = 6;
    auto m_prime = eop::rotate_random_access_nontrivial<pointer(int)>(begin(a0), begin(a0) + k, end(a0));
    int a = source(m_prime);
    EXPECT_EQ(1, source(m_prime));
    EXPECT_EQ(expected_a0, a0);
  }

  TEST(chapter_10_4_rotate, test_rotate_indexed_nontrivial)
  {
    array<int, 9> a0 {1, 2, 3, 4, 5, 6, 7, 8, 9};
    array<int, 9> expected_a0 {7, 8, 9, 1, 2, 3, 4, 5, 6};
    const int k = 6;
    auto m_prime = eop::rotate_indexed_nontrivial<pointer(int)>(begin(a0), begin(a0) + k, end(a0));
    int a = source(m_prime);
    EXPECT_EQ(1, source(m_prime));
    EXPECT_EQ(expected_a0, a0);
  }

  void expect_rotate_bidirectional_nontrivial(list<int> & input, list<int> const& expected, const int k)
  {
    auto m_prime = eop::rotate_bidirectional_nontrivial<list<int>::iterator>(begin(input), end(input) - k, end(input));
    if (!input.empty()) {
      int a = source(m_prime);
      EXPECT_EQ(1, source(m_prime));
    }
    EXPECT_EQ(expected, input);
  }

  TEST(chapter_10_4_rotate, test_rotate_bidirectional)
  {
    list<int> test_input_0 {1, 2, 3, 4, 5, 6, 7, 8, 9};
    list<int> expected_0 {7, 8, 9, 1, 2, 3, 4, 5, 6};
    expect_rotate_bidirectional_nontrivial(test_input_0, expected_0, 3);
    list<int> test_input_1 { };
    list<int> expected_1 { };
    expect_rotate_bidirectional_nontrivial(test_input_1, expected_1, 0);
    list<int> test_input_2 {1, 2, 3, 4, 5, 6, 7, 8};
    list<int> expected_2 {4, 5, 6, 7, 8, 1, 2, 3};
    expect_rotate_bidirectional_nontrivial(test_input_2, expected_2, 5);
    list<int> test_input_3 {1, 2};
    list<int> expected_3 {2, 1};
    expect_rotate_bidirectional_nontrivial(test_input_3, expected_3, 1);
  }

  // copying the input, so I don't need to redefine it
  void expect_rotate_forward_nontrivial(list<int> input, list<int> const& expected, const int k)
  {
    auto m_prime = eop::rotate_forward_nontrivial<list<int>::iterator>(begin(input), end(input) - k, end(input));
    if (!input.empty()) {
      int a = source(m_prime);
      EXPECT_EQ(1, source(m_prime));
    }
    EXPECT_EQ(expected, input);
  }

  TEST(chapter_10_4_rotate, test_rotate_forward_nontrivial)
  {
    list<int> test_input_0 { };
    list<int> expected_0 { };
    expect_rotate_forward_nontrivial(test_input_0, expected_0, 0);    
    list<int> test_input_1 {1, 2};
    list<int> expected_1 {2, 1};
    expect_rotate_forward_nontrivial(test_input_1, expected_1, 1);
    list<int> test_input_2 {1, 2, 3, 4, 5, 6, 7, 8};
    list<int> expected_2 {6, 7, 8, 1, 2, 3, 4, 5};
    expect_rotate_forward_nontrivial(test_input_2, expected_2, 3);
    list<int> expected_3 {4, 5, 6, 7, 8, 1, 2, 3};
    expect_rotate_forward_nontrivial(test_input_2, expected_3, 5);
    list<int> expected_4 {5, 6, 7, 8, 1, 2, 3, 4};
    expect_rotate_forward_nontrivial(test_input_2, expected_4, 4);
  }

  TEST(chapter_10_4_rotate, test_rotate_forward_nontrivial_large)
  {
    std::srand(std::time(0));
    constexpr size_t SIZE = 12000;
    list<int> test_input(SIZE);
    std:iota(begin(test_input), end(test_input), 0);
    const size_t K = std::rand() % SIZE;
    auto m_prime = eop::rotate_forward_nontrivial<list<int>::iterator>(begin(test_input), end(test_input) - K, end(test_input));
    EXPECT_EQ(0, source(m_prime));
    list<int> expected(SIZE);
    std::iota(begin(expected), begin(expected) + K, SIZE - K);
    std::iota(begin(expected) + K, end(expected), 0);
    EXPECT_EQ(expected, test_input);
  }

  TEST(chapter_11_1_partition, test_partitioned_at_point)
  {
     vector<int> v0 {1, 3, 5, 2, 4};
     bool result0 = eop::partitioned_at_point(begin(v0), begin(v0)+3, end(v0), eop::is_Even<int>());
     EXPECT_TRUE(result0);
     vector<int> v1 {1, 3, 2, 5, 4};
     bool result1 = eop::partitioned_at_point(begin(v1), begin(v1)+3, end(v1), eop::is_Even<int>());
     EXPECT_FALSE(result1);
     vector<int> v2 {1, 3, 5, 7, 4};
     bool result2 = eop::partitioned_at_point(begin(v2), begin(v2)+3, end(v2), eop::is_Even<int>());
     EXPECT_FALSE(result2);
     vector<int> v3 {1, 3, 5, 2, 7};
     bool result3 = eop::partitioned_at_point(begin(v3), begin(v3)+3, end(v3), eop::is_Even<int>());
     EXPECT_FALSE(result3);
  }

  TEST(chapter_11_1_partition, test_potential_partition_point)
  {
     vector<int> v0 {1, 3, 5, 2, 4};
     auto result0 = eop::potential_partition_point(begin(v0), end(v0), eop::is_Even<int>());
     EXPECT_EQ(begin(v0)+3, result0);
     auto result1 = eop::potential_partition_point(begin(v0), end(v0), eop::is_Odd<int>());
     EXPECT_EQ(begin(v0)+2, result1);

     vector<int> v2 {1, 3, 5, 7, 9};
     auto result2 = eop::potential_partition_point(begin(v2), end(v2), eop::is_Even<int>());
     EXPECT_EQ(begin(v2)+5, result2);
     auto result3 = eop::potential_partition_point(begin(v2), end(v2), eop::is_Odd<int>());
     EXPECT_EQ(begin(v2), result3);

     vector<int> v4 {};
     auto result4 = eop::potential_partition_point(begin(v4), end(v4), eop::is_Even<int>());
     EXPECT_EQ(begin(v4), result4);
  }

  TEST(chapter_11_1_partition, test_partition_semistable)
  {
    vector<int> v { 1, 2, 3, 9, 6, 7, 4, 5};
    auto m = eop::partition_semistable(begin(v), end(v), eop::is_Even<int>());
    vector<int> expected {1, 3, 9, 7, 5, 2, 4, 6};
    EXPECT_EQ(expected, v);
    EXPECT_TRUE(eop::partitioned_at_point(begin(v), m, end(v), eop::is_Even<int>())) << "Not partitioned at the returned iterator";
  }

  TEST(chapter_11_1_partition, test_partition_semistable_expanded)
  {
    vector<int> v { 1, 2, 3, 9, 6, 7, 4, 5};
    auto m = eop::partition_semistable_expanded(begin(v), end(v), eop::is_Even<int>());
    vector<int> expected {1, 3, 9, 7, 5, 2, 4, 6};
    EXPECT_EQ(expected, v);
    EXPECT_TRUE(eop::partitioned_at_point(begin(v), m, end(v), eop::is_Even<int>())) << "Not partitioned at the returned iterator";

    vector<int> v1 { 1, 2 };
    eop::partition_semistable_expanded(begin(v1), end(v1), eop::is_Even<int>());
    vector<int> expected1 {1, 2};
    EXPECT_EQ(expected1, v1);    
  }

  TEST(chapter_11_1_partition, test_semipartition)
  {
    vector<int> v { 1, 2, 3, 9, 6, 7, 4, 5};
    auto m = eop::semipartition(begin(v), end(v), eop::is_Even<int>());
    vector<int> expected {2, 4, 6};
    EXPECT_TRUE(eop::equals(m, end(v), begin(expected), end(expected)));
  }

  TEST(chapter_11_1_partition, test_partition)
  {
    vector<int> v0 { 1, 2, 3, 9, 6, 7, 4, 5};
    eop::partition(begin(v0), end(v0), eop::is_Even<int>());
    vector<int> expected0 {3, 9, 7, 5, 1, 2, 4, 6};
    EXPECT_EQ(expected0, v0);

    vector<int> v1 { 1, 2 };
    eop::partition(begin(v1), end(v1), eop::is_Even<int>());
    vector<int> expected1 {1, 2};
    EXPECT_EQ(expected1, v1);
  }

  TEST(chapter_11_1_partition, test_partition_forward)
  {
    vector<int> v0 { 1, 2, 3, 9, 6, 7, 4, 5};
    auto m0 = eop::partition_forward(begin(v0), end(v0), eop::is_Even<int>());
    vector<int> expected0 {1, 7, 3, 9, 5, 2, 4, 6};
    EXPECT_EQ(expected0, v0);
    EXPECT_TRUE(eop::partitioned_at_point(begin(v0), m0, end(v0), eop::is_Even<int>())) << "Not partitioned at the returned iterator";

    vector<int> v1 { 1, 2 };
    auto m1 = eop::partition_forward(begin(v1), end(v1), eop::is_Even<int>());
    vector<int> expected1 {1, 2};
    EXPECT_EQ(expected1, v1);

    vector<int> v2 { 2, 1 };
    auto m2 = eop::partition_forward(begin(v2), end(v2), eop::is_Even<int>());
    vector<int> expected2 {1, 2};
    EXPECT_EQ(expected2, v2);
  }

  TEST(chapter_11_1_partition, test_partition_single_cycle)
  {
    vector<int> v0 { 1, 2 };
    auto m0 = eop::partition_single_cycle(begin(v0), end(v0), eop::is_Even<int>());
    vector<int> expected0 {1, 2};
    EXPECT_EQ(expected0, v0);

    vector<int> v1 { 2, 1 };
    auto m1 = eop::partition_single_cycle(begin(v1), end(v1), eop::is_Even<int>());
    vector<int> expected1 {1, 2};
    EXPECT_EQ(expected1, v1);

    vector<int> v2 { 1, 2, 3, 9, 6, 7, 4, 5};
    auto m2 = eop::partition_single_cycle(begin(v2), end(v2), eop::is_Even<int>());
    vector<int> expected2 {1, 7, 3, 9, 5, 6, 4, 2};
    EXPECT_EQ(expected2, v2);
    EXPECT_TRUE(eop::partitioned_at_point(begin(v2), m2, end(v2), eop::is_Even<int>())) << "Not partitioned at the returned iterator";

    vector<int> v3 { 1, 3, 9, 7, 5};
    auto m3 = eop::partition_single_cycle(begin(v3), end(v3), eop::is_Even<int>());
    vector<int> expected3 {1, 3, 9, 7, 5};
    EXPECT_EQ(expected3, v3);
    EXPECT_TRUE(eop::partitioned_at_point(begin(v3), m3, end(v3), eop::is_Even<int>())) << "Not partitioned at the returned iterator";

    vector<int> v4 { 2, 4, 6, 8};
    auto m4 = eop::partition_single_cycle(begin(v4), end(v4), eop::is_Even<int>());
    vector<int> expected4 {2, 4, 6, 8};
    EXPECT_EQ(expected4, v4);
    EXPECT_TRUE(eop::partitioned_at_point(begin(v4), m4, end(v4), eop::is_Even<int>())) << "Not partitioned at the returned iterator";
  }
  
  TEST(chapter_11_1_partition, test_partition_stable_with_buffer)
  {
    vector<int> buffer(8);

    vector<int> v0 { 1, 2 };
    auto m0 = eop::partition_stable_with_buffer(begin(v0), end(v0), begin(buffer), eop::is_Even<int>());
    vector<int> expected0 {1, 2};
    EXPECT_EQ(expected0, v0);

    vector<int> v1 { 2, 1 };
    auto m1 = eop::partition_stable_with_buffer(begin(v1), end(v1), begin(buffer), eop::is_Even<int>());
    vector<int> expected1 {1, 2};
    EXPECT_EQ(expected1, v1);

    vector<int> v2 { 1, 2, 3, 9, 6, 7, 4, 5};
    auto m2 = eop::partition_stable_with_buffer(begin(v2), end(v2), begin(buffer), eop::is_Even<int>());
    vector<int> expected2 {1, 3, 9, 7, 5, 2, 6, 4};
    EXPECT_EQ(expected2, v2);
    EXPECT_TRUE(eop::partitioned_at_point(begin(v2), m2, end(v2), eop::is_Even<int>())) << "Not partitioned at the returned iterator";
  }

  TEST(chapter_11_1_partition, test_partition_stable_with_buffer_n)
  {
    vector<int> buffer(8);

    vector<int> v0 { 1, 2 };
    auto m0 = eop::partition_stable_with_buffer_n(begin(v0), v0.size(), begin(buffer), eop::is_Even<int>());
    vector<int> expected0 {1, 2};
    EXPECT_EQ(expected0, v0);

    vector<int> v1 { 2, 1 };
    auto m1 = eop::partition_stable_with_buffer_n(begin(v1), v1.size(), begin(buffer), eop::is_Even<int>());
    vector<int> expected1 {1, 2};
    EXPECT_EQ(expected1, v1);

    vector<int> v2 { 1, 2, 3, 9, 6, 7, 4, 5};
    auto m2 = eop::partition_stable_with_buffer_n(begin(v2), v2.size(), begin(buffer), eop::is_Even<int>());
    vector<int> expected2 {1, 3, 9, 7, 5, 2, 6, 4};
    EXPECT_EQ(expected2, v2);
    EXPECT_TRUE(eop::partitioned_at_point(begin(v2), m2.first, end(v2), eop::is_Even<int>())) << "Not partitioned at the returned iterator";
  }

  TEST(chapter_11_1_partition, test_partition_stable_singleton)
  {
    vector<int> even{14};
    auto m0 = eop::partition_stable_singleton(begin(even), eop::is_Even<int>());
    EXPECT_EQ(begin(even), m0.first); 
    EXPECT_EQ(end(even), m0.second);

    vector<int> odd{7};
    auto m1 = eop::partition_stable_singleton(begin(odd), eop::is_Even<int>());
    EXPECT_EQ(end(odd), m1.first); 
    EXPECT_EQ(end(odd), m1.second);
  }

  TEST(chapter_11_1_partition, test_partition_stable_n_nonempty)
  {
    vector<int> v0{1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto m0 = eop::partition_stable_n_nonempty(begin(v0), v0.size(), eop::is_Even<int>());
    vector<int> e0{1, 3, 5, 7, 9, 2, 4, 6, 8};
    EXPECT_EQ(e0, v0);
    EXPECT_TRUE(eop::partitioned_at_point(begin(v0), m0.first, end(v0), eop::is_Even<int>())) << "Not partitioned at the returned iterator";

    vector<int> v1{2, 4, 6, 8, 1, 3, 5, 7, 9};
    auto m1 = eop::partition_stable_n_nonempty(begin(v1), v1.size(), eop::is_Even<int>());
    vector<int> e1{1, 3, 5, 7, 9, 2, 4, 6, 8};
    EXPECT_EQ(e1, v1);
    EXPECT_TRUE(eop::partitioned_at_point(begin(v1), m1.first, end(v1), eop::is_Even<int>())) << "Not partitioned at the returned iterator";

    vector<int> v2{2, 1};
    auto m2 = eop::partition_stable_n_nonempty(begin(v2), v2.size(), eop::is_Even<int>());
    vector<int> e2{1, 2};
    EXPECT_EQ(e2, v2);
    EXPECT_TRUE(eop::partitioned_at_point(begin(v2), m2.first, end(v2), eop::is_Even<int>())) << "Not partitioned at the returned iterator"; 

    vector<int> v3;
    auto m3 = eop::partition_stable_n(begin(v3), v3.size(), eop::is_Even<int>());
    ASSERT_TRUE(v3.empty());
  }

  TEST(chapter_11_1_partition, test_partition_stable_n_adaptive)
  {
    vector<int> buffer(8);

    vector<int> v0 { 1, 2 };
    auto m0 = eop::partition_stable_n_adaptive(begin(v0), v0.size(), begin(buffer), buffer.size(), eop::is_Even<int>());
    vector<int> expected0 {1, 2};
    EXPECT_EQ(expected0, v0);

    vector<int> v1 { 2, 1 };
    auto m1 = eop::partition_stable_n_adaptive(begin(v1), v1.size(), begin(buffer), buffer.size(), eop::is_Even<int>());
    vector<int> expected1 {1, 2};
    EXPECT_EQ(expected1, v1);

    vector<int> v2 { 1, 2, 3, 9, 6, 7, 4, 5};
    auto m2 = eop::partition_stable_n_adaptive(begin(v2), v2.size(), begin(buffer), buffer.size(), eop::is_Even<int>());
    vector<int> expected2 {1, 3, 9, 7, 5, 2, 6, 4};
    EXPECT_EQ(expected2, v2);
    EXPECT_TRUE(eop::partitioned_at_point(begin(v2), m2.first, end(v2), eop::is_Even<int>())) << "Not partitioned at the returned iterator";

    vector<int> v3 { 1, 2, 3, 9, 6, 7, 4, 5, 10, 14, 11, 13, 15, 12, 8};
    auto m3 = eop::partition_stable_n_adaptive(begin(v3), v3.size(), begin(buffer), buffer.size(), eop::is_Even<int>());
    vector<int> expected3 {1, 3, 9, 7, 5, 11, 13, 15, 2, 6, 4, 10, 14, 12, 8};
    EXPECT_EQ(expected3, v3);
    EXPECT_TRUE(eop::partitioned_at_point(begin(v3), m3.first, end(v3), eop::is_Even<int>())) << "Not partitioned at the returned iterator";
  }

  template<typename P, typename T1>
  struct compare_first {
    typedef Domain(P) T0;
    P pred;
    typedef pair<T0, T1> first_argument_type;
    typedef bool result_type;
    typedef pair<T0, T1> input_type;
    compare_first(P pred) : pred(pred) {}
    bool operator()(pair<T0, T1> x) {
      return pred(x.first);
    }
  };

  TEST(chapter_11_1_partition, test_partition_stable_iterative)
  {
    vector<int> v0 { 1, 2 };
    auto m0 = eop::partition_stable_iterative(begin(v0), end(v0), eop::is_Even<int>());
    vector<int> expected0 {1, 2};
    EXPECT_EQ(expected0, v0);

    vector<int> v1 { 2, 1 };
    auto m1 = eop::partition_stable_iterative(begin(v1), end(v1), eop::is_Even<int>());
    vector<int> expected1 {1, 2};
    EXPECT_EQ(expected1, v1);

    vector<int> v2 { 1, 2, 3, 9, 6, 7, 4, 5};
    auto m2 = eop::partition_stable_iterative(begin(v2), end(v2), eop::is_Even<int>());
    vector<int> expected2 {1, 3, 9, 7, 5, 2, 6, 4};
    EXPECT_EQ(expected2, v2);
    EXPECT_TRUE(eop::partitioned_at_point(begin(v2), m2, end(v2), eop::is_Even<int>())) << "Not partitioned at the returned iterator";

    vector<int> v3 { 1, 2, 3, 9, 6, 7, 4, 5, 10, 14, 11, 13, 15, 12, 8};
    auto m3 = eop::partition_stable_iterative(begin(v3), end(v3), eop::is_Even<int>());
    vector<int> expected3 {1, 3, 9, 7, 5, 11, 13, 15, 2, 6, 4, 10, 14, 12, 8};
    EXPECT_EQ(expected3, v3);
    EXPECT_TRUE(eop::partitioned_at_point(begin(v3), m3, end(v3), eop::is_Even<int>())) << "Not partitioned at the returned iterator";
  }

  template<typename T>
  struct is_First_Even
  {
    typedef pair<T, T> first_argument_type;
    typedef bool result_type;
    typedef pair<T, T> input_type;
    bool operator()(pair<T, T> value) {
      return eop::even(value.first);
    }
  };

  TEST(chapter_11_1_partition, test_partition_stable_iterative_stability)
  {
    using type = pair<int, int>;
    vector<type> v0 { {1, 2}, {2, 1}, {1, 1}, {2, 2} };
    auto m0 = eop::partition_stable_iterative(begin(v0), end(v0), is_First_Even<int>());
    vector<type> expected0 {{1, 2}, {1, 1}, {2, 1}, {2, 2}};
    EXPECT_EQ(expected0, v0);
  }
}
