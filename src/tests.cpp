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
    TypeParam container{ 3,3,3,2,2,2,1,1,1 };
    TypeParam expected_after_sorting{ 1,1,1,2,2,2,3,3,3 };

    std::sort(container.begin(), container.end());

    EXPECT_EQ(expected_after_sorting, container);
}

//TYPED_TEST_P(CustomCollectionTests, Find_finds_correct_element) {
//    // Inside a test, refer to TypeParam to get the type parameter.
//    // TypeParam n = 0;
//    SUCCEED();
//}

/***************************************************************/
// Register all test names, e.g. Should_Succeed
REGISTER_TYPED_TEST_CASE_P(CustomCollectionTests,
    After_sort_elements_in_correct_order);

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