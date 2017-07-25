#include "binary_counter.h"
#include "list_pool.h"
#include "min_element_1_2.h"

#include <iostream>

template<typename I>
void print(I f, I l) {
  if (f == l) cout << "N/A";
  else        cout << *f;
}

void min_1_2(std::vector<int> const& v) {
  auto min_two = min_two_element_binary(begin(v), end(v), std::less<int>());
  print(min_two.first, end(v));
  cout << ", ";
  if (min_two.second != end(v)) {
    cout << min_two.second - begin(v) << " ";
  }
  print(min_two.second, end(v));
  cout << std::endl;
}

/*int main() {
  typedef list_pool<int>::list_type list_type;
  list_pool<int> pool;
  list_type l;
  for (int i : {3, 7, 2, 4, 6, 1, 8, 5}) {
   l = pool.allocate(i, l);
  }
  list_type min = min_element(pool, l, std::less<int>());
  std::cout << pool.value(min) << std::endl;
  min_1_2(std::vector<int> {3, 7, 4, 6, 1, 2, 9, 2, 8, 5} );
  min_1_2(std::vector<int> {1} );
  min_1_2(std::vector<int> {} );
}*/
