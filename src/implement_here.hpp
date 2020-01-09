#pragma once

#include <compare>
#include <algorithm>
#include <vector>

// Level 0 -> Spaceship operator
// Level 1 -> Iterators for your type useful for a library
// Level 2 -> Ranges for your type useful for a library

struct SomethingIwantToCompare
{
	int value;

	// old approach: bool operator<(...), operator>, operator==, .... boiler plate...
	// Now we get -> Spaceship
	// auto operator<=>(const SomethingIwantToCompare&) const = default; // There fixed. ( Note this will implicitly default operator==)

	std::weak_ordering operator<=>(const SomethingIwantToCompare& other) const
	{
		if (value < other.value)
		{
			return std::weak_ordering::greater;
		}
		else if (value > other.value)
		{
			return std::weak_ordering::less;
		}
		else if (value == other.value)
		{
			return std::weak_ordering::equivalent;
		}
		return std::weak_ordering::equivalent;
	}

	bool operator==(const SomethingIwantToCompare& other) const = default; // If we define a custom spaceship operator we need to provide an equality operator
};

struct MyContainer
{
	std::vector<int> values;

	struct iterator
	{
		using iterator_category = std::random_access_iterator_tag;
		using value_type = int;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type*;
		using reference = value_type&;

		iterator(MyContainer& container) : container(&container), current_element(nullptr){}
		iterator(MyContainer& container, pointer position) : container(&container), current_element(position) {}
		iterator& operator=(const iterator& other) { container = other.container; current_element = other.current_element; return *this; }
		iterator(const iterator& other) : container(other.container), current_element(other.current_element) {}

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

		auto operator<=>(const iterator&) const = default;
	private:
		MyContainer* container;
		pointer current_element;
	};

	iterator begin(){ return iterator{ *this, values.data() }; };
	iterator end(){ return iterator{ *this, values.data() + values.size() }; };

	auto operator<=>(const MyContainer&)const = default;
};

// Learnings;
// Default is very useful ( actually it even takes into account optimizations e.g. for == operator for strings ( by creating default == operator in addition) 
// 3 Way comparison works by giving you an ordering in the form of e.g. std::strong_ordering::less (which is a typedef for -1? I believe) the compiler uses that to synthesize the wanted expression, e.g. (4 <=> 5) < 0 for <operator
// Overload resolution takes into account synthesized expressions after all other so no ambiguity if you want to define only some of your own comparison operators
// A custom 3 way comparison is not used as overload resolution for equality, reasons are performance, e.g. lexicographical comparison of strings compares each character, while testing for equality you can test for equal sizes first then no loop is required.
// Don't try to dereference an .end iterator even if you never plan to do anything with the element.
// + takes precedence over any * -> & operators