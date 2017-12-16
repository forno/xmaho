#ifndef XMAHO_STD_EXT_DETAIL_VALARRAY_H
#define XMAHO_STD_EXT_DETAIL_VALARRAY_H
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

#include "../valarray.hpp"

#include <cmath>
#include <limits>
#include <type_traits>

template<typename T>
inline T xmaho::std_ext::inner_product(const std::valarray<T>& a, const std::valarray<T>& b)
{
  return (a * b).sum();
}

template<typename T>
inline std::valarray<T> xmaho::std_ext::vector_product(const std::valarray<T>& a, const std::valarray<T>& b)
{
  return a.cshift(1) * b.cshift(-1) - a.cshift(-1) * b.cshift(1);
}

namespace xmaho
{
namespace std_ext
{
namespace detail
{

template<std::size_t ordinal>
struct norm_impl
{
  static_assert(ordinal > 0, "The norm of vector space is over 0.");

  template<typename T>
  auto operator()(const std::valarray<T>& vector)
  {
    constexpr auto reciprocal {1. / ordinal};
    if constexpr (std::is_unsigned_v<T>)
      return std::pow(std::pow<T>(vector, ordinal).sum(), reciprocal);
    else if constexpr (ordinal % 2)
      return std::pow(std::pow<T>(std::abs(vector), ordinal).sum(), reciprocal);
    else
      return std::pow(std::pow<T>(vector, ordinal).sum(), reciprocal);
  }
};

template<>
struct norm_impl<1u>
{
  template<typename T>
  auto operator()(const std::valarray<T>& vector)
  {
    if constexpr (std::is_unsigned_v<T>)
      return vector.sum();
    else
      return std::abs(vector).sum();
  }
};

template<>
struct norm_impl<2u>
{
  template<typename T>
  auto operator()(const std::valarray<T>& vector)
  {
    return std::sqrt((vector * vector).sum());
  }
};

template<>
struct norm_impl<3u>
{
  template<typename T>
  auto operator()(const std::valarray<T>& vector)
  {
    if constexpr (std::is_unsigned_v<T>)
      return std::cbrt((vector * vector * vector).sum());
    else {
      const auto abs_vec {std::abs(vector)};
      return std::cbrt((abs_vec * abs_vec * abs_vec).sum());
    }
  }
};

template<>
struct norm_impl<std::numeric_limits<std::size_t>::max()>
{
  template<typename T>
  auto operator()(const std::valarray<T>& vector)
  {
    if constexpr (std::is_unsigned_v<T>)
      return vector.max();
    else
      return std::abs(vector).max();
  }
};

}
}
}

template<std::size_t ordinal, typename T>
auto xmaho::std_ext::norm(const std::valarray<T>& vector)
{
  return detail::norm_impl<ordinal>{}(vector);
}

template<typename T>
T average(const std::valarray<T>& vector)
{
  return vector.sum() / vector.size();
}

#endif
