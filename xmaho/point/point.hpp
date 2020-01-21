/*
BSD 2-Clause License

Copyright (c) 2017 - 2020, FORNO
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

#ifndef XMAHO_POINT_POINT_H
#define XMAHO_POINT_POINT_H

#include <cstddef>
#include <utility>

/**
 * @file point/point.hpp
 * @brief Utility library for std::pair<T, T> as point.
 */

namespace xmaho::point
{

/**
 * @brief The std::pair<T, T> is point.
 *
 * @tparam T The same type of both of pair types.
 */
template<typename T>
using point = std::pair<T, T>;

/**
 * @brief Decrement point.first.
 *
 * @tparam T point type.
 * @param[in] p The base point.
 * @return The point that decreamented point.first.
 *
 * @code
 * constexpr point<int> p {0, 0};
 * constexpr auto lp {left(p)};
 * static_assert(lp.first == -1 && lp.second == 0, "xmaho::point::left(point<int>{}) == point<int>{-1, 0}");
 * @endcode
 */
template<typename T>
constexpr point<T> left(point<T> p);

/**
 * @brief Increment point.first.
 *
 * @tparam T point type.
 * @param[in] p The base point.
 * @return The point that incremented point.first.
 *
 * @code
 * constexpr point<int> p {0, 0};
 * constexpr auto rp {right(p)};
 * static_assert(rp.first == 1 && rp.second == 0, "xmaho::point::right(point<int>{}) == point<int>{1, 0}");
 * @endcode
 */
template<typename T>
constexpr point<T> right(point<T> p);

/**
 * @brief Decrement point.second.
 *
 * @tparam T point type.
 * @param[in] p The base point.
 * @return The point that decreamented point.second.
 *
 * @code
 * constexpr point<int> p {0, 0};
 * constexpr auto up_p {up(p)};
 * static_assert(up_p.first == 0 && up_p.second == -1, "xmaho::point::up(point<int>{}) == point<int>{0, -1}");
 * @endcode
 */
template<typename T>
constexpr point<T> up(point<T> p);

/**
 * @brief Increment point.second.
 *
 * @tparam T point type.
 * @param[in] p The base point.
 * @return The point that incremented point.second.
 *
 * @code
 * constexpr point<int> p {0, 0};
 * constexpr auto dp {down(p)};
 * static_assert(dp.first == 0 && dp.second == 1, "xmaho::point::down(point<int>{}) == point<int>{0, 1}");
 * @endcode
 */
template<typename T>
constexpr point<T> down(point<T> p);

/**
 * @brief Calculate norm of point.
 *
 * Calculate norm of point as vector.
 * default ordinal is 2.
 *
 * if ordinal is numeric_limits<size_t>::max() then
 *   return uniform norm (that is max(first, second)).
 *
 *
 * @tparam ordinal The norm of vector space.
 * @tparam T point type.
 * @param[in] p The pair of elements as vector.
 * @return The norm of the pair as vector.
 *
 * @code
 * constexpr point<int> p {3, 4};
 * const auto euclidean_distance {norm(p)};
 * static_assert(is_same_v<const double, decltype(euclidean_distance)>, "norm on ordinal 2 return floting point type");
 * assert(abs(euclidean_distance - 5) < std::numeric_limits<decltype(euclidean_distance)>::epsilon());
 *
 * const auto taxicab_distance {norm<1>(p)};
 * static_assert(is_same_v<const int, decltype(taxicab_distance)>, "norm on ordinal 1 return same as point value type");
 * assert(taxicab_distance == 7);
 *
 * const auto uniform_norm {norm<numeric_limits<size_t>::max()>(p)};
 * static_assert(is_same_v<const int, decltype(uniform_norm)>, "norm on ordinal max return same as point value type");
 * assert(uniform_norm == 4);
 * @endcode
 */
template<std::size_t ordinal = 2, typename T>
constexpr auto norm(const point<T>& p);

}

#include "detail/point.hpp"

#endif
