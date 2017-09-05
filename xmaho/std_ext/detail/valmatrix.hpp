#ifndef XMAHO_STD_EXT_DETAIL_VALMATRIX_H
#define XMAHO_STD_EXT_DETAIL_VALMATRIX_H
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

#include "../valmatrix.hpp"

#include <cassert>
#include <stdexcept>

namespace xmaho
{
namespace std_ext
{
namespace detail
{

std::size_t size(std::pair<std::size_t, std::size_t> pos)
{
  return pos.first * pos.second;
}

std::size_t get_serial_index(
  std::pair<std::size_t, std::size_t> pos,
  std::size_t col_size)
{
  return pos.first * col_size + pos.second;
}

}
}
}

template<typename T>
xmaho::std_ext::valmatrix<T>::valmatrix(size_type row_size, size_type col_size)
  : std::valarray<T>(row_size * col_size),
    size_ {col_size ? row_size : 0, row_size ? col_size : 0}
{
}

template<typename T>
xmaho::std_ext::valmatrix<T>::valmatrix(size_type row_size, size_type col_size, std::valarray<T> values)
  : std::valarray<T>(std::move(values)),
    size_ {col_size ? row_size : 0, row_size ? col_size : 0}
{
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator=(const std::valarray<T>& rhs) &
{
  assert(size() == rhs.size());
  std::valarray<T>::operator=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator=(std::valarray<T>&& rhs) &
{
  assert(size() == rhs.size());
  std::valarray<T>::operator=(std::move(rhs));
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator=(const T& rhs) &
{
  std::valarray<T>::operator=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator=(T&& rhs) & noexcept
{
  std::valarray<T>::operator=(std::move(rhs));
  return *this;
}

template<typename T>
const T& xmaho::std_ext::valmatrix<T>::operator[](position_type position) const
{
  assert(position.first < size_.first);
  assert(position.second < size_.second);
  return std::valarray<T>::operator[](detail::get_serial_index(position, size_.second));
}

template<typename T>
T& xmaho::std_ext::valmatrix<T>::operator[](position_type position)
{
  assert(position.first < size_.first);
  assert(position.second < size_.second);
  return std::valarray<T>::operator[](detail::get_serial_index(position, size_.second));
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::valmatrix<T>::operator+() const noexcept
{
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::valmatrix<T>::operator-() const noexcept
{
  valmatrix result {size_.first, size_.second};
  result = std::valarray<T>::operator-();
  return result;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::valmatrix<T>::operator~() const noexcept
{
  valmatrix result {size_.first, size_.second};
  result = std::valarray<T>::operator~();
  return result;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator+=(const valmatrix& rhs) &
{
  std::valarray<T>::operator+=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator+=(const std::valarray<T>& rhs) &
{
  assert(size() == rhs.size());
  std::valarray<T>::operator+=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator+=(const T& rhs) &
{
  std::valarray<T>::operator+=(rhs);
  return *this;
}

template<typename T>
const T* xmaho::std_ext::valmatrix<T>::begin() const noexcept
{
  return &std::valarray<T>::operator[](0);
}

template<typename T>
T* xmaho::std_ext::valmatrix<T>::begin() noexcept
{
  return &std::valarray<T>::operator[](0);
}

template<typename T>
const T* xmaho::std_ext::valmatrix<T>::end() const noexcept
{
  return (&std::valarray<T>::operator[](size() - 1)) + 1;
}

template<typename T>
T* xmaho::std_ext::valmatrix<T>::end() noexcept
{
  return (&std::valarray<T>::operator[](size() - 1)) + 1;
}

template<typename T>
void xmaho::std_ext::valmatrix<T>::swap(valmatrix& other) noexcept
{
  valmatrix temp {std::move(other)};
  other = std::move(*this);
  *this = std::move(temp);
}

template<typename T>
const T* xmaho::std_ext::begin(const valmatrix<T>& v) noexcept
{
  return v.begin();
}

template<typename T>
T* xmaho::std_ext::begin(valmatrix<T>& v) noexcept
{
  return v.begin();
}

template<typename T>
const T* xmaho::std_ext::end(const valmatrix<T>& v) noexcept
{
  return v.end();
}

template<typename T>
T* xmaho::std_ext::end(valmatrix<T>& v) noexcept
{
  return v.end();
}

template<typename T>
void xmaho::std_ext::swap(valmatrix<T>& a, valmatrix<T>& b) noexcept
{
  a.swap(b);
}

#endif
