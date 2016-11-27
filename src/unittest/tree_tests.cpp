
#include "gtest/gtest.h"
#include "intrinsics.h"
#include "tree.h"
#include "type_functions.h"

namespace eoptest {
	TEST(stree_tests, empty_stree_coordinate)
	{
		eop::stree_coordinate<int> c{ 0 };
		EXPECT_TRUE(eop::empty(c));
	}

	TEST(tree_tests, empty_coordinate_tree)
	{
		eop::tree_coordinate<int> c{ 0 };
		EXPECT_TRUE(eop::empty(c));
	}
} // namespace eoptest
