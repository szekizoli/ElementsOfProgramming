#include "binary_counter.h"
#include "list_pool.h"

template<typename I, typename N>
  std::pair<I, typename list_pool<I, N>::list_type>
  combine(list_pool<I, N>& pool,
	  const std::pair<I, typename list_pool<I, N>::list_type>& x,
	  const std::pair<I, typename list_pool<I, N>::list_type>& y) {
  free_list(pool, y.second);
  return make_pair(x.first, pool.allocate(y.first, x.second));
}

// Compares two iterators and returns the one
//   pointing to the smaller element
template<typename T, typename N,  typename Compare>
// R is relation
class op_min_1_2
{
private:
  Compare cmp;
  list_pool<T, N>* pool;
public:
  // Constructor.
  op_min_1_2(Compare const& cmp, list_pool<T, N>* pool) : cmp(cmp), pool(pool) {}

  template<typename I>
  I operator()(I & x, I & y) {
    return cmp(y.first, x.first) ?
      combine(*pool, y, x) : combine(*pool, x, y);
  }
};

template<typename Compare>
class compare_source {
private:
  Compare cmp;
public:
  compare_source(Compare cmp) : cmp(cmp) {}

  template<typename I>
  bool operator()(I x, I y) {
    return cmp(*x, *y);
  }  
};

template<typename I, typename Compare>
std::pair<I, I> min_two_element_binary(I f, I l, Compare cmp) {
  typedef typename list_pool<I>::list_type list_type;
  typedef std::pair<I, list_type> P;
  typedef compare_source<Compare> compare_source_type;
  typedef op_min_1_2<I, std::size_t, compare_source_type> counter_operation_type;
  typedef binary_counter<counter_operation_type, P> counter_type;
  
  list_pool<I, std::size_t> pool;
  compare_source_type cmp_source(cmp);
  counter_operation_type op(cmp_source, &pool);
  counter_type counter(op, P(l, pool.empty()));

  while(f != l) counter.add(P(f++, pool.empty()));
  P result = counter.reduce();
  I r2 = pool.is_empty(result.second) ? l : pool.value(min_element(pool, result.second, cmp_source));
  return make_pair(result.first, r2);
}

