// tree.h (originally in eop.h)

// Copyright (c) 2009 Alexander Stepanov and Paul McJones
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without
// fee, provided that the above copyright notice appear in all copies
// and that both that copyright notice and this permission notice
// appear in supporting documentation. The authors make no
// representations about the suitability of this software for any
// purpose. It is provided "as is" without express or implied
// warranty.

// The origin of the contents of this file is
// http://www.elementsofprogramming.com/code/eop.h

#pragma once

#include <initializer_list>

#include "eop.h"
#include "intrinsics.h"
#include "pointers.h"

namespace eop {
  template<typename T>
    requires(Regular(T))
  struct slist_node 
  {
    typedef T value_type;
    typedef pointer(slist_node<T>) Link;
    T value;
    Link forward_link;
    slist_node() : forward_link(0) {}
    slist_node(T value, Link s = 0) : value(value), forward_link(s) {}
    slist_node(const slist_node& other) = default;
  };

  template<typename T>
    requires(Regular(T))
  struct value_type<slist_node<T>>
  {
    typedef T type;
  };

  template<typename T>
    requires(Regular(T))
  struct slist_iterator
  {
    typedef T value_type;
    typedef int weight_type;
    pointer(slist_node<T>) ptr;
    slist_iterator(pointer(slist_node<T>) ptr = 0) : ptr(ptr) {}
    slist_iterator(slist_iterator<T> const& x) : ptr(x.ptr) {}
    slist_iterator(slist_iterator<T> && x) : ptr(x.ptr) 
    {
      x.ptr = 0;
    }
    slist_iterator<T>& operator=(slist_iterator<T> const& x)
    {
      ptr = x.ptr;
      return *this;
    }
    slist_iterator<T>& operator=(slist_iterator<T> && x)
    {
      ptr = x.ptr;
      x.ptr = 0;
      return *this;
    }
  };

  template<typename T>
    requires(Regular(T))
  struct weight_type<slist_iterator<T>>
  {
    typedef int type;
  };

  template<typename T>
    requires(Regular(T))
  struct value_type<slist_iterator<T>>
  {
    typedef T type;
  };

  template<typename T>
    requires(Regular(T))
  struct distance_type<slist_iterator<T>>
  {
    typedef int type;
  };

  template<typename T>
    requires(Regular(T))
  bool empty(slist_iterator<T> t)
  {
    typedef pointer(slist_node<T>) I;
    return t.ptr == I{ 0 };
  }

  template<typename T>
    requires(Regular(T))
  slist_iterator<T> successor(slist_iterator<T> t)
  {
    return slist_iterator<T>(source(t.ptr).forward_link);
  }

  template<typename T>
    requires(Regular(T))
  bool has_successor(slist_iterator<T> t)
  {
    return !empty(successor(t));
  }
  
  template<typename T>
    requires(Regular(T))
  void set_forward_link(slist_iterator<T> c, slist_iterator<T> s)
  {
    forward_linker<slist_iterator<T>>()(c, s);
    //sink(c.ptr).forward_link = s.ptr;
  }

  template<typename T>
    requires(Regular(T))
  bool operator==(slist_iterator<T> const& a, slist_iterator<T> const& b)
  {
    return a.ptr == b.ptr;
  }

  template<typename T>
    requires(Regular(T))
  bool operator!=(slist_iterator<T> const& a, slist_iterator<T> const& b)
  {
    return a.ptr != b.ptr;
  }

  template<typename T>
    requires(Regular(T))
  T const& source(slist_iterator<T> c)
  {
    return source(c.ptr).value;
  }

  template<typename T>
    requires(Regular(T))
  T& sink(slist_iterator<T> c)
  {
    return sink(c.ptr).value;
  }

  template<typename T>
    requires(Regular(T))
  struct initializer_list_iterator
  {
    typedef typename std::initializer_list<T> List;
    typedef typename std::initializer_list<T>::iterator Iterator; 
    Iterator first;
    Iterator last;
    initializer_list_iterator(Iterator f, Iterator l) : first(f), last(l) {}
    initializer_list_iterator(List const& l) : first(begin(l)), last(end(l)) {}
  };

  template<typename T>
    requires(Regular(T))
  bool empty(initializer_list_iterator<T> const& c)
  {
    return c.first == c.last;
  }

