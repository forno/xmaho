cmake_minimum_required(VERSION 3.1.3)
project(xmaho
  VERSION 0.1.0
  LANGUAGES CXX
)

set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type for only meaningful to single-configuration generators")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS RelWithDebInfo Release Debug MinSizeRel None)

option(ENABLE_TEST "Add build target for gtest" OFF)

if(ENABLE_TEST)
  include_directories(.)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
  enable_testing()
  add_subdirectory(test)
endif(ENABLE_TEST)

add_subdirectory(doxygen)

install(DIRECTORY "${PROJECT_NAME}" DESTINATION include)
