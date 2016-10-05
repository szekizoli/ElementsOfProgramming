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

#include "intrinsics.h"
#include "pointers.h"
#include "type_functions.h"

namespace eop {

	template<typename T>
		requires(Regular(T))
	struct stree_node
	{
		typedef typename T value_type;
		typedef pointer(stree_node<T>) Link;
		const T value;
		Link left_successor_link;
		Link right_successor_link;
		constexpr stree_node() : left_successor_link(0), right_successor_link(0) {}
		constexpr stree_node(T value, Link l = 0, Link r = 0) :
			value(value),
			left_successor_link(l), right_successor_link(r) {}
		constexpr stree_node(const stree_node& x) = default;
	};

	template<typename T>
		requires(Regular(T))
	struct stree_coordinate
	{
		typedef typename T value_type;
		typedef typename int weight_type;
		pointer(stree_node<T>) ptr;
		constexpr explicit stree_coordinate(pointer(stree_node<T>) ptr = 0) : ptr(ptr) {}
	};

	template<typename T>
		requires(Regular(T))
	struct weight_type< stree_coordinate<T> >
	{
		typedef int type;
	};

	template<typename T>
		requires(Regular(T))
	struct value_type< stree_coordinate<T> >
	{
		typedef T type;
	};

	template<typename T>
		requires(Regular(T))
	constexpr bool empty(stree_coordinate<T> t)
	{
		typedef pointer(stree_node<T>) I;
		return t.ptr == I{ 0 };
	}

	template<typename T>
		requires(Regular(T))
	constexpr stree_coordinate<T> left_successor(stree_coordinate<T> t)
	{
		return stree_coordinate<T>(source(t.ptr).left_successor_link);
	}

	template<typename T>
		requires(Regular(T))
	constexpr bool has_left_successor(stree_coordinate<T> t)
	{
		return !empty(left_successor(t));
	}

	template<typename T>
		requires(Regular(T))
	constexpr stree_coordinate<T> right_successor(stree_coordinate<T> t)
	{
		return stree_coordinate<T>(source(t.ptr).right_successor_link);
	}

	template<typename T>
		requires(Regular(T))
	constexpr bool has_right_successor(stree_coordinate<T> t)
	{
		return !empty(right_successor(t));
	}

	template<typename T>
		requires(Regular(T))
	void set_left_successor(stree_coordinate<T> c, stree_coordinate<T> l)
	{
		sink(c.ptr).left_successor_link = l.ptr;
	}

	template<typename T>
		requires(Regular(T))
	void set_right_successor(stree_coordinate<T> c, stree_coordinate<T> r)
	{
		sink(c.ptr).right_successor_link = r.ptr;
	}

	template<typename T>
		requires(Regular(T))
	constexpr bool operator==(stree_coordinate<T> a, stree_coordinate<T> b)
	{
		return a.ptr == b.ptr;
	}

	template<typename T>
		requires(Regular(T))
	constexpr const T& source(stree_coordinate<T> t)
	{
		return source(t.ptr).value;
	}

	template<typename T>
		requires(Regular(T))
	stree_node<T>& sink(pointer(stree_node<T>) t)
	{
		return *t;
	}

	template<typename T>
		requires(Regular(T))
	T& sink(stree_coordinate<T> t)
	{
		return sink(t.ptr).ptrvalue;
	}

	static int stree_node_count = 0; /* ***** TESTING ***** */

	template<typename T>
		requires(Regular(T))
	struct stree_node_construct {
		typedef typename stree_coordinate<T> C;
		stree_node_construct() {}
		C operator()(T x, C l = C(0), C r = C(0)) const
		{
			++stree_node_count;
			return C(new stree_node<T>(x, l.ptr, r.ptr));
		}
		C operator()(C c)           const { return (*this) (source(c), left_successor(c),right_successor(c)); }
		C operator()(C c, C l, C r) const { return (*this) (source(c), l, r); }
	};

	template<typename T>
		requires(Regular(T))
	struct stree_node_destroy {
		stree_node_destroy() {}
		void operator()(stree_coordinate<T> c) const
		{
			--stree_node_count;
			delete c.ptr;
		}
	};

	template<typename C, typename ND>
		requires(BifurcateCoordinate(C) && TreeNodeDeleter(ND))
	void bifurcate_erase(C c, ND node_delete)
	{
		if (empty(c)) return;
		C stack{ 0 }; // chained through left successor
		while (true) {
			C left = left_successor(c);
			C right = right_successor(c);
			if (!empty(left)) {
				if (!empty(right)) {
					set_left_successor(c, stack);
					stack = c;
				}
				else
					node_delete(c);
				c = left;
			}
			else if (!empty(right)) {
				node_delete(c);
				c = right;
			}
			else {
				node_delete(c);
				if (!empty(stack)) {
					c = stack;
					stack = left_successor(stack);
					set_left_successor(c, C{ 0 });
				}
				else return;
			}
		}
	}

	/*
	The next function is based on MAKECOPY in this paper:

	K. P. Lee.
	A linear algorithm for copying binary trees using bounded workspace.
	Commun. ACM 23, 3 (March 1980), 159-162. DOI=10.1145/358826.358835
	http://doi.acm.org/10.1145/358826.358835
	*/

	template<typename C, typename Cons>
	requires(EmptyLinkedBifurcateCoordinate(C) &&
		TreeNodeConstructor(Cons) && NodeType(C) == NodeType(Cons))
	C bifurcate_copy(C c)
	{
		Cons construct_node;
		if (empty(c)) return c;              // Us      / Lee
		C stack = construct_node(c, c, C()); // stack   / V'
		C c_new = stack;                     // c\_new  / COPY
		while (!empty(stack)) {              // empty() / null
			c = left_successor(stack);       // c       / V
			C l = left_successor(c);
			C r = right_successor(c);
			C top = stack;
			if (!empty(l)) {
				if (!empty(r)) {
					r = construct_node(r, r, right_successor(stack));
					stack = construct_node(l, l, r);
				}
				else {
					r = C();
					stack = construct_node(l, l, right_successor(stack));
				}
				l = stack;
			}
			else if (!empty(r)) {
				stack = construct_node(r, r, right_successor(stack));
				r = stack;
			}
			else
				stack = right_successor(stack);
			set_right_successor(top, r);
			set_left_successor(top, l);
		}
		return c_new;
	}

	template<typename T>
		requires(Regular(T))
	struct stree
	{
		typedef stree_coordinate<T> C;
		typedef stree_coordinate<T> coordinate_type;
		typedef stree_node_construct<T> Cons;
		C root;
		constexpr stree() : root(0) {}
		constexpr stree(T x) : root(Cons()(x)) {}
		constexpr stree(T x, const stree& left, const stree& right) : root(Cons()(x))
		{
			set_left_successor(root, bifurcate_copy<C, Cons>(left.root));
			set_right_successor(root, bifurcate_copy<C, Cons>(right.root));
		}
		stree(const stree& x) : root(bifurcate_copy<C, Cons>(x.root)) {}
		stree(stree&& x) : root(0) {
			swap(root, x.root);
		}
		~stree() { bifurcate_erase(root, stree_node_destroy<T>{}); }
		void operator=(stree x) { swap(root, x.root); }
	};

	template<typename T>
		requires(Regular(T))
	struct coordinate_type<stree<T>>
	{
		typedef stree_coordinate<T> type;
	};

	template<typename T>
		requires(Regular(T))
	struct value_type<stree<T>>
	{
		typedef T type;
	};

	template<typename T>
		requires(Regular(T))
	struct weight_type<stree<T>>
	{
		typedef int type;
	};

	template<typename T>
		requires(Regular(T))
	stree_coordinate<T> begin(const stree<T>& x) { return x.root; }

	template<typename T>
		requires(Regular(T))
	constexpr bool empty(const stree<T>& x) { return empty(x.root); }

	template<typename T>
		requires(Regular(T))
	bool operator==(const stree<T>& x, const stree<T>& y)
	{
		if (empty(x)) return empty(y);
		if (empty(y)) return false;
		return bifurcate_equivalent_nonempty(begin(x), begin(y), std::equal<T>());
	}

	template<typename T>
		requires(Regular(T))
	bool operator<(const stree<T>& x, const stree<T>& y)
	{
		if (empty(x)) return !empty(y);
		if (empty(y)) return false;
		less<T> lt;
		return bifurcate_compare_non_empty(
			begin(x), begin(y),
			comparator_3_way<less<T>>(lt)
		);
	}

