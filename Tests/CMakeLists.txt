project(Tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# adding the Google_Tests_run target
add_executable(UnitTests GTest.cpp)

# linking Google_Tests_run with DateConverter_lib which will be tested
target_link_libraries(UnitTests Engine)

target_link_libraries(UnitTests gtest gtest_main)