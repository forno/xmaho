#ifndef XMAHO_POINT_POINT_H
#define XMAHO_POINT_POINT_H
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
 * @file point/point.hpp
 * @brief Utility library for std::pair<T, T> as Point.
 */

#include <cstddef>
#include <type_traits>
#include <utility>

namespace xmaho
{
namespace point
{

/**
 * @brief The std::pair<T, T> is Point.
 *
 * @tparam T The same type of both of pair types.
 */
template<typename T>
using BasicPoint = std::pair<T, T>;

//! @brief BasicPoint<int> for normal template specialization.
using Point = BasicPoint<int>;
//! @brief BasicPoint<unsinged int> for unsigned template specialization.
using UPoint = BasicPoint<unsigned int>;
//! @brief BasicPoint<std::size_t> for size template specialization.
using SizePoint = BasicPoint<std::size_t>;

/**
 * @brief Decrement point.first.
 *
 * @tparam T Point type.
 * @param[in] point The Base point.
 * @return The point that decreamented point.first.
 */
template<typename T>
constexpr BasicPoint<T> left(BasicPoint<T> point);

/**
 * @brief Decrement point.first.
 *
 * @tparam T Point type.
 * @param[in] point The Base point.
 * @return The point that incremented point.first.
 */
template<typename T>
constexpr BasicPoint<T> right(BasicPoint<T> point);

/**
 * @brief Decrement point.second.
 *
 * @tparam T Point type.
 * @param[in] point The Base point.
 * @return The point that decreamented point.second.
 */
template<typename T>
constexpr BasicPoint<T> up(BasicPoint<T> point);

/**
 * @brief Decrement point.second.
 *
 * @tparam T Point type.
 * @param[in] point The Base point.
 * @return The point that incremented point.second.
 */
template<typename T>
constexpr BasicPoint<T> down(BasicPoint<T> point);

}
}

#include "detail/point.hpp"

#endif
