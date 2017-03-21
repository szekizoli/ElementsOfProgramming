#include <utility>
#include <vector>
#include "intrinsics.h"
#include "list.h"

// utility methods for testing
namespace eoptest {

    template<typename T>
		requires(Regular(T))
	std::vector<T> list_to_vector(eop::slist_coordinate<T> c)
	{
		std::vector<T> v;
		while (!empty(c)) {
			v.push_back(source(c));
			c = successor(c);
		}
		return v;
	}

    template<typename T>
		requires(Regular(T))
	std::vector<T> list_to_vector(eop::slist_coordinate<T> f, eop::slist_coordinate<T> l)
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