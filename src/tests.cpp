#include "reference.hpp"
#include "implement_here.hpp"
#include "gtest/gtest.h"

template <typename T>
class CustomCollectionTests : public ::testing::Test{}; // need this for the typed test...
TYPED_TEST_CASE_P(CustomCollectionTests);

/***************************************************************/
/* Actual tests */
TYPED_TEST_P(CustomCollectionTests, After_sort_elements_in_correct_order) 
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

TYPED_TEST_P(CustomCollectionTests, Find_can_be_used_on_container)
{
    TypeParam container{ .values = {1,2,3,4,5,6,7} };
    const int to_find = 4;

    auto result = std::find(container.begin(), container.end(), to_find);

    EXPECT_EQ(*result, to_find);
}

/***************************************************************/
// Register all test names, e.g. Should_Succeed
REGISTER_TYPED_TEST_CASE_P(CustomCollectionTests,
    After_sort_elements_in_correct_order,
    Sorting_empty_container_should_not_crash,
    Container_is_usable_in_range_based_for,
    Find_can_be_used_on_container);

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