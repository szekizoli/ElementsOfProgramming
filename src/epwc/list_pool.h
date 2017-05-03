//
// Lecture 7
//

#pragma once

#include<vector>
#include<cstddef>

template<typename T, typename N = typename std::size_t>
  //Requires: SemiRegular(T) && 
class list_pool
{
 public:
  typedef N list_type;
 private:
  struct node_t {
    T value;
    list_type next;
  };

  std::vector<node_t> pool;
  list_type free_list;

  node_t& node(list_type x) {
    return pool[x-1];
  }

  node_t const& node(list_type x) const {
    return pool[x-1];
  }

  list_type new_list() {
    pool.push_back(node_t());
    return pool.size();
  }

 public:
  
  list_type empty() const {
    return list_type();
  }
  
  bool is_empty(list_type x) const {
   return x == empty();
  }

  list_pool() : free_list(empty()) {
  }
  
  T& value(list_type x) {
    return node(x).value;
  }

  T const& value(list_type x) const {
    return node(x).value;    
  }

  list_type& next(list_type x) {
    return node(x).next;
  }

  list_type const& next(list_type x) const {
    return node(x).next;
  }

  list_type free(list_type x) {
    list_type cdr = next(x);
    next(x) = free_list;
    return cdr;
  }
 
  list_type allocate(const T& x, list_type tail = list_type()) {
    list_type list = free_list;
    if (is_empty(free_list)) {
      list = new_list();
    } else {
      free_list = next(free_list);
    }
    value(list) = x;
    next(list) = tail;
    return list;
  }
};

template<typename T, typename N>
void free_list(list_pool<T, N>& pool,
	       typename list_pool<T, N>::list_type x)
{
  while(!pool.is_empty(x)) x = pool.free(x);
}

template<typename T, typename N, typename Cmp>
typename list_pool<T, N>::list_type
min_element(list_pool<T, N> const& pool,
		   typename list_pool<T, N>::list_type list,
		   Cmp cmp) {
  if (pool.is_empty(list)) return list;
  typename list_pool<T, N>::list_type current_min = list;
  while(!pool.is_empty(list)) {
    if (!cmp(pool.value(current_min), pool.value(list))) current_min = list;
    list = pool.next(list);    
  }
  return current_min;
}
		 
