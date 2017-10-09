#include <array>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "coordinate_iterator_adpater.h"
#include "eop.h"
#include "testutils.h"
#include "intrinsics.h"
#include "list.h"
#include "pointers.h"
#include "tree.h"
#include "type_functions.h"

#include "project_7_1.h"

namespace eoptest {
  using std::array;
  using std::list;
  using std::vector;
  using std::pair;
  using std::swap;
  using std::back_inserter;

  using namespace eop;

  STree create_stree_5();

  Tree create_tree();
  
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
    EXPECT_EQ(0, eop::stree_node_count()) << "Before construct test EOP";
    {
      STree x = create_stree_5();
      EXPECT_EQ(5, eop::weight_recursive(begin(x)));
    }
    EXPECT_EQ(0, eop::stree_node_count()) << "Before construct test EOP";
  }

  TEST(coordinatestest, test_weight_recursive_tree)
  {
    EXPECT_EQ(0, eop::tree_node_count()) << "Before construct test";
    {
      Tree x = create_tree();
      EXPECT_EQ(5, eop::weight_recursive(begin(x)));
    }
    EXPECT_EQ(0, eop::tree_node_count()) << "After construct test";
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
    EXPECT_EQ(0, eop::stree_node_count()) << "Before construct test";
    {
      STree x = create_stree_5();
      EXPECT_EQ(3, eop::height_recursive(begin(x)));
    }
    EXPECT_EQ(0, eop::stree_node_count()) << "After construct test";
  }

  TEST(coordinatestest, test_height_recursive_tree)
  {
    EXPECT_EQ(0, eop::tree_node_count()) << "Before construct test";
    {
      Tree x = create_tree();
      EXPECT_EQ(3, eop::height_recursive(begin(x)));
    }
    EXPECT_EQ(0, eop::tree_node_count()) << "After construct test";
  }

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
} // namespace eoptest
