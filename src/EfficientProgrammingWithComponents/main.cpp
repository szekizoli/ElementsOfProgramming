#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

#include "lecture_5.h"

using namespace std;

void test_odd() {
	vector<int> v = { 5, 3, 32, 1, 9, 2, 10, 32, 2, 1, 14 };
	auto res = minmax_element(begin(v), end(v), std::less<int>());
	cout << "min = " << *res.first << ", max = " << *res.second << endl;
	std::copy(begin(v), end(v), std::ostream_iterator<int>(cout, ", "));
	//v.erase(res.first);
	v.erase(res.second);
	cout << endl;
	std::copy(begin(v), end(v), std::ostream_iterator<int>(cout, ", "));
	// test empty, one element, two elements, even number of elements, odd number of elements
}

int main() {
	
}