  template<typename T>
    requires(Regular(T))
  T source(initializer_list_iterator<T> const& c)
  {
    return source(c.first);
  }

  template<typename T>
    requires(Regular(T))
  typename std::initializer_list<T>::iterator successor(typename std::initializer_list<T>::iterator const& i)
  {
    return i + 1;
  }

  template<typename T>
    requires(Regular(T))
  initializer_list_iterator<T> successor(initializer_list_iterator<T> const& c)
  {
    return initializer_list_iterator<T>(c.first + 1, c.last);
  }

  int& slist_node_count();

  template<typename T>
    requires(Regular(T))
  struct slist_node_construct
  {
    typedef initializer_list_iterator<T> ILI;
    typedef slist_iterator<T> I;
    slist_node_construct() {}
    I operator()(T x, I s = I(0)) const
    {
      ++slist_node_count();
      return I(new slist_node<T>(x, s.ptr));
    }
    I operator()(ILI i)    const { return (*this)(source(i)); }
    I operator()(I i)      const { return (*this)(source(i)); }
    I operator()(I i, I s) const { return (*this)(source(i), s); }
  };

  template<typename T>
    requires(Regular(T))
  slist_iterator<T> erase_first(slist_iterator<T> i)
  {
    slist_iterator<T> n = successor(i);
    --slist_node_count();
    delete i.ptr;
    return n;
  }

  template<typename T>
    requires(Regular(T))
  void erase_after(slist_iterator<T> i)
  {
    set_forward_link(i, erase_first(successor(i)));
  }

  template<typename T>
  void erase_all(slist_iterator<T> i)
  {
    while(!empty(i)) i = erase_first(i);
  }

  template<typename C, typename C1, typename Cons>
    requires(Regular(T) && ListNodeConstructor(Cons))
  C list_copy(C1 c)
  {
    if (empty(c)) return C(0);
    Cons construct_node;
    C f = construct_node(c);
    C l = f;
    c = successor(c);
    while (!empty(c))
    {
      set_forward_link(l, construct_node(c));
      l = successor(l);
      c = successor(c);
    }
    return f;
  }

  // singly-linked list
  template<typename T>
    requires(Regular(T))
  struct slist
  {
    using I = slist_iterator<T>;
    using Cons = slist_node_construct<T>;
    using ILI =  initializer_list_iterator<T>;
    I root;
    // default constructor
    slist() : root(0) {}

    // from value constructor
    slist(T x) : root(Cons()(x)) {}

    // copy constructor
    slist(const slist& x) : root(list_copy<I, I, Cons>(x.root)) {}

    // list-initialization
    slist(std::initializer_list<T> const& l) : root(list_copy<I, ILI, Cons>(ILI(l))) {}

    // move constructor
    slist(slist&& x) : root(x.root) 
    {
      x.root = 0;
    }

    // append to head
    slist(T x, const slist& l) : root(Cons()(x)) 
    {
      set_forward_link(root, list_copy<I, I, Cons>(l.root));
    }

    // desctructor
    ~slist()
    {
      erase_all(root);
    }
  };

  template<typename T>
    requires(Regular(T))
  struct iterator_type<slist<T>>
  {
    typedef slist_iterator<T> type;
  };

  template<typename T>
    requires(Regular(T))
  slist_iterator<T> begin(slist<T> const& x) { return slist_iterator<T>(x.root); }

  template<typename T>
    requires(Regular(T))
  slist_iterator<T> end(slist<T> const& x)  { return slist_iterator<T>(); }

  // double-linked list
  template<typename T>
    requires(Regular(T))
  struct list_node
  {
    typedef T value_type;
    typedef pointer(T) Link;
    T value;
    Link forward_link;
    Link backward_link;
    // default constructor
    list_node() : forward_link(0), backward_link(0) {}
    list_node(T x, Link s_link = 0, Link p_link = 0) : value(x),
      forward_link(s_link), backward_link(p_link) {}
    // copy constructor
    list_node(list_node const& x) = default;
  };

  template<typename T>
    requires(Regular(T))
  struct value_type<list_node<T>>
  {
    typedef T type;
  };

  int& list_node_count();

  template<typename T>
    requires(Regular(T))
  struct list_iterator
  {
    typedef T value_type;
    typedef int distance_type;
    pointer(list_node<T>) ptr;
    // Default constructor
    list_iterator(pointer(list_node<T>) ptr = 0) : ptr(ptr) {}
    // Copy constructor
    list_iterator(list_iterator<T> const& o) : ptr(o.ptr) {}
    // Move constructor
    list_iterator(list_iterator<T> && o) : ptr(o.ptr) 
    {
      o.ptr = 0;
    }
    // Copy assignment
    list_iterator<T>& operator=(list_iterator<T> const& o)
    {
      ptr = o.ptr;
      return *this;
    }
    // Move assignment
    list_iterator<T>& operator=(list_iterator<T> && o)
    {
      ptr = o.ptr;
      o.ptr = 0;
      return *this;
    }
  };

  // Equality
  template<typename T>
    requires(Regular(T))
  bool operator==(list_iterator<T> const& l, list_iterator<T> const& r)
  {
    return l.ptr == r.ptr;
  }

  // Inequality
  template<typename T>
    requires(Regular(T))
  bool operator!=(list_iterator<T> const& l, list_iterator<T> const& r)
  {
    return !(l == r);
  }

  template<typename T>
    requires(Regular(T))
  struct iterator_concept<list_iterator<T>>
  {
    typedef bidirectional_iterator_tag concept;
  };

  template<typename T>
    requires(Regular(T))
  struct value_type<list_iterator<T>>
  {
    typedef T type;
  };

  template<typename T>
    requires(Regular(T))
  struct distance_type<list_iterator<T>>
  {
    typedef T type;
  };

  template<typename T>
    requires(Regular(T))
  bool empty(list_iterator<T> const& i)
  {
    typedef pointer(slist_node<T>) I;
    return i.ptr == I{0};
  }

  template<typename T>
    requires(Regular(T))
  list_iterator<T> successor(list_iterator<T> t)
  {
    return list_iterator<T>(source(t.ptr).forward_link);
  }

  template<typename T>
    requires(Regular(T))
  bool has_successor(list_iterator<T> t)
  {
    return !empty(successor(t));
  }
  
  template<typename T>
    requires(Regular(T))
  void set_link_forward(list_iterator<T> c, list_iterator<T> s)
  {
    forward_linker<list_iterator<T>>()(c, s);
  }

  template<typename T>
    requires(Regular(T))
  list_iterator<T> predecessor(list_iterator<T> t)
  {
    return list_iterator<T>(source(t.ptr).backward_link);
  }

  template<typename T>
    requires(Regular(T))
  bool has_predecessor(list_iterator<T> t)
  {
    return !empty(predecessor(t));
  }
  
  template<typename T>
    requires(Regular(T))
  void set_link_backward(list_iterator<T> c, list_iterator<T> s)
  {
    backward_linker<list_iterator<T>>()(c, s);
  }

  template<typename T>
    requires(Regular(T))
  void set_link_bidirectional(list_iterator<T> c, list_iterator<T> s)
  {
    bidirectional_linker<list_iterator<T>>()(c, s);
  }

  template<typename T>
    requires(Regular(T))
  const T& source(list_iterator<T> i)
  {
    return source(i.ptr).value;
  }

  template<typename T>
    requires(Regular(T))
  T& sink(list_iterator<T> i)
  {
    return sink(i.ptr).value;
  }

  template<typename T>
    requires(Regular(T))
  struct less<list_iterator<T>>
  {
    bool operator()(list_iterator<T> i,
            list_iterator<T> j)
    {
      return i.ptr < j.ptr;   
    }
  };

  template<typename T>
    requires(Regular(T))
  void erase(list_iterator<T> i)
  {
    bidirectional_linker<list_iterator<T>>(predecessor(i), successor(i));
    delete i.ptr;
    --list_node_count();
  }

  // template<typename I>
  // list_iterator<T> construct(T value, list_iterator<T> successor = {}, list_iterator<T> predecessor= {})
  // {
  //      return successor;
  // }


  template<typename T>
    requires(Regular(T))
  struct list
  {
    list_iterator<T> root;

    // Default constructor
    list(list_iterator<T> r = list_iterator<T>()) : root(r) {}
  };

} // namespace eop
