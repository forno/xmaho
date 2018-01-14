/*
BSD 2-Clause License

Copyright (c) 2017 - 2018, Doi Yusuke
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
 * @brief Utility library for std::pair<T, T> as Point.
 */

namespace xmaho::point
{

/**
 * @brief The std::pair<T, T> is Point.
 *
 * @tparam T The same type of both of pair types.
 */
template<typename T>
using Point = std::pair<T, T>;

/**
 * @brief Decrement point.first.
 *
 * @tparam T Point type.
 * @param[in] point The base point.
 * @return The point that decreamented point.first.
 *
 * @code
 * constexpr Point<int> p {0, 0};
 * constexpr auto lp {left(p)};
 * static_assert(lp.first == -1 && lp.second == 0, "xmaho::point::left(Point<int>{}) == Point<int>{-1, 0}");
 * @endcode
 */
template<typename T>
constexpr Point<T> left(Point<T> point);

/**
 * @brief Increment point.first.
 *
 * @tparam T Point type.
 * @param[in] point The base point.
 * @return The point that incremented point.first.
 *
 * @code
 * constexpr Point<int> p {0, 0};
 * constexpr auto rp {right(p)};
 * static_assert(rp.first == 1 && rp.second == 0, "xmaho::point::right(Point<int>{}) == Point<int>{1, 0}");
 * @endcode
 */
template<typename T>
constexpr Point<T> right(Point<T> point);

/**
 * @brief Decrement point.second.
 *
 * @tparam T Point type.
 * @param[in] point The base point.
 * @return The point that decreamented point.second.
 *
 * @code
 * constexpr Point<int> p {0, 0};
 * constexpr auto up_p {up(p)};
 * static_assert(up_p.first == 0 && up_p.second == -1, "xmaho::point::up(Point<int>{}) == Point<int>{0, -1}");
 * @endcode
 */
template<typename T>
constexpr Point<T> up(Point<T> point);

/**
 * @brief Increment point.second.
 *
 * @tparam T Point type.
 * @param[in] point The base point.
 * @return The point that incremented point.second.
 *
 * @code
 * constexpr Point<int> p {0, 0};
 * constexpr auto dp {down(p)};
 * static_assert(dp.first == 0 && dp.second == 1, "xmaho::point::down(Point<int>{}) == Point<int>{0, 1}");
 * @endcode
 */
template<typename T>
constexpr Point<T> down(Point<T> point);

/**
 * @brief Calculate norm of Point.
 *
 * Calculate norm of point as vector.
 * default ordinal is 2.
 *
 * if ordinal is numeric_limits<size_t>::max() then
 *   return uniform norm (that is max(first, second)).
 *
 *
 * @tparam ordinal The norm of vector space.
 * @tparam T Point type.
 * @param[in] point The pair of elements as vector.
 * @return The norm of the pair as vector.
 *
 * @code
 * constexpr Point<int> p {3, 4};
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
constexpr auto norm(const Point<T>& point);

}

#include "detail/point.hpp"

#endif
