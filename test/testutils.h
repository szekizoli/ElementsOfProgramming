#pragma once

#include <iostream>
#include <utility>
#include <vector>

#include "eop.h"
#include "intrinsics.h"
#include "list.h"
#include "type_functions.h"
#include "tree.h"

namespace eop {
  using std::vector;

  template<typename T>
  struct value_type<typename std::vector<T>>
  {
    typedef T type;
  };
  
  template<typename T>
  struct value_type<typename std::back_insert_iterator< std::vector<T> > >
  {
    // ValueType of the back_insert_iterator is back_insert_iterator
    typedef std::back_insert_iterator< std::vector<T> > type;
  };

  template<>
  struct iterator_concept<typename std::vector<int>::iterator> 
  {
    typedef random_access_iterator_tag concept;
  };

  template<>
  struct iterator_concept<typename std::vector<std::pair<int, int>>::iterator> 
  {
    typedef random_access_iterator_tag concept;
  };
}

// utility methods for testing
namespace eoptest {

  using std::vector;
  
  template<typename T>
  T less(T const& a, T const& b) {
    return a < b;
  }

  template<typename T>
    requires(Regular(T))
  auto size(std::vector<T> const& v) 
  {
    return v.size();
  }
  
  template<typename T>
    requires(Regular(T))
  std::vector<T> list_to_vector(eop::slist_iterator<T> i)
  {
    std::vector<T> v;
    while (!empty(i)) {
      v.push_back(source(i));
      i = successor(i);
    }
    return v;
  }

  template<typename T>
    requires(Regular(T))
  std::vector<T> list_to_vector(eop::slist_iterator<T> f, eop::slist_iterator<T> l)
  {
    std::vector<T> v;
    if (empty(f)) return v;
    while (f != l) {
      v.push_back(source(f));
      f = successor(f);
    }
    v.push_back(source(f));
    return v;
  }

  template<typename L>
  void set_root(L& list, eop::IteratorType<L> & i) 
  {
    list.root = i;
  }

  template<int N>
  std::array<eop::slist_node<int>, N> create_list(int first, int diff)
  {
    typedef eop::slist_node<int> SN;
    std::array<SN, N> a;
    a[N-1] = SN(first + (N-1)*diff);
    for (int i = N-2; !(i < 0); --i) {
      a[i] = SN(first + i*diff, addressof(a[i+1]));
    }
    return a;
  }

  template<int N, typename I>
  std::array<eop::slist_node<int>, N> create_list(I f, I l)
  {
    // Precondition: l - f == N
    typedef eop::slist_node<int> SN;
    std::array<SN, N> a;
    for (int i = 0; i < N && f != l; ++i) {
      a[i] = SN(eop::source(f));
      i = eop::successor(i);
      f = eop::successor(f);
    }
    for (int i = 0; i < N - 1 ; ++i) {
      a[i].forward_link = addressof(a[i+1]); 
    }
    return a;
  }

  template<int N>
  struct test_slist
  {
    std::array<eop::slist_node<int>, N> array;
    test_slist(int first = 0, int diff = 1) : array(create_list<N>(first, diff)) {}
    test_slist(std::initializer_list<int> const& l) : array(create_list<N>(begin(l), end(l))) {} 
    test_slist(test_slist<N> const& o) : array(o.array) {}
    test_slist(test_slist<N> && o) : array(std::move(o.array)) {}
  };

  template<int N>
  eop::slist_iterator<int> begin(test_slist<N> l)
  {
    return eop::slist_iterator<int>(addressof(l.array[0]));
  }

  template<int N>
  eop::slist_iterator<int> end(test_slist<N> l)
  {
    return eop::slist_iterator<int>();
  }

  template<typename T>
  std::ostream& operator<<(std::ostream& o, std::vector<T> const& v)
  {
    for(T t : v) o << t << " ";
    return o;
  }

  struct equals_To {
    const int value;
    equals_To(int v) : value(v) {};
    bool operator()(int x) {
      return value == x;
    }
  };

  struct sum {
    int _sum = 0;
    void operator()(int value) {
      _sum += value;
    }
  };

  struct less_Than {
    const int value;
    typedef int first_argument_type;
    typedef int input_type;
    less_Than(int v) : value(v) {};
    bool operator()(int x) {
      return x < value;
    }
  };

  // Tree testutils

  using eop::begin;

  using namespace eop;
  typedef eop::stree_node<int> SNode;
  typedef eop::stree_coordinate<int> SCoordinate;
  typedef eop::stree<int> STree;
  typedef eop::stree_node<int> Node;
  typedef eop::tree_coordinate<int> Coordinate;
  typedef eop::tree<int> Tree;
  typedef eop::slist<int> SList;

  template<typename C>
    requires(BifurcateCoordinate(C))
  struct traverse_result
  {
    vector<int> preorder;
    vector<int> inorder;
    vector<int> postorder;
    void operator()(eop::visit v, C c)
    {
      switch(v) {
        case eop::visit::pre:  preorder.push_back(source(c));  break;
        case eop::visit::in:   inorder.push_back(source(c));   break;
        case eop::visit::post: postorder.push_back(source(c)); break; 
      }
    }
  };
}
