#include "binary_counter.h"
#include "list_pool.h"
#include "min_element_1_2.h"

#include <iostream>

int main() {
  typedef list_pool<int>::list_type list_type;
  list_pool<int> pool;
  list_type l;
  for (int i : {3, 7, 2, 4, 6, 1, 8, 5}) {
   l = pool.allocate(i, l);
  }
  list_type min = min_element(pool, l, std::less<int>());
  std::cout << pool.value(min) << std::endl;
  std::vector<int> v {3, 7, 2, 4, 6, 1, 8, 5};
  auto min_two = min_two_element_binary(begin(v), end(v), std::less<int>());
  std::cout << *min_two.first << ", " << *min_two.second << std::endl;
}
