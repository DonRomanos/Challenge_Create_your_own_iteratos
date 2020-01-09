cmake_minimum_required(VERSION 3.14)

project(custom_collection_with_stl)

add_library(${PROJECT_NAME} INTERFACE)
add_library(challenge::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME} INTERFACE
	src/implement_here.hpp)

# Normally I would make this private, but since this is my own and I am lazy I can do this :)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)
target_compile_options(${PROJECT_NAME} INTERFACE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)

######################################################################
option(BUILD_REFERENCE "Will build a reference implementation as a library, 
                        which will also be tested. Cool for comparison." ON)
if(BUILD_REFERENCE)
    add_library(${PROJECT_NAME}_Reference INTERFACE)
    add_library(challenge::${PROJECT_NAME}_Reference ALIAS ${PROJECT_NAME}_Reference)

    target_sources(${PROJECT_NAME}_Reference INTERFACE
        src/reference.hpp)
endif()

######################################################################
option(BUILD_TESTS "Will build the tests verifying your result" ON) # If you disable this there will not be much to build...
if( BUILD_TESTS)
    find_package(gtest REQUIRED)
    add_executable(${PROJECT_NAME}_Tests)
    target_sources(${PROJECT_NAME}_Tests PRIVATE 
        src/tests.cpp)
    target_include_directories(${PROJECT_NAME}_Tests PRIVATE
        src)
    target_link_libraries(${PROJECT_NAME}_Tests PRIVATE
        challenge::${PROJECT_NAME}
        gtest::gtest
        $<$<TARGET_EXISTS:${PROJECT_NAME}_Reference>:${PROJECT_NAME}_Reference>)

    enable_testing()
    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME}_Tests)
endif()
