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

namespace xmaho
{
namespace std_ext
{
namespace detail
{

template<std::size_t norm>
struct distance_impl
{
  template<typename T>
  T operator()(const std::valarray<T>& vector)
  {
    return std::pow(std::pow(vector, norm).sum(), 1. / norm);
  }
};

template<>
template<typename T>
T distance_impl<1u>::operator()(const std::valarray<T>& vector)
{
  return std::abs(vector).sum();
}

template<>
template<typename T>
T distance_impl<2u>::operator()(const std::valarray<T>& vector)
{
  return std::sqrt((vector * vector).sum());
}

template<>
template<typename T>
T distance_impl<3u>::operator()(const std::valarray<T>& vector)
{
  return std::cbrt((vector * vector * vector).sum());
}

template<>
template<typename T>
T distance_impl<std::numeric_limits<std::size_t>::max()>::operator()(const std::valarray<T>& vector)
{
  return std::abs(vector).max();
}

}
}
}

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

template<std::size_t norm, typename T>
T xmaho::std_ext::distance(const std::valarray<T>& vector)
{
  return detail::distance_impl<norm>{}(vector);
}

#endif
