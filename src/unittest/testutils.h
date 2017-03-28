#include <utility>
#include <vector>
#include "intrinsics.h"
#include "list.h"

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

}