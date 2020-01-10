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
	struct iterator;
	struct const_iterator;

	iterator begin() { return iterator{ *this, values.data() }; };
	iterator end() { return iterator{ *this, values.data() + values.size() }; };
	const_iterator begin() const { return const_iterator{ begin() }; };
	const_iterator end() const{ return const_iterator{ end() }; };
	const_iterator cbegin() const { return const_iterator{ begin() }; };
	const_iterator cend() const { return const_iterator{ end() }; };
	std::reverse_iterator<iterator> rbegin() { return std::make_reverse_iterator<iterator>(end()); }
	std::reverse_iterator<iterator> rend() { return std::make_reverse_iterator<iterator>(begin()); }

	auto operator<=>(const MyContainer&)const = default;

	struct iterator
	{
		using iterator_category = std::random_access_iterator_tag;
		using value_type = int;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type*;
		using reference = value_type&;

		iterator(MyContainer& container, pointer position) : container(&container), current_element(position) {}
		iterator& operator=(const iterator& other) = default;
		iterator(const iterator& other) = default;

		iterator& operator++() { ++current_element; return *this; }
		iterator operator++(int value) { return iterator{*container, current_element + value }; }
		iterator& operator--() { --current_element; return *this; }
		iterator operator--(int value) { return iterator{*container, current_element - value }; }
		friend iterator operator+(const iterator& it, int index) { return iterator{ *it.container, it.current_element + index }; }
		friend iterator operator+(int index, const iterator& it) { return iterator{ *it.container, it.current_element + index }; }
		friend iterator operator-(const iterator& it, int index) { return iterator{ *it.container, it.current_element - index }; }
		friend difference_type operator-(const iterator& it, const iterator& it2) { return it.current_element - it2.current_element; }

		iterator& operator+=(int index) { current_element += index; return *this; }
		iterator& operator-=(int index) { current_element -= index; return *this; }

		reference operator*() { return *current_element; }
		reference operator[](size_t pos) { return container->values[pos]; }

		const_iterator operator()() const { return const_iterator{*this}; }

		auto operator<=>(const iterator&) const = default;
	private:
		MyContainer* container;
		pointer current_element;
	};

	struct const_iterator
	{
		using iterator_category = std::random_access_iterator_tag;
		using value_type = int;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type*;
		using reference = value_type&;

		const_iterator(const iterator& base) : iter(base){}
		const_iterator(const const_iterator& other) = default;
		const_iterator& operator=(const const_iterator& other) = default;

		const_iterator& operator++() { ++iter; return *this; }
		const_iterator operator++(int value) { return const_iterator{ iter + value }; }
		const_iterator& operator--() { --iter; return *this; }
		const_iterator operator--(int value) { return const_iterator{ iter - value }; }
		friend const_iterator operator+(const const_iterator& it, int index) { return const_iterator{ it.iter + index }; }
		friend const_iterator operator+(int index, const const_iterator& it) { return const_iterator{ it.iter + index }; }
		friend const_iterator operator-(const const_iterator& it, int index) { return const_iterator{ it.iter - index }; }
		friend difference_type operator-(const const_iterator& it, const const_iterator& it2) { return it.iter - it2.iter; }

		const_iterator& operator+=(int index) { iter += index; return *this; }
		const_iterator& operator-=(int index) { iter -= index; return *this; }

		const value_type& operator*(){ return *iter; }
		const value_type& operator[](size_t pos){ return iter[pos]; }

		auto operator<=>(const const_iterator&) const = default;
	private:
		iterator iter;
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