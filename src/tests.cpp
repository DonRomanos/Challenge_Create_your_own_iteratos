#include "reference.hpp"
#include "implement_here.hpp"
#include "gtest/gtest.h"

template <typename T>
class CustomCollectionTests : public ::testing::Test{}; // need this for the typed test...
TYPED_TEST_CASE_P(CustomCollectionTests);

/***************************************************************/
/* Actual tests */
TYPED_TEST_P(CustomCollectionTests, Container_can_be_sorted) 
{
    TypeParam container{ .values = {3,3,3,2,2,2,1,1,1} };
    TypeParam expected_after_sorting{ .values = {1,1,1,2,2,2,3,3,3} };

    std::sort(container.begin(), container.end());

    EXPECT_EQ(expected_after_sorting, container);
}

TYPED_TEST_P(CustomCollectionTests, Sorting_empty_container_should_not_crash)
{
    TypeParam container{ .values = {} };

    std::sort(container.begin(), container.end());

    SUCCEED();
}

TYPED_TEST_P(CustomCollectionTests, Container_is_usable_in_range_based_for)
{
    TypeParam container{ .values = {1,2,3,4,5,6,7} };
    std::vector<int> result;
    std::vector<int> expected{ 1,2,3,4,5,6,7 };

    for (auto value : container)
    {
        result.push_back(value);
    }

    EXPECT_EQ(expected, result);
}

TYPED_TEST_P(CustomCollectionTests, Reverse_iterators_work_as_expected)
{
    TypeParam container{ .values = {1,2,3,4,5,6,7} };
    std::vector<int> expected{ 7,6,5,4,3,2,1 };
    std::vector<int> result;
    result.reserve(expected.size());

    std::copy(container.rbegin(), container.rend(), std::back_inserter(result));

    EXPECT_EQ(expected, result);
}

TYPED_TEST_P(CustomCollectionTests, Find_can_be_used_on_NON_const_container)
{
    TypeParam container{ .values = {1,2,3,4,5,6,7} };
    const int to_find = 4;

    auto result = std::find(container.begin(), container.end(), to_find);

    EXPECT_EQ(*result, to_find);
}

TYPED_TEST_P(CustomCollectionTests, Comparison_of_non_const_and_const_iterators_work)
{
    TypeParam container{ .values = {1,2,3,4,5,6,7} };

    EXPECT_TRUE(container.cbegin() == container.begin());
}

TYPED_TEST_P(CustomCollectionTests, Conversion_of_iterators_only_works_from_non_const_to_const)
{
    EXPECT_TRUE((std::is_convertible<TypeParam::iterator, TypeParam::const_iterator>::value));
    EXPECT_FALSE((std::is_convertible<TypeParam::const_iterator, TypeParam::iterator>::value));
}

// My Compiler does not have support for ranges yet...
//TYPED_TEST_P(CustomCollectionTests, Container_works_with_ranges)
//{
//    // Example: print only even values in reverse order
//    TypeParam container{ .values = {1,2,3,4,5,6,7} };
//    const std::vector<int> expected = { 6,4,2 };
//
//    auto is_even = [](auto x) { return x % 2 == 0; };
//    auto result = container | std::ranges::views::filter(is_even) | std::ranges::views::reverse | std::ranges::views::to_vector;
//
//    EXPECT_EQ(expected, result);
//}

TYPED_TEST_P(CustomCollectionTests, Find_can_be_used_on_const_container)
{
    const TypeParam container{ .values = {1,2,3,4,5,6,7} };
    const int to_find = 6;

    auto result = std::find(container.begin(), container.end(), to_find);

    EXPECT_EQ(*result, to_find);
}

/***************************************************************/
// Register all test names, e.g. Should_Succeed
REGISTER_TYPED_TEST_CASE_P(CustomCollectionTests,
    Container_can_be_sorted,
    Sorting_empty_container_should_not_crash,
    Container_is_usable_in_range_based_for,
    Find_can_be_used_on_NON_const_container,
    Reverse_iterators_work_as_expected,
    Find_can_be_used_on_const_container,
    Comparison_of_non_const_and_const_iterators_work,
    Conversion_of_iterators_only_works_from_non_const_to_const);

// Now we can instantiate it with our types.
typedef ::testing::Types<MyContainer> TypesToTest;
INSTANTIATE_TYPED_TEST_CASE_P( YouWinIfAllOfTheseAreGreen, CustomCollectionTests, TypesToTest);

// Had some Problems with the conan cmake_find_package generator, 
// it does not generate the correct targets therefore we dont have a gtest_main target
// so I write my own main.
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}