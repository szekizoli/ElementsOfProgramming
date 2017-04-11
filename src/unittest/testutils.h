#include <utility>
#include <vector>
#include "intrinsics.h"
#include "list.h"
#include "type_functions.h"

// utility methods for testing
namespace eoptest {

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
}