#pragma once

#include <compare>
#include <vector>

/***************************************************************
Task: Make std::find and std::sort usable with this container thats a wrapper around a vector<int>.
      Your methods should give access to the same elements a vector would.

Following is required
    begin(), end()
    cbegin(), cend()
	rbegin(), rend()
	equality comparison of const and non-const iterators
	conversion from non-const in const iterator

You win when all the tests are green!
*****************************************************************/

struct SomeReallySimpleContainer
{
	using const_iterator = const int*;
	using iterator = int*;
	using reverse_iterator = int*;


	// You are welcome to adapt the signatures to your needs these are just samples so you can start with something compiling
	iterator begin() noexcept { return nullptr; };
	iterator end()   noexcept { return nullptr; };
	const_iterator begin() const noexcept { return nullptr; };
	const_iterator end()   const noexcept { return nullptr;}
	const_iterator cbegin() const noexcept { return nullptr; };
	const_iterator cend()   const noexcept { return nullptr; };
	reverse_iterator rbegin() { return nullptr; }
	reverse_iterator rend()   { return nullptr;}

	auto operator<=>(const SomeReallySimpleContainer&) const = default; // For convenience in the tests ;) 
	std::vector<int> values; // The tests use aggregate initialization so don't make this private
};

// Have fun, for me it was quite a challenge! :)