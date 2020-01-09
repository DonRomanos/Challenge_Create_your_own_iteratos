# Using stl algorithms with custom collections
Your task: Make the example collection usable for stl algorithms, precisely std::find and std::sort. Your task is fulfilled if the code compiles and the provided unit tests are green.

Goal: Get familiar with stl iterators, how to write your own.

Bonus: Experiment with ranges, find a convenient way to write comparison operations.

My reference implementation is in reference.h and an explanation of all the things I learned within the reference and at the end of this file.

Note: This time there is no benchmark as there should be no real logic in the iterators, if one is requested I can add it later.
## How to use
Requirements:
* CMake
* Conan
* Python (for Conan)
* C++20 capable compiler 

Setting up a Conan profile for your environment
```
conan profile new WhateverCompilerSettingsYouWant --detect
```
For more Infos see: https://docs.conan.io/en/latest/reference/profiles.html

Installing the Libraries using Conan
```
// Good news google benchmark is now on conan-center :)
mkdir build && cd build
conan install .. --build missing
conan build ..
```

Now you should see the library successfully compiling and running the tests.

#### Note
If you don't have the dependencies or dont want to build with them for some reason you can disable them with these CMake Options, simply set them to 'OFF'.

```
BUILD_REFERENCE
BUILD_TESTS
```

### Alternative without Conan
Requirements:
* CMake

You can build the library without Conan if you manually install the gtest and google benchmark libraries.

Just tell Cmake where to find them by setting following CMake Variables

```
gtest_DIR
```

You can do so via command line
```
cmake -Dgtest_DIR=usr/local/...
```
or via the gui by adding a path entry with the name.

# Learnings and Explanations
...