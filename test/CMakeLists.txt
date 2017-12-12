cmake_minimum_required(VERSION 3.8.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

add_subdirectory(gtest)

set(WARN_OPTIONS
  -Weverything
  -Wno-c++98-compat
  -Wno-c++98-compat-pedantic
  -Wno-global-constructors
  -Wno-missing-variable-declarations
  -Wno-padded
)
add_compile_options(
  "$<$<CXX_COMPILER_ID:Clang>:${WARN_OPTIONS}>")

# Add some test
add_subdirectory(point)
add_subdirectory(std_ext)
