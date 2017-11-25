add_executable(test_std_ext_valmatrix valmatrix.cpp)
target_link_libraries(test_std_ext_valmatrix gmock_main)

add_executable(test_std_ext_valarray valarray.cpp)
target_link_libraries(test_std_ext_valarray gmock_main)

set(TEST_TARGETS ${TEST_TARGETS}
  test_std_ext_valmatrix
  test_std_ext_valarray
  PARENT_SCOPE
)
