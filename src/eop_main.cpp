#include <cstdlib> // srand, rand
#include <ctime>   // time
#include <iostream>

#include <random>

#include "eop.h"
#include "common.h"
#include "type_functions.h"

using namespace eop;

//#define CHAPTER_1
//#define CHAPTER_2
#define CHAPTER_3

std::linear_congruential_engine<std::uint_fast32_t, 16807, 0, 2147483647> r;

int add(int x, int y) {
	return x + y;
}

void add_accumulate(int& x, int y) {
	x = x + y;
}

int mul(int x, int y) {
	return x * y;
}

void mul_accumulate(int& x, const int y) {
	x = x * y;
}

int sub(int x, int y) {
	return x - y;
}

void sub_accumulate(int& x, const int y) {
	x = x - y;
}

int next_rand(int x) {
	r.seed(x);
	return r();
}

template<typename T, typename U, typename V>
std::ostream& operator<<(std::ostream& o, std::tuple<T, U, V> t) {
	o << "(" << std::get<0>(t) << ", " << std::get<1>(t) << ", " << std::get<2>(t) << ")\n";
	return o;
}

int main() {
	std::cout << "Elements of Programming exercises.\n";

#ifdef CHAPTER_1
	std::cout << "Chapter 1 - Foundations.\n";
	std::cout << "auto x = square(4, multiply);\n";
	auto x_1 = square(4, multiply);
	std::cout << "x = " << x_1 << std::endl;
#endif

#ifdef CHAPTER_2
	std::cout << "Chapter 2 - Transformations and Their Orbits.\n";
	std::cout << "auto x = power_unary(0, 10, increment);\n";
	auto x_2 = power_unary(0, 10, increment);
	std::cout << "x = " << x_2 << std::endl;
	std::cout << "auto x = distance(0, 10, increment);\n";
	auto x_3 = distance(0, 10, increment);
	std::cout << "x = " << x_3 << std::endl;
	std::cout << "auto x = collision_point(0, increment, less_than_ten);\n";
	auto x_4 = collision_point(0, increment, less_than_ten);
	std::cout << "x = " << x_4 << std::endl;
	std::cout << "auto x = collision_point_non_terminating_orbit(42, next_rand);\n";
	auto x_5 = collision_point_nonterminating_orbit(42, next_rand);
	std::cout << "x = " << x_5 << std::endl;
	std::cout << "bool c = circular_nonterminating_orbit(42, next_rand);\n";
	bool c_1 = circular_nonterminating_orbit(42, next_rand);
	std::cout << "rand starting with '42' is circular? " << c_1 << std::endl;
	std::cout << "bool c = circular_nonterminating_orbit(2981, next_rand);\n";
	bool c_2 = circular_nonterminating_orbit(2981, next_rand);
	std::cout << "rand starting with '2981' is circular? " << c_2 << std::endl;
	std::cout << "auto x = connection_point_nonterminating_orbit(2981, next_rand);\n";
	auto x_6 = connection_point_nonterminating_orbit(2981, next_rand);
	std::cout << "x = " << x_6 << std::endl;
	std::cout << "auto x = connection_point_nonterminating_orbit(42, next_rand);\n";
	auto x_7 = connection_point_nonterminating_orbit(42, next_rand);
	std::cout << "x = " << x_7 << std::endl;
	std::cout << "auto orbit_structure = orbit_structure_nonterminating_orbit(42, next_rand);\n";
	auto orbit_structure_0 = orbit_structure_nonterminating_orbit(42, next_rand);
	std::cout << "orbit_structure = " << orbit_structure_0 << std::endl;
	auto seed = std::time(0);
	std::cout << "auto orbit_structure = orbit_structure_nonterminating_orbit("<< seed << ", next_rand);\n";
	auto orbit_structure_1 = orbit_structure_nonterminating_orbit(seed, next_rand);
	std::cout << "orbit_structure = " << orbit_structure_1 << std::endl;
#endif

#ifdef CHAPTER_3
	std::cout << "Chapter 3 - Associative Operations.\n";
	std::cout << "Applying power_left_associated\n";
	// Computing a^n
	int a = 2;
	int n = 9;
	int r = power_left_associated(a, n, add);
	std::cout << "(" << a << ", " << n << ", add) = " << r << std::endl;
	r = power_left_associated(a, n, mul);
	std::cout << "(" << a << ", " << n << ", mul) = " << r << std::endl;
	r = power_left_associated(a, n, sub);
	std::cout << "(" << a << ", " << n << ", sub) = " << r << std::endl;
	std::cout << "Applying power_right_associated\n";
	r = power_right_associated(a, n, add);
	std::cout << "(" << a << ", " << n << ", add) = " << r << std::endl;
	r = power_right_associated(a, n, mul);
	std::cout << "(" << a << ", " << n << ", mul) = " << r << std::endl;
	r = power_right_associated(a, n, sub);
	std::cout << "(" << a << ", " << n << ", sub) = " << r << std::endl;
	std::cout << "Applying power_0\n";
	r = power_0(a, n, add);
	std::cout << "(" << a << ", " << n << ", add) = " << r << std::endl;
	r = power_0(a, 1, mul);
	std::cout << "(" << a << ", 1, mul) = " << r << std::endl;
	r = power_0(a, n, mul);
	std::cout << "(" << a << ", " << n << ", mul) = " << r << std::endl;
	std::cout << "Applying power_1\n";
	r = power_1(a, n, add);
	std::cout << "(" << a << ", " << n << ", add) = " << r << std::endl;
	r = power_1(a, n, mul);
	std::cout << "(" << a << ", " << n << ", mul) = " << r << std::endl;
	std::cout << "Applying power_accumulate_0\n";
	r = power_accumulate_0(a, 0, n, add);
	std::cout << "(" << a << ", 0, " << n << ", add) = " << r << std::endl;
	r = power_accumulate_0(a, 1, n, mul);
	std::cout << "(" << a << ", 1, " << n << ", mul) = " << r << std::endl;
	std::cout << "Applying power_accumulate_1\n";
	r = power_accumulate_1(a, 0, n, add);
	std::cout << "(" << a << ", 0, " << n << ", add) = " << r << std::endl;
	r = power_accumulate_1(a, 1, n, mul);
	std::cout << "(" << a << ", 1, " << n << ", mul) = " << r << std::endl;
	std::cout << "Applying power_accumulate_2\n";
	r = power_accumulate_2(a, 0, n, add);
	std::cout << "(" << a << ", 0, " << n << ", add) = " << r << std::endl;
	r = power_accumulate_2(a, 1, n, mul);
	std::cout << "(" << a << ", 1, " << n << ", mul) = " << r << std::endl;
	std::cout << "Applying power_accumulate_3\n";
	r = power_accumulate_3(a, 0, n, add);
	std::cout << "(" << a << ", 0, " << n << ", add) = " << r << std::endl;
	r = power_accumulate_3(a, 1, n, mul);
	std::cout << "(" << a << ", 1, " << n << ", mul) = " << r << std::endl;
	std::cout << "Applying power_accumulate_4\n";
	r = power_accumulate_4(a, 0, n, add);
	std::cout << "(" << a << ", 0, " << n << ", add) = " << r << std::endl;
	r = power_accumulate_4(a, 1, n, mul);
	std::cout << "(" << a << ", 1, " << n << ", mul) = " << r << std::endl;
	std::cout << "Applying power_accumulate_positive_0\n";
	r = power_accumulate_positive_0(a, 0, n, add);
	std::cout << "(" << a << ", 0, " << n << ", add) = " << r << std::endl;
	r = power_accumulate_positive_0(a, 1, n, mul);
	std::cout << "(" << a << ", 1, " << n << ", mul) = " << r << std::endl;
	std::cout << "Applying power_5\n";
	r = power_accumulate_5(a, 0, n, add);
	std::cout << "(" << a << ", 0, " << n << ", add) = " << r << std::endl;
	r = power_accumulate_5(a, 1, n, mul);
	std::cout << "(" << a << ", 1, " << n << ", mul) = " << r << std::endl;
	std::cout << "Applying power_2\n";
	r = power_2(a, n, add);
	std::cout << "(" << a << ", " << n << ", add) = " << r << std::endl;
	r = power_2(a, 1, mul);
	std::cout << "(" << a << ", 1, mul) = " << r << std::endl;
	r = power_2(a, n, mul);
	std::cout << "(" << a << ", " << n << ", mul) = " << r << std::endl;
	std::cout << "Applying power_3\n";
	r = power_3(a, n, add);
	std::cout << "(" << a << ", " << n << ", add) = " << r << std::endl;
	r = power_3(a, 1, mul);
	std::cout << "(" << a << ", 1, mul) = " << r << std::endl;
	r = power_3(a, 8, mul);
	std::cout << "(" << a << ", 8, mul) = " << r << std::endl;
	r = power_3(a, n, mul);
	std::cout << "(" << a << ", " << n << ", mul) = " << r << std::endl;

	std::cout << "Applying power_accumulate\n";
	r = power_accumulate(a, 0, n, add);
	std::cout << "(" << a << ", 0, " << n << ", add) = " << r << std::endl;
	r = power_accumulate(a, 1, n, mul);
	std::cout << "(" << a << ", 1, " << n << ", mul) = " << r << std::endl;
	std::cout << "Applying power_accumulate_positive\n";
	r = power_accumulate_positive(a, 0, n, add);
	std::cout << "(" << a << ", 0, " << n << ", add) = " << r << std::endl;
	r = power_accumulate_positive(a, 1, n, mul);
	std::cout << "(" << a << ", 1, " << n << ", mul) = " << r << std::endl;
	std::cout << "Applying power\n";
	r = power(a, n, add);
	std::cout << "(" << a << ", " << n << ", add) = " << r << std::endl;
	r = power(a, 1, mul);
	std::cout << "(" << a << ", 1, mul) = " << r << std::endl;
	r = power(a, 8, mul);
	std::cout << "(" << a << ", 8, mul) = " << r << std::endl;
	r = power(a, n, mul);
	std::cout << "(" << a << ", " << n << ", mul) = " << r << std::endl;
	for (int i = 0; i < 15; ++i) 
		std::cout << "fibonacci("<<i<<") = " << fibonacci(i) << std::endl;

	// Accumulative versions
	std::cout << "Left associative." << std::endl;
	int k = 0;
	for (int i = 1; i < 10; ++i) {
		r = power_left_associated_accumulative(a, i, add_accumulate);
		std::cout << "(" << a << ", " << i << ", add) = " << r << std::endl;
		r = power_left_associated_accumulative(a, i, mul_accumulate);
		std::cout << "(" << a << ", " << i << ", mul) = " << r << std::endl;
		r = power_left_associated_accumulative(a, i, sub_accumulate);
		std::cout << "(" << a << ", " << i << ", sub) = " << r << std::endl;
	}

	std::cout << "Right associative." << std::endl;

	for (int i = 1; i < 10; ++i) {
		r = power_right_associated_accumulative(a, i, add_accumulate);
		std::cout << "(" << a << ", " << i << ", add) = " << r << std::endl;
		r = power_right_associated_accumulative(a, i, mul_accumulate);
		std::cout << "(" << a << ", " << i << ", mul) = " << r << std::endl;
		r = power_right_associated_accumulative(a, i, sub_accumulate);
		std::cout << "(" << a << ", " << i << ", sub) = " << r << std::endl;
	}

	std::cout << "Applying power_accumulate\n";
	r = power_accumulate(a, 0, n, add);
	std::cout << "(" << a << ", 0, " << n << ", add) = " << r << std::endl;
	r = power_accumulate(a, 1, n, mul);
	std::cout << "(" << a << ", 1, " << n << ", mul) = " << r << std::endl;
	std::cout << "Applying power_accumulate_positive_accumulative\n";
	r = power_accumulate_positive_accumulative(a, 0, n, add_accumulate);
	std::cout << "(" << a << ", 0, " << n << ", add) = " << r << std::endl;
	r = power_accumulate_positive_accumulative(a, 1, n, mul_accumulate);
	std::cout << "(" << a << ", 1, " << n << ", mul) = " << r << std::endl;
	std::cout << "Applying power\n";
	r = power(a, n, add);
	std::cout << "(" << a << ", " << n << ", add) = " << r << std::endl;
	r = power(a, 1, mul);
	std::cout << "(" << a << ", 1, mul) = " << r << std::endl;
	r = power(a, 8, mul);
	std::cout << "(" << a << ", 8, mul) = " << r << std::endl;
	r = power(a, n, mul);
	std::cout << "(" << a << ", " << n << ", mul) = " << r << std::endl;
	for (int i = 0; i < 15; ++i)
		std::cout << "fibonacci(" << i << ") = " << fibonacci(i) << std::endl;
#endif
}