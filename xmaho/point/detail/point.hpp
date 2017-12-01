#ifndef XMAHO_POINT_DETAIL_POINT_H
#define XMAHO_POINT_DETAIL_POINT_H
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

#include "../point.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

template<typename T>
constexpr xmaho::point::BasicPoint<T> xmaho::point::left(BasicPoint<T> point)
{
  --point.first;
  return point;
}

template<typename T>
constexpr xmaho::point::BasicPoint<T> xmaho::point::right(BasicPoint<T> point)
{
  ++point.first;
  return point;
}

template<typename T>
constexpr xmaho::point::BasicPoint<T> xmaho::point::up(BasicPoint<T> point)
{
  --point.second;
  return point;
}

template<typename T>
constexpr xmaho::point::BasicPoint<T> xmaho::point::down(BasicPoint<T> point)
{
  ++point.second;
  return point;
}

namespace xmaho
{
namespace point
{
namespace detail
{

template<std::size_t ordinal>
struct norm_impl
{
  static_assert(ordinal > 0, "The norm of vector space is over 0.");

  template<typename T>
  auto operator()(const BasicPoint<T>& point)
  {
    constexpr auto reciprocal {1. / ordinal};
    if constexpr (std::disjunction_v<std::is_floating_point<T>, std::is_unsigned<T>>)
      return std::pow(std::pow(point.first, ordinal) + std::pow(point.second, ordinal), reciprocal);
    else if constexpr (ordinal % 2)
      return std::pow(std::pow(std::abs(point.first), ordinal) + std::pow(std::abs(point.second), ordinal), reciprocal);
    else
      return std::pow(std::pow(point.first, ordinal) + std::pow(point.second, ordinal), reciprocal);

  }
};

template<>
struct norm_impl<1u>
{
  template<typename T>
  auto operator()(const BasicPoint<T>& point)
  {
    if constexpr (std::is_unsigned_v<T>) {
      return point.first + point.second;
    } else {
      return std::abs(point.first) + std::abs(point.second);
    }
  }
};

template<>
struct norm_impl<2u>
{
  template<typename T>
  auto operator()(const BasicPoint<T>& point)
  {
    return std::hypot(point.first, point.second);
  }
};

template<>
struct norm_impl<3u>
{
  template<typename T>
  auto operator()(const BasicPoint<T>& point)
  {
    [[deprecated]]
    const auto [a, b] {[&]() -> BasicPoint<T> {
      if constexpr (std::is_unsigned_v<T>) {
        return {point.first, point.second};
      } else {
        return {std::abs(point.first), std::abs(point.second)};
      }
    }()};
    return std::cbrt(std::pow(a, 3) + std::pow(b, 3));
  }
};


template<>
struct norm_impl<std::numeric_limits<std::size_t>::max()>
{
  template<typename T>
  auto operator()(const BasicPoint<T>& point)
  {
    if constexpr (std::is_unsigned_v<T>) {
      return std::max(point.first, point.second);
    } else {
      return std::max(std::abs(point.first), std::abs(point.second));
    }
  }
};

}
}
}

template<std::size_t ordinal, typename T>
constexpr auto xmaho::point::norm(const BasicPoint<T>& point)
{
  return detail::norm_impl<ordinal>{}(point);
}

#endif
