
#include <utility>
#include "gtest/gtest.h"
#include "intrinsics.h"
#include "list.h"

#include "testutils.h"

namespace eoptest {
	TEST(slist_tests, empty_slist_iterator)
	{
		eop::slist_iterator<int> i{ 0 };
		EXPECT_TRUE(eop::empty(i));
	}

	TEST(slist_tests, test_non_empty_slist_iterator)
	{
		eop::slist_node<int> n {1};
		eop::slist_iterator<int> i{ addressof(n) };
		EXPECT_FALSE(eop::empty(i));
		EXPECT_FALSE(eop::has_successor(i));
	}

	TEST(slist_tests, test_set_successor)
	{
		eop::slist_node<int> n0{ 1 };
		eop::slist_iterator<int> i0{ addressof(n0) };
		EXPECT_FALSE(eop::empty(i0));
		EXPECT_FALSE(eop::has_successor(i0));

		eop::slist_node<int> n1{ 2 };
		eop::slist_iterator<int> i1{ addressof(n1) };
		EXPECT_FALSE(eop::empty(i1));
		EXPECT_FALSE(eop::has_successor(i1));

		set_successor(i0, i1);
		EXPECT_TRUE(eop::has_successor(i0));
		EXPECT_FALSE(eop::has_successor(i1));
		EXPECT_EQ(i1, successor(i0));
	}

	TEST(slist_tests, test_source)
	{
		eop::slist_node<int> n0{ 1 };
		eop::slist_iterator<int> i{ addressof(n0) };
		EXPECT_EQ(n0.value, source(i));
	}

	TEST(slist_tests, test_list_copy)
	{
		typedef eop::slist_iterator<int> I;
		typedef eop::slist_node_construct<int> Cons;
		eop::slist_node<int> n0{ 1 };
		eop::slist_iterator<int> i0{ addressof(n0) };
		eop::slist_node<int> n1{ 2 };
		eop::slist_iterator<int> i1{ addressof(n1) };
		eop::set_successor(i0, i1);

		I i_copy = eop::list_copy<I, I, Cons>(i0);
		EXPECT_FALSE(eop::empty(i_copy));
		EXPECT_TRUE(eop::has_successor(i_copy));
		EXPECT_EQ(2, eop::slist_node_count());

		eop::erase_all(i_copy);
		EXPECT_EQ(0, eop::slist_node_count());
	}

	TEST(slist_tests, test_list_copy_empty)
	{
		typedef eop::slist_iterator<int> I;
		typedef eop::slist_node_construct<int> Cons;
		I i(0);
		I r = eop::list_copy<I, I, Cons>(i);
		EXPECT_TRUE(eop::empty(r));
		EXPECT_EQ(0, eop::slist_node_count());
		eop::erase_all(r);
		EXPECT_EQ(0, eop::slist_node_count());
	}

	TEST(slist_tests, test_slist_construct_empty)
	{
		{
			eop::slist<int> l;
			EXPECT_EQ(0, eop::slist_node_count());
			EXPECT_TRUE(eop::empty(l.root));
		}
		EXPECT_EQ(0, eop::slist_node_count());
	}

	TEST(slist_tests, test_slist_construct_from_value)
	{
		typedef eop::slist_iterator<int> C;
		{
			eop::slist<int> list (12);
			EXPECT_EQ(1, eop::slist_node_count());
			EXPECT_FALSE(eop::empty(list.root));
			EXPECT_FALSE(eop::has_successor(list.root));
			EXPECT_EQ(1, eop::slist_node_count());
		}
		EXPECT_EQ(0, eop::slist_node_count());
	}

	TEST(slist_tests, test_slist_construct_non_empty)
	{
		{
			eop::slist<int> l0 (12);
			eop::slist<int> l1 (11, l0);
			EXPECT_EQ(3, eop::slist_node_count());
			ASSERT_FALSE(eop::empty(l0.root));
			EXPECT_FALSE(eop::has_successor(l0.root));
			ASSERT_FALSE(eop::empty(l1.root));
			EXPECT_TRUE(eop::has_successor(l1.root));
			EXPECT_EQ(12, source(l0.root));
			EXPECT_EQ(11, source(l1.root));
		}
		EXPECT_EQ(0, eop::slist_node_count());
	}

	TEST(slist_tests, test_slist_construct_list_initialization)
	{
		EXPECT_EQ(0, eop::slist_node_count());
		{
			eop::slist<int> l {1,2,3,4,5};
			EXPECT_EQ(5, eop::slist_node_count());
			ASSERT_FALSE(eop::empty(l.root));
			ASSERT_TRUE(eop::has_successor(l.root));
			std::vector<int> expected {1, 2, 3, 4, 5};
			std::vector<int> actual = list_to_vector(begin(l));
			EXPECT_EQ(expected, actual);
		}
		EXPECT_EQ(0, eop::slist_node_count());
	}

	TEST(slist_tests, test_slist_move_constructor)
	{
		{
			eop::slist<int> l0 (12);
			eop::slist<int> l1 (11, l0);
			eop::slist<int> l_moved(std::move(l1));
			EXPECT_EQ(3, eop::slist_node_count());
			ASSERT_TRUE(eop::empty(l1.root));
			ASSERT_FALSE(eop::empty(l_moved.root));
			EXPECT_TRUE(eop::has_successor(l_moved.root));
			EXPECT_EQ(11, source(l_moved.root));
		}
		EXPECT_EQ(0, eop::slist_node_count());
	}

} // namespace eoptest
