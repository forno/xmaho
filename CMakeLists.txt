cmake_minimum_required(VERSION 3.1.3)
project(xmaho
  VERSION 0.1.0
  LANGUAGES C CXX
)

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS RelWithDebInfo Release Debug MinSizeRel None)
if(NOT CMAKE_BUILD_TYPE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE RelWithDebInfo)
endif(NOT CMAKE_BUILD_TYPE)

option(ENABLE_TEST "Add build target for gtest" ON)

if(ENABLE_TEST)
  enable_testing()
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

  # Download and unpack googletest at configure time
  configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
	execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
	  RESULT_VARIABLE result
	  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
	if(result)
	  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
	endif()
	execute_process(COMMAND ${CMAKE_COMMAND} --build .
	  RESULT_VARIABLE result
	  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
	if(result)
	  message(FATAL_ERROR "Build step for googletest failed: ${result}")
	endif()

  # Add googletest directly to our build. This defines
  # the gtest and gtest_main targets.
  add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                   ${CMAKE_BINARY_DIR}/googletest-build)

  set(TEST_TARGETS
  )

  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG(-std=c++1z HAVE_FLAG_CXX17)
  if(HAVE_FLAG_CXX17)
    foreach(i IN LISTS TEST_TARGETS)
      target_compile_options(${i} PUBLIC -std=c++1z)
    endforeach(i IN LISTS TEST_TARGETS)
  else(HAVE_FLAG_CXX17)
    set_property(TARGET ${TEST_TARGETS} PROPERTY CXX_STANDARD 14)
    set_property(TARGET ${TEST_TARGETS} PROPERTY CXX_STANDARD_REQUIRED True)
    set_property(TARGET ${TEST_TARGETS} PROPERTY CXX_EXTENSIONS False)
  endif(HAVE_FLAG_CXX17)

endif(ENABLE_TEST)

install(DIRECTORY "${PROJECT_NAME}" DESTINATION "include")
