cmake_minimum_required(VERSION 3.1.3)
project(xmaho VERSION 0.1.0)

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS RelWithDebInfo Release Debug MinSizeRel None)
if(NOT CMAKE_BUILD_TYPE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE RelWithDebInfo)
endif(NOT CMAKE_BUILD_TYPE)

option(ENABLE_TEST "Add build target for gtest" ON)

if(ENABLE_TEST)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
  enable_testing()
  add_subdirectory(test)
endif(ENABLE_TEST)

add_subdirectory(doxygen)

install(DIRECTORY "${PROJECT_NAME}" DESTINATION include)