	template<typename T, typename Proc>
		requires(Regular(T) && Arity(Proc) == 2
			visit == InputType(Proc, 0) && 
			CoordinateType(stree<T>) == InputType(Proc, 1))
	Proc traverse(const stree<T>& x, Proc proc)
	{
		if (empty(x)) return proc;
		return traverse_nonempty(begin(x), proc);
	}

	template<typename T>
		requires(Regular(T))
	struct tree_node
	{
		typedef typename T value_type;
		typedef pointer(tree_node<T>) Link;
		const T value;
		Link predecessor_link;
		Link left_successor_link;
		Link right_successor_link;
		constexpr tree_node() : predecessor_link(0),
			left_successor_link(0), right_successor_link(0) {}
		constexpr tree_node(T value, Link l = 0, Link r = 0, Link p = 0) :
			value(value),
			left_successor_link(l), right_successor_link(r), 
			predecessor_link(p) {}
		constexpr tree_node(const tree_node&) = default;
	};

	template<typename T>
		requires(Regular(T))
	struct tree_coordinate
	{
		typedef typename int weight_type;
		typedef typename T value_type;
		pointer(tree_node<T>) ptr;
		constexpr tree_coordinate(pointer(tree_node<T>) ptr = 0) : ptr(ptr) {}
	};

	template<typename T>
		requires(Regular(T))
	struct weight_type<tree_coordinate<T>>
	{
		typedef typename DistanceType(pointer(tree_node<T>)) type;
	};

	template<typename T>
		requires(Regular(T))
	struct value_type<tree_coordinate<T>>
	{
		typedef typename T type;
	};

	template<typename T>
		requires(Regular(T))
	constexpr bool empty(tree_coordinate<T> c)
	{
		return c.ptr == 0;
	}

	template<typename T>
		requires(Regular(T))
	constexpr bool operator==(const tree_coordinate<T>& x, const tree_coordinate<T>& y)
	{
		return x.ptr == y.ptr;
	}

	template<typename T>
		requires(Regular(T))
	constexpr bool operator!=(const tree_coordinate<T>& x, const tree_coordinate<T>& y)
	{
		return !(x.ptr == y.ptr);
	}

	template<typename T>
		requires(Regular(T))
	constexpr tree_coordinate<T> left_successor(tree_coordinate<T> c)
	{
		return source(c.ptr).left_successor_link;
	}

	template<typename T>
		requires(Regular(T))
	constexpr bool has_left_successor(tree_coordinate<T> c)
	{
		return !empty(left_successor(c));
	}

	template<typename T>
		requires(Regular(T))
	constexpr tree_coordinate<T> right_successor(tree_coordinate<T> c)
	{
		return source(c.ptr).right_successor_link;
	}

	template<typename T>
		requires(Regular(T))
	constexpr bool has_right_successor(tree_coordinate<T> c)
	{
		return !empty(right_successor(c));
	}

	template<typename T>
		requires(Regular(T))
	void set_left_successor(tree_coordinate<T> c, tree_coordinate<T> l)
	{
		sink(c.ptr).left_successor_link = l.ptr;
	}

	template<typename T>
		requires(Regular(T))
	void set_right_successor(tree_coordinate<T> c, tree_coordinate<T> r)
	{
		sink(c.ptr).right_successor_link = r.ptr;
	}

	template<typename T>
		requires(Regular(T))
	constexpr tree_coordinate<T> predecessor(tree_coordinate<T> c)
	{
		return sink(c.ptr).predecessor_link;
	}

	template<typename T>
		requires(Regular(T))
    constexpr bool has_predecessor(tree_coordinate<T> c)
	{
		return !empty(predecessor(c));
	}

	template<typename T>
		requires(Regular(T))
	void set_predecessor(tree_coordinate<T> c, tree_coordinate<T> p)
	{
		sink(c.ptr).predecessor_link = p.ptr;
	}

	template<typename T>
	constexpr const T& source(tree_coordinate<T> c)
	{
		return source(c.ptr).value;
	}

	template<typename T>
	constexpr T& sink(tree_coordinate<T> c)
	{
		return sink(c.ptr).value;
	}

