#include "binary_counter.h"
#include "list_pool.h"

template<typename I>
struct iterator_and_list
{
  typedef I value_type;
  typedef typename list_pool<value_type>::list_type list_type;

  value_type value;
  list_type list;
  iterator_and_list(value_type value, list_type list = list_type())
    : value(value), list(list) {}
  iterator_and_list operator=(iterator_and_list const& x) {
    value = x.value;
    list = x.list;
    return *this;
  }
  
  value_type operator*() {
    return value;
  }
};

template<typename I>
bool operator==(iterator_and_list<I> const& l, iterator_and_list<I> const& r) {
  return l.value == r.value;
}

template<typename I>
bool operator!=(iterator_and_list<I> const& l, iterator_and_list<I> const& r) {
  return l.value != r.value;
}

template<typename I>
iterator_and_list<I> operator++(iterator_and_list<I> const& i) {
  return iterator_and_list<I>(++i.value, i.list);
}

template<typename T, typename I>
I merge(list_pool<T>& pool, I x, I y) {
  x.list = pool.allocate(y.value, x.list);
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
  typedef iterator_and_list<I> IL;
  typedef compare_source<Compare> CompareSource;

  list_pool<I> pool;
  binary_counter<MinTwoOp<I, CompareSource>, IL> min_counter(MinTwoOp<I, CompareSource>(CompareSource(cmp), pool), IL(l));
  while(f != l) min_counter.add(IL(f++));
  IL result = min_counter.reduce();
  return make_pair(result.value, pool.value(min_element(pool, result.list, compare_source<Compare>(cmp))));
}

