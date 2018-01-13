#ifndef XMAHO_STD_EXT_VALARRAY_H
#define XMAHO_STD_EXT_VALARRAY_H
/*
BSD 2-Clause License

Copyright (c) 2017, Doi Yusuke
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * @file std_ext/valarray.hpp
 * @brief The utility functions for std::valarray.
 */

#include <cstddef>
#include <valarray>

namespace xmaho::std_ext
{

/**
 * @brief Return inner product "a * b"
 *
 * @pre a.size() == b.size()
 * @param[in] a lhs value.
 * @param[in] b rhs value.
 * @return The inner product by a and b.
 *
 * @code
 * const valarray<int> a {2, 4, 6};
 * cosnt valarray<int> b {1, 3, 5};
 * const auto result {inner_product(a, b)}
 * assert(44 == result);
 * @endcode
 */
template<typename T>
T inner_product(const std::valarray<T>& a, const std::valarray<T>& b);

/**
 * @brief Return vector product "a cross b"
 *
 * @pre a.size() == b.size()
 * @param[in] a lhs value.
 * @param[in] b rhs value.
 * @return The vector product by a and b.
 *
 * @code
 * const valarray<int> a {2, 4, 6};
 * cosnt valarray<int> b {1, 3, 5};
 * const auto result {vector_product(a, b)}
 * const auto correct {2, -4, 2};
 * assert(equal(begin(correct), end(correct), begin(result), end(result)));
 * @endcode
 */
template<typename T>
std::valarray<T> vector_product(const std::valarray<T>& a, const std::valarray<T>& b);

/**
 * @brief Return size of vector.
 *
 * @tparam ordinal The norm of vector space.
 * @tparam T The value type of valarray.
 * @param[in] vector Input vector.
 * @return The vector size.
 *
 * @code
 * const valarray<int> v {1, -2, 2};
 * const auto euclidean_distance {norm(v)};
 * static_assert(is_same_v<const double, decltype(euclidean_distance)>, "norm on ordinal 2 return floting point type");
 * assert(euclidean_distance == 3.); // It is correct operation with "==" because result is interger.
 *
 * const auto taxicab_distance {norm<1>(v)};
 * static_assert(is_same_v<const int, decltype(taxicab_distance)>, "norm on ordinal 1 return same as value type");
 * assert(taxicab_distance == 5);
 *
 * const auto uniform_norm {norm<numeric_limits<size_t>::max()>(v)};
 * static_assert(is_same_v<const int, decltype(uniform_norm)>, "norm on ordinal max return same as value type");
 * assert(uniform_norm == 2);
 * @endcode
 */
template<std::size_t ordinal = 2, typename T>
auto norm(const std::valarray<T>& vector);

}

#include "detail/valarray.hpp"

#endif
