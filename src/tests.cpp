#include "reference.hpp"
#include "implement_here.hpp"
#include "gtest/gtest.h"

template <typename T>
class CustomCollectionTests : public ::testing::Test{}; // need this for the typed test...
TYPED_TEST_CASE_P(CustomCollectionTests);

/***************************************************************/
TYPED_TEST_P(CustomCollectionTests, Should_Succeed) {
    // Inside a test, refer to TypeParam to get the type parameter.
    // TypeParam n = 0;
    SUCCEED();
}

// Register all test names, e.g. Should_Succeed
REGISTER_TYPED_TEST_CASE_P(CustomCollectionTests,
    Should_Succeed);

// Now we can instantiate it with our types.
#ifdef BUILD_REFERENCE
typedef ::testing::Types<onions::SomeClass, reference::SomeClass> TestTypes;
INSTANTIATE_TYPED_TEST_CASE_P(My, CustomCollectionTests, TestTypes);
#else
typedef ::testing::Types<MyContainer> TypesToTest;
INSTANTIATE_TYPED_TEST_CASE_P( YouWinIfAllOfTheseAreGreen, CustomCollectionTests, TypesToTest);
#endif

// Had some Problems with the conan cmake_find_package generator, 
// it does not generate the correct targets therefore we dont have a gtest_main target
// so I write my own main.
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}