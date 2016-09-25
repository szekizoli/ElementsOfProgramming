
#include "gtest/gtest.h"
#include "intrinsics.h"
#include "tree.h"

namespace eoptest {

	typedef eop::stree_node<int> SNode;
	typedef eop::stree_coordinate<int> SCoordinate;

	TEST(tree_tests, empty_coordinate)
	{
		SCoordinate c{ 0 };
		EXPECT_TRUE(eop::empty(c));
	}

	TEST(tree_tests, node_construct)
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

	TEST(tree_tests, source)
	{
		SNode n0{ 42 };
		SCoordinate c0{ addressof(n0) };
		EXPECT_EQ(42, eop::source(c0));
	}

} // namespace eoptest
