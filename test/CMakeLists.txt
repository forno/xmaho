cmake_minimum_required(VERSION 3.8.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

add_subdirectory(gtest)

add_executable(test_gtest gtest.cpp)
target_link_libraries(test_gtest gtest_main)
add_test(NAME test_gtest COMMAND test_gtest)

add_executable(test_gmock gmock.cpp)
target_link_libraries(test_gmock gmock_main)
add_test(NAME test_gmock COMMAND test_gmock)

# Add some test
add_subdirectory(std_ext)
