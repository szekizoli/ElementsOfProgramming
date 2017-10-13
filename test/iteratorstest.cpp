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

namespace eoptest {

  using std::vector;
  using namespace std;  

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
} // namespace eoptest
