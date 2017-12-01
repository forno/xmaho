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

namespace xmaho
{
namespace std_ext
{

/**
 * @brief Return inner product "a * b"
 *
 * @pre a.size() == b.size()
 * @param[in] a lhs value.
 * @param[in] b rhs value.
 * @return The inner product by a and b.
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
 */
template<typename T>
std::valarray<T> vector_product(const std::valarray<T>& a, const std::valarray<T>& b);

/**
 * @brief Return size of vector.
 *
 * @tparam norm The norm of vector space.
 * @tparam T The value type of valarray.
 * @param[in] vector Input vector.
 * @return The vector size.
 */
template<std::size_t norm = 2, typename T>
auto distance(const std::valarray<T>& vector);

}
}

#include "detail/valarray.hpp"

#endif
