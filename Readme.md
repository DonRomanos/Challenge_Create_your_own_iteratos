# Using STL Algorithms with Custom Collections

Something every library writer probably has or will encounter ... about collections and iterators ...

## Task

 Make the example collection usable for stl algorithms, precisely **std::find** and **std::sort**. You are done once all unit tests are green. My reference implementation is provided in the file *reference.hpp*.

### Requirements

* provide the collection with an iterator
* provide the collection with a const_iterator
* make sure both iterators are equality comparable
* make sure iterator is convertible into const_iterator
* std::find and std::sort must then work on the collection

**This should be fulfilled when all unit tests are green!**

## Goal

Get familiar with stl iterators, find out how to write your own. Avoid code duplication between const and non const types.

**Bonus:**
Find a convenient way to write comparison operations, get to know some of the new standard keywords and features.

**Note:**
 This time there is no benchmark as there should be no real logic in the iterators, if one is requested I can add it later.

## How to use

Requirements:

* CMake
* Conan
* Python (for Conan)
* C++20 capable compiler

Setting up a Conan profile for your environment

```shell
conan profile new WhateverCompilerSettingsYouWant --detect
```

For more Infos see: https://docs.conan.io/en/latest/reference/profiles.html

Installing the Libraries using Conan

```shell
mkdir build && cd build
conan install .. --build missing
conan build ..
```

Now you should see the library successfully compiling and running the tests.

**Note:**
If you don't have the dependencies or dont want to build with them for some reason you can disable them with these CMake Options, simply set them to 'OFF'.

```cmake
BUILD_REFERENCE
BUILD_TESTS
```

### Alternative without Conan

Requirements:

* CMake

You can build the library without Conan if you manually install the gtest and google benchmark libraries.

Just tell Cmake where to find them by setting following CMake Variables

```cmake
gtest_DIR
```

You can do so via command line

```shell
cmake -Dgtest_DIR=usr/local/...
```

or via the gui by adding a path entry with the name.

## Learnings and Explanations

If you are curious how my implementation went and what I learned during developement take a peek.

I can say that this was suprisingly tough for me and I was googling and stackoverflowing quite a lot during developement.

<details><summary>Spoiler Alert</summary>
<p>

First some dry theory... if you just want to see my personal remarks, you can find them at the end.

### Requirements for STL-Iterators

The STL knows four different types of iterators, each with its own requirements:

* input_iterator
* output_iterator
* forward_iterator
* bidirectional_iterator
* random_access_iterator
* contiguous_iterator (since C++20)

You have to provide the following typedefs / aliases inside an iterator class to make it usable with stl-algorithms.
 (There used to be a std::iterator template which you could inherit but it was deprecated due to readability issues, you can read more about it here: [FluentCpp - Why std::iterator was deprecated](https://www.fluentcpp.com/2018/05/08/std-iterator-deprecated/) )

* difference_type
* value_type
* pointer
* reference
* **iterator_category** (one of the above four + _tag)

The interesting part here is actually only the iterator category. The category is ordered in a way that each iterator is more constraining then the one before and requires you to provide more methods.

Most important are:

* equality comparable: operator==
* dereferencable: operator* / operator->
* incrementable: pre- / postincrement
* decrementable: pre- / postdecrement

For Random Access Iterators:

* ordering: <,>,<=,>=
* arithmetic: +=, -=, +, - (with iterators and ints)
* arbitrary dereferencing: operator[]

(My iterator does not actually provide all of the above but only most, as I created it specifically for std::find and std::sort)

An output iterator is a bit special as almost all types will be void. It only requires increment and assignment, it cannot be used to read a value.

The reason for the iterator category is a performance optimization, behind it stands a technique called tag-dispatching.

#### Tag Dispatching

We use the compiler to select the correct code path at compile time. This requires no virtual dispatch and should therefore be faster.

Simple Example:

```cpp
template<typename T>
int distance(T first, T second, random_access_iterator_tag)
{
    return second - first;
}

template<typename T>
int distance(T first, T second, forward_iterator_tag)
{
    int distance = 0;
    while(first != second)
    {
        ++distance;
    }
    return distance;
}

template<typename T>
int distance(T first, T second)
{
    return distance(first, second, T::iterator_category)
}
```

### The Problem of Constness

Most STL containers provide in addition to regular iterators also const versions, which leads us to the problem of code duplication, as the const and non const version have basically the same methods with only some consts that differ.

This is a bit tricky and can be solved in numerous ways.

I decided to go for a template, where I use a boolean flag to decide wheter the iterator is const or not. To disable certain methods for each kind enable_if is used.

With this combination we can pack everything into one class and have basically no duplication.

The conversion from iterator to const_iterator is implemented with an enable_if custom conversion operator only for const_iterator.

This way I only ever need to include typedefs in my class, everything else is automatically taken care of(like equality comparison etc).

The basic idea is from [here](https://stackoverflow.com/questions/2150192/how-to-avoid-code-duplication-implementing-const-and-non-const-iterators).

#### Code duplication in Const and Non-Const Member Functions

Something unrelated I learned while googling. The right way to avoid code duplication in const and non-const Member functions is to have the non-const version call the const one and not the other way around!

So the logic is in the const function! Makes sense when I thought about it, but is a bit counter intuitive. See [here](https://stackoverflow.com/questions/123758/how-do-i-remove-code-duplication-between-similar-const-and-non-const-member-func) for an explanation.

Example:

```cpp
T const & f() const {
    return something_complicated();
}
T & f() {
    return const_cast<T &>(std::as_const(*this).f());
}
```

### Learnings in short

* Spaceship operator is very useful, if you use *=default* it takes care of equality comparisons as well ( if you use a custom one you have to provide it yourself)
* *Don't* try to *dereference* an *.end iterator* even if you never plan to do anything with the element
* \+ takes precedence over any \* -> & operators
* Changing the signature of a function based on a template parameter is possible using SFINAE e.g. const and non-const functions
* Use the following keywords whenever appropriate: const, [[nodiscard]], noexcept, explicit

</p>
</details>

## For the Future

If I find some time and am in the right mood, I will expand this to provide a proper container, not something as basic as this.

Also if you can use [boost](https://www.boost.org/doc/libs/1_65_0/libs/iterator/doc/iterator_facade.html) there are a lot of convenience types and iterator facades which do most of the work for you, so no need to implement this yourself ;).

## Please give me some Feedback if this helped or you have any suggestions or remarks
