//
// Lecture 6
//
// Binary counter
// 
// Finds the first two minimum elements in a sequence

#include <array>
#include <utility>

using namespace std;

// Compares two iterators and returns the one
//   pointing to the smaller element
template<typename Compare>
// R is relation
class MinOp
{
private:
  Compare cmp;
public:
  // Constructor.
  MinOp(Compare const& cmp) : cmp(cmp) {}

  template<typename I>
  I operator()(I const& x, I const& y) {
    return cmp(*y, *x) ? y : x;
  }
};


/*
    3      1      6      2
    ^      ^      ^      ^
  8   3  1   7  9   6  2   5  
*/
template<typename T, typename I, typename Op>
// requires BinaryOperation(Op) && T == Domain(Op)
// requires Op is associative
// requires I is ForwardIterator and ValueType(I) == T
T add_to_counter(I f, I l, Op op, T const& zero, T carry)
{
  // precondition carry != zero
  while (f != l) {
    if (*f == zero) {
      *f = carry;
      return zero;
    }
    carry = op(*f, carry);
    *f = zero;
    ++f;
  }
  return carry;
}

template<typename T, typename I, typename Op>
// requires BinaryOperation(Op) && T == Domain(Op)
// requires Op is associative
// requires I is ForwardIterator and ValueType(I) == T
T reduce_counter(I f, I l, Op op, T const& zero)
{
  // find first non-zero value
  while (f != l && *f == zero) ++f;
  if (f == l) return zero;
  T result = *f;
  while (++f != l)
    if (*f != zero)
      result = op(*f, result);
  return result;
}

// Class implementation. State:
// - vector for the counter
// - zero
// - op
template<typename T, typename Op>
class binary_counter
{
  // Members.
private:
  std::vector<T> counter;
  T zero;
  Op op;

public:
  // Constructor.
  binary_counter(Op op, T zero) : op(op), zero(zero) {
    counter.reserve(32);
  }

  // add
  // returns: nothing
  void add(T x) {
    x = add_to_counter(begin(counter), end(counter), op, zero, x);
    if (x != zero) counter.push_back(x);
  }

  // reduce
  // returns: value of the counter
  T reduce() {
    return reduce_counter(begin(counter), end(counter), op, zero);
  }
};

template<typename I, typename Compare>
I min_element_binary(I f, I l, Compare cmp) {
  binary_counter<I, MinOp<Compare>> min_counter(MinOp<Compare>(cmp), l);
  while(f != l) min_counter.add(f++);
  return min_counter.reduce();
}
