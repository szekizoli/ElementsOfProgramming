#include "binary_counter.h"
#include "list_pool.h"

template<typename I>
struct iterator_with_list
{
  I i;
  typedef typename I::value_type value_type;
  typedef typename list_pool<value_type>::list_type list_type;
  list_type list;
  iterator_with_list(I i, list_type list = list_type())
    : i(i), list(list) {}
  iterator_with_list operator=(iterator_with_list const& x) {
    i = x.i;
    list = x.list;
    return *this;
  }
  
  value_type operator*() {
    return *i;
  }
};

template<typename I>
bool operator==(iterator_with_list<I> const& l, iterator_with_list<I> const& r) {
  return l.i == r.i;
}

template<typename I>
bool operator!=(iterator_with_list<I> const& l, iterator_with_list<I> const& r) {
  return l.i != r.i;
}

template<typename I>
iterator_with_list<I> operator++(iterator_with_list<I> const& i) {
  return iterator_with_list<I>(++i.i, i.list);
}

template<typename I>
I merge(list_pool<typename I::value_type>& pool, I x, I y) {
  x.list = pool.allocate(*y, x.list);
  free_list(pool, y.list);
  return x;
}

// Compares two iterators and returns the one
//   pointing to the smaller element
template<typename T, typename Compare>
// R is relation
class MinTwoOp
{
private:
  Compare cmp;
  list_pool<T>& pool;
public:
  // Constructor.
   MinTwoOp(Compare const& cmp, list_pool<T>& pool) : cmp(cmp), pool(pool) {}

  template<typename I>
  I operator()(I & x, I & y) {
    return cmp(*y, *x) ?
      merge(pool, y, x) : merge(pool, x, y);
  }
};

template<typename I, typename Compare>
std::pair<I, typename I::value_type> min_two_element_binary(I f, I l, Compare cmp) {
  typedef iterator_with_list<I> IL;
  typedef typename I::value_type T;
  list_pool<T> pool;
  binary_counter<MinTwoOp<T, Compare>, IL> min_counter(MinTwoOp<T, Compare>(cmp, pool), IL(l));
  while(f != l) min_counter.add(IL(f++));
  IL result = min_counter.reduce();
  return make_pair(result.i, min_element(pool, result.list, cmp));
}

