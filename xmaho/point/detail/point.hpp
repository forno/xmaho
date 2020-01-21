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

#ifndef XMAHO_POINT_DETAIL_POINT_H
#define XMAHO_POINT_DETAIL_POINT_H

#include "../point.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

template<typename T>
constexpr xmaho::point::point<T> xmaho::point::left(point<T> p)
{
  --p.first;
  return p;
}

template<typename T>
constexpr xmaho::point::point<T> xmaho::point::right(point<T> p)
{
  ++p.first;
  return p;
}

template<typename T>
constexpr xmaho::point::point<T> xmaho::point::up(point<T> p)
{
  --p.second;
  return p;
}

template<typename T>
constexpr xmaho::point::point<T> xmaho::point::down(point<T> p)
{
  ++p.second;
  return p;
}

namespace xmaho::point::detail
{

template<std::size_t ordinal>
struct norm_impl
{
  static_assert(ordinal > 0, "The norm of vector space is over 0.");

  template<typename T>
  constexpr auto operator()(const point<T>& p) const
  {
    constexpr auto reciprocal {1. / ordinal};
    if constexpr (std::is_unsigned_v<T>)
      return std::pow(std::pow(p.first, ordinal) + std::pow(p.second, ordinal), reciprocal);
    else if constexpr (ordinal % 2)
      return std::pow(std::pow(std::abs(p.first), ordinal) + std::pow(std::abs(p.second), ordinal), reciprocal);
    else
      return std::pow(std::pow(p.first, ordinal) + std::pow(p.second, ordinal), reciprocal);

  }
};

template<>
struct norm_impl<1u>
{
  template<typename T>
  constexpr auto operator()(const point<T>& p) const
  {
    if constexpr (std::is_unsigned_v<T>) {
      return p.first + p.second;
    } else {
      return std::abs(p.first) + std::abs(p.second);
    }
  }
};

template<>
struct norm_impl<2u>
{
  template<typename T>
  constexpr auto operator()(const point<T>& p) const
  {
    return std::hypot(p.first, p.second);
  }
};

template<>
struct norm_impl<3u>
{
  template<typename T>
  constexpr auto operator()(const point<T>& p) const
  {
    if constexpr (std::is_unsigned_v<T>) {
      return std::cbrt(std::pow(p.first, 3) + std::pow(p.second, 3));
    } else {
      return std::cbrt(std::pow(std::abs(p.first), 3) + std::pow(std::abs(p.second), 3));
    }
  }
};


template<>
struct norm_impl<std::numeric_limits<std::size_t>::max()>
{
  template<typename T>
  constexpr auto operator()(const point<T>& p) const
  {
    if constexpr (std::is_unsigned_v<T>) {
      return std::max(p.first, p.second);
    } else {
      return std::max(std::abs(p.first), std::abs(p.second));
    }
  }
};

}

template<std::size_t ordinal, typename T>
constexpr auto xmaho::point::norm(const point<T>& p)
{
  return detail::norm_impl<ordinal>{}(p);
}

#endif
