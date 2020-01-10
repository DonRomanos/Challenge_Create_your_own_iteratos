#pragma once

#include <compare>
#include <algorithm>
#include <vector>

// Level 0 -> Spaceship operator
// Level 1 -> Iterators for your type useful for a library
// Level 2 -> Ranges for your type useful for a library

// Todo restructure, make a bit more sophisticated
// Tests reverse iterators.
// Tests const iterators.

struct MyContainer
{
	template<typename T, bool Const = false>
	struct iterator_base;

	using iterator = iterator_base<int>;
	using const_iterator = iterator_base<int, true>;

	iterator begin() { return iterator{ values.data()}; };
	iterator end() { return iterator{ values.data() + values.size() }; };
	const_iterator begin() const { return const_iterator{ values.data()};};
	const_iterator end() const{ return const_iterator{ values.data() + values.size()};}
	const_iterator cbegin() const { return const_iterator{ values.data() };};
	const_iterator cend() const { return const_iterator{ values.data() + values.size()};};
	std::reverse_iterator<iterator> rbegin() { return std::make_reverse_iterator<iterator>(end()); }
	std::reverse_iterator<iterator> rend() { return std::make_reverse_iterator<iterator>(begin()); }

	auto operator<=>(const MyContainer&)const = default;

	template<typename T, bool Const = false>
	struct iterator_base
	{
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = typename std::conditional_t< Const, T const*, T* >;;
		using reference = typename std::conditional_t< Const, T const&, T& >;;

		iterator_base(pointer current) : current(current) {}
		iterator_base(const iterator_base& other) = default;
		iterator_base& operator=(const iterator_base& other) = default;

		iterator_base& operator++() { ++current; return *this; }
		iterator_base operator++(int value) { return iterator_base{ current + value }; }
		iterator_base& operator--() { --current; return *this; }
		iterator_base operator--(int value) { return iterator_base{ current - value }; }
		friend iterator_base operator+(const iterator_base& it, int index) { return iterator_base{ it.current + index }; }
		friend iterator_base operator+(int index, const iterator_base& it) { return iterator_base{ it.current + index }; }
		friend iterator_base operator-(const iterator_base& it, int index) { return iterator_base{ it.current - index }; }
		friend difference_type operator-(const iterator_base& it, const iterator_base& it2) { return it.current - it2.current; }

		iterator_base& operator+=(int index) { current += index; return *this; }
		iterator_base& operator-=(int index) { current -= index; return *this; }

		// This is the tricky part, how can we change the function signature based on a template parameter?
		// Answer: SFINAE yeaaaay, see this: https://stackoverflow.com/questions/2150192/how-to-avoid-code-duplication-implementing-const-and-non-const-iterators
		template<bool Is_Const = Const>
		std::enable_if_t<Is_Const, reference> operator*() const { return *current; }

		template<bool Is_Const = Const>
		std::enable_if_t<!Is_Const, reference> operator*() { return *current; }

		auto operator<=>(const iterator_base&) const = default;
	private:
		pointer current;
	};

	std::vector<int> values;
};

// Learnings;
// Default is very useful ( actually it even takes into account optimizations e.g. for == operator for strings ( by creating default == operator in addition) 
// 3 Way comparison works by giving you an ordering in the form of e.g. std::strong_ordering::less (which is a typedef for -1? I believe) the compiler uses that to synthesize the wanted expression, e.g. (4 <=> 5) < 0 for <operator
// Overload resolution takes into account synthesized expressions after all other so no ambiguity if you want to define only some of your own comparison operators
// A custom 3 way comparison is not used as overload resolution for equality, reasons are performance, e.g. lexicographical comparison of strings compares each character, while testing for equality you can test for equal sizes first then no loop is required.
// Don't try to dereference an .end iterator even if you never plan to do anything with the element.
// + takes precedence over any * -> & operators
// the stl provides you with a template for reverse_iterators: std::reverse_iterator<> no need to implement anything yourself
// creating const and non const versions without doubling the code is difficult: https://www.boost.org/doc/libs/1_41_0/libs/iterator/doc/iterator_facade.html#tutorial-example , https://stackoverflow.com/questions/2150192/how-to-avoid-code-duplication-implementing-const-and-non-const-iterators
// Changing the signature of a function based on a template parameter is possible using SFINAE