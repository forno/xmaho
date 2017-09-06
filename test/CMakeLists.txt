add_subdirectory(gtest)

add_executable(test_gtest gtest.cpp)
target_link_libraries(test_gtest gtest_main)
add_executable(test_gmock gmock.cpp)
target_link_libraries(test_gmock gmock_main)

set(TEST_TARGETS
  test_gtest
  test_gmock
)

# Add some test
add_subdirectory(std_ext)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG(-std=c++1z HAVE_FLAG_CXX17)
if(HAVE_FLAG_CXX17)
  foreach(i IN LISTS TEST_TARGETS)
    target_compile_options(${i} PRIVATE -std=c++1z)
  endforeach(i IN LISTS TEST_TARGETS)
else(HAVE_FLAG_CXX17)
  set_property(TARGET ${TEST_TARGETS} PROPERTY CXX_STANDARD 14)
  set_property(TARGET ${TEST_TARGETS} PROPERTY CXX_STANDARD_REQUIRED True)
  set_property(TARGET ${TEST_TARGETS} PROPERTY CXX_EXTENSIONS False)
endif(HAVE_FLAG_CXX17)

foreach(i IN LISTS TEST_TARGETS)
  add_test(NAME ${i} COMMAND ${i})
endforeach(i IN LISTS TEST_TARGETS)
