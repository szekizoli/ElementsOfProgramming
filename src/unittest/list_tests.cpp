
#include "gtest/gtest.h"
#include "intrinsics.h"
#include "list.h"

namespace eoptest {
	TEST(slist_tests, empty_slist_coordinate)
	{
		eop::slist_coordinate<int> c{ 0 };
		EXPECT_TRUE(eop::empty(c));
	}

	TEST(slist_tests, test_non_empty_slist_coordinate)
	{
		eop::slist_node<int> n {1};
		eop::slist_coordinate<int> c{ addressof(n) };
		EXPECT_FALSE(eop::empty(c));
		EXPECT_FALSE(eop::has_successor(c));
	}

	TEST(slist_tests, test_set_successor)
	{
		eop::slist_node<int> n0{ 1 };
		eop::slist_coordinate<int> c0{ addressof(n0) };
		EXPECT_FALSE(eop::empty(c0));
		EXPECT_FALSE(eop::has_successor(c0));

		eop::slist_node<int> n1{ 2 };
		eop::slist_coordinate<int> c1{ addressof(n1) };
		EXPECT_FALSE(eop::empty(c1));
		EXPECT_FALSE(eop::has_successor(c1));

		set_successor(c0, c1);
		EXPECT_TRUE(eop::has_successor(c0));
		EXPECT_FALSE(eop::has_successor(c1));
		EXPECT_EQ(c1, successor(c0));
	}

	TEST(slist_tests, test_source)
	{
		eop::slist_node<int> n0{ 1 };
		eop::slist_coordinate<int> c0{ addressof(n0) };
		EXPECT_EQ(n0.value, source(c0));
	}

	TEST(slist_tests, test_list_copy)
	{
		typedef eop::slist_coordinate<int> C;
		typedef eop::slist_node_construct<int> Cons;
		typedef eop::slist_node_destroy<int> ND;
		eop::slist_node<int> n0{ 1 };
		eop::slist_coordinate<int> c0{ addressof(n0) };
		eop::slist_node<int> n1{ 2 };
		eop::slist_coordinate<int> c1{ addressof(n1) };
		eop::set_successor(c0, c1);

		C cc = eop::list_copy<C, Cons>(c0);
		EXPECT_FALSE(eop::empty(cc));
		EXPECT_TRUE(eop::has_successor(cc));
		EXPECT_EQ(2, eop::slist_node_count);

		eop::list_erase(cc, ND());
		EXPECT_EQ(0, eop::slist_node_count);
	}

	TEST(slist_tests, test_list_copy_empty)
	{
		typedef eop::slist_coordinate<int> C;
		typedef eop::slist_node_construct<int> Cons;
		typedef eop::slist_node_destroy<int> ND;
		C c(0);
		C r = eop::list_copy<C, Cons>(c);
		EXPECT_TRUE(eop::empty(r));
		EXPECT_EQ(0, eop::slist_node_count);
		eop::list_erase(r, ND());
		EXPECT_EQ(0, eop::slist_node_count);
	}

} // namespace eoptest
