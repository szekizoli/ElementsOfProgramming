#include "tree.h"

namespace eop {
  int& stree_node_count() {
    static int stree_node_count = 0; /* ***** TESTING ***** */
    return stree_node_count;
  }

  int& tree_node_count() {
    static int tree_node_count = 0; /* ***** TESTING ***** */
    return tree_node_count;
  }
}