	static int tree_node_count = 0; /* ******* TESTING ******* */

	template<typename T>
		requires(Regular(T))
	struct tree_node_construct 
	{
		typedef tree_coordinate<T> C;
		tree_node_construct() {}
		C operator()(T x, C l = C{ 0 }, C r = C{ 0 }, C p = C{ 0 })
		{
			++tree_node_count;
			return C(new tree_node<T>(x, l.ptr, r.ptr, p.ptr));
		}
		C operator()(C c)           { return (*this)(source(c), left_successor(c), right_successor(c)); }
		C operator()(C c, C l, C r) { return (*this)(source(c), l, r); }
	};

	template<typename T>
		requires(Regular(T))
	struct tree_node_destroy
	{
		tree_node_destroy() = default;
		void operator()(tree_coordinate<T> c)
		{
			--tree_node_count;
			delete c.ptr;
		}
	};

	template<typename C, typename Cons>
	requires(EmptyLinkedBifurcateCoordinate(C) &&
		TreeNodeConstructor(Cons) && NodeType(C) == NodeType(Cons))
	C bidirectional_bifurcate_copy(C c)
	{
		Cons construct_node;
		if (empty(c)) return c;              // Us      / Lee
		C stack = construct_node(c, c, C()); // stack   / V'
		C c_new = stack;                     // c\_new  / COPY
		while (!empty(stack)) {              // empty() / null
			c = left_successor(stack);       // c       / V
			C l = left_successor(c);
			C r = right_successor(c);
			C top = stack;
			if (!empty(l)) {
				if (!empty(r)) {
					r = construct_node(r, r, right_successor(stack));
					stack = construct_node(l, l, r);
				}
				else {
					r = C();
					stack = construct_node(l, l, right_successor(stack));
				}
				l = stack;
			}
			else if (!empty(r)) {
				stack = construct_node(r, r, right_successor(stack));
				r = stack;
			}
			else
				stack = right_successor(stack);
			set_right_successor(top, r);
			set_left_successor(top, l);
			if (!empty(r)) set_predecessor(r, top);
			if (!empty(l)) set_predecessor(l, top);
		}
		return c_new;
	}

	template<typename T>
		requires(Regular(T))
	struct tree
	{
		typedef tree_coordinate<T> C;
		typedef tree_node_construct<T> Cons;
		C root;
		tree() : root(0) {}
		tree(T value) : root(Cons{}(value)) {}
		tree(T value, const tree& left, const tree& right) : root(Cons{}(value))
		{
			set_left_successor(root, bidirectional_bifurcate_copy<C, Cons>(left.root));
			set_right_successor(root, bidirectional_bifurcate_copy<C, Cons>(right.root));
			if (has_left_successor(root))
				set_predecessor(left_successor(root), root);
			if (has_right_successor(root))
				set_predecessor(right_successor(root), root);
		}
		tree(const tree& x) : root(bidirectional_bifurcate_copy<C, Cons>(x.root)) {}
		~tree()
		{
			bifurcate_erase(root, tree_node_destroy<T>{});
		}
		void operator=(tree&& t)
		{
			std::swap(root, t.root);
		}
	};

	template<typename T>
		requires(Regular(T))
	struct coordinate_type<tree<T>>
	{
		typedef tree_coordinate<T> type;
	};

	template<typename T>
		requires(Regular(T))
	struct value_type<tree<T>> 
	{
		typedef ValueType(CoordinateType(tree<T>)) type;
	};

	template<typename T>
		requires(Regular(T))
	struct weight_type 
	{
		typedef WeightType(CoordinateType(tree<T>)) type;
	};

	template<typename T>
		requires(Regular(T))
	tree_coordinate<T> begin(const tree<T>& x)
	{
		return x.root;
	}

	template<typename T>
		requires(Regular(T))
	bool empty(const tree<T>& x) 
	{
		return empty(x.root);
	}

	template<typename T>
		requires(Regular(T))
	bool operator==(const tree<T>& x, const tree<T>& y)
	{
		return bifurcate_equal(begin(x), begin(y));
	}

	template<typename T, typename Proc>
		requires(Regular(T))
	Proc traverse(const tree<T>& x)
	{
		return traverse(begin(x), proc);
	}

} // namespace eop