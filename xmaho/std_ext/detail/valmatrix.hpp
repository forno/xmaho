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

namespace xmaho
{
namespace std_ext
{
namespace detail
{

constexpr std::size_t get_serial_index(
  std::pair<std::size_t, std::size_t> pos,
  std::size_t col_size)
{
  return col_size * pos.second + pos.first;
}

constexpr std::pair<std::size_t, std::size_t> get_init_size(std::size_t row_size, std::size_t col_size)
{
  return {col_size ? row_size : 0 , row_size ? col_size : 0};
}

}
}
}

template<typename T>
xmaho::std_ext::valmatrix<T>::valmatrix(size_type row_size, size_type col_size)
  : std::valarray<T>(row_size * col_size),
    size_ {detail::get_init_size(row_size, col_size)}
{
}

template<typename T>
xmaho::std_ext::valmatrix<T>::valmatrix(const T& value, size_type row_size, size_type col_size)
  : std::valarray<T>(value, row_size * col_size),
    size_ {detail::get_init_size(row_size, col_size)}
{
}

template<typename T>
xmaho::std_ext::valmatrix<T>::valmatrix(const std::valarray<T>& values, size_type row_size, size_type col_size)
  : std::valarray<T>(values.size() == row_size * col_size ? values : std::valarray<T>(row_size * col_size)),
    size_ {detail::get_init_size(row_size, col_size)}
{
}

template<typename T>
xmaho::std_ext::valmatrix<T>::valmatrix(std::valarray<T>&& values, size_type row_size, size_type col_size)
  : std::valarray<T>(values.size() == row_size * col_size ? std::move(values) : std::valarray<T>(row_size * col_size)),
    size_ {detail::get_init_size(row_size, col_size)}
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
const T& xmaho::std_ext::valmatrix<T>::operator[](position_type position) const
{
  assert(position.first < row_size());
  assert(position.second < col_size());
  return std::valarray<T>::operator[](detail::get_serial_index(position, col_size()));
}

template<typename T>
T& xmaho::std_ext::valmatrix<T>::operator[](position_type position)
{
  assert(position.first < row_size());
  assert(position.second < col_size());
  return std::valarray<T>::operator[](detail::get_serial_index(position, col_size()));
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::valmatrix<T>::operator+() const noexcept
{
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::valmatrix<T>::operator-() const noexcept
{
  return valmatrix{std::valarray<T>::operator-(), row_size(), col_size()};
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::valmatrix<T>::operator~() const noexcept
{
  return valmatrix{std::valarray<T>::operator~(), row_size(), col_size()};
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator+=(const valmatrix& rhs) &
{
  assert(rhs.size_ == size_);
  std::valarray<T>::operator+=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator+=(const std::valarray<T>& rhs) &
{
  assert(rhs.size() == size());
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
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator-=(const valmatrix& rhs) &
{
  assert(rhs.size_ == size_);
  std::valarray<T>::operator-=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator-=(const std::valarray<T>& rhs) &
{
  assert(rhs.size() == size());
  std::valarray<T>::operator-=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator-=(const T& rhs) &
{
  std::valarray<T>::operator-=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator*=(const valmatrix& rhs) &
{
  assert(rhs.size_ == size_);
  std::valarray<T>::operator*=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator*=(const std::valarray<T>& rhs) &
{
  assert(rhs.size() == size());
  std::valarray<T>::operator*=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator*=(const T& rhs) &
{
  std::valarray<T>::operator*=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator/=(const valmatrix& rhs) &
{
  assert(rhs.size_ == size_);
  std::valarray<T>::operator/=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator/=(const std::valarray<T>& rhs) &
{
  assert(rhs.size() == size());
  std::valarray<T>::operator/=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator/=(const T& rhs) &
{
  std::valarray<T>::operator/=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator%=(const valmatrix& rhs) &
{
  assert(rhs.size_ == size_);
  std::valarray<T>::operator%=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator%=(const std::valarray<T>& rhs) &
{
  assert(rhs.size() == size());
  std::valarray<T>::operator%=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator%=(const T& rhs) &
{
  std::valarray<T>::operator%=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator&=(const valmatrix& rhs) &
{
  assert(rhs.size_ == size_);
  std::valarray<T>::operator&=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator&=(const std::valarray<T>& rhs) &
{
  assert(rhs.size() == size());
  std::valarray<T>::operator&=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator&=(const T& rhs) &
{
  std::valarray<T>::operator&=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator|=(const valmatrix& rhs) &
{
  assert(rhs.size_ == size_);
  std::valarray<T>::operator|=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator|=(const std::valarray<T>& rhs) &
{
  assert(rhs.size() == size());
  std::valarray<T>::operator|=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator|=(const T& rhs) &
{
  std::valarray<T>::operator|=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator^=(const valmatrix& rhs) &
{
  assert(rhs.size_ == size_);
  std::valarray<T>::operator^=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator^=(const std::valarray<T>& rhs) &
{
  assert(rhs.size() == size());
  std::valarray<T>::operator^=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator^=(const T& rhs) &
{
  std::valarray<T>::operator^=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator<<=(const valmatrix& rhs) &
{
  assert(rhs.size_ == size_);
  std::valarray<T>::operator<<=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator<<=(const std::valarray<T>& rhs) &
{
  assert(rhs.size() == size());
  std::valarray<T>::operator<<=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator<<=(const T& rhs) &
{
  std::valarray<T>::operator<<=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator>>=(const valmatrix& rhs) &
{
  assert(rhs.size_ == size_);
  std::valarray<T>::operator>>=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator>>=(const std::valarray<T>& rhs) &
{
  assert(rhs.size() == size());
  std::valarray<T>::operator>>=(rhs);
  return *this;
}

template<typename T>
xmaho::std_ext::valmatrix<T>& xmaho::std_ext::valmatrix<T>::operator>>=(const T& rhs) &
{
  std::valarray<T>::operator>>=(rhs);
  return *this;
}

template<typename T>
typename xmaho::std_ext::valmatrix<T>::size_type xmaho::std_ext::valmatrix<T>::row_size() const noexcept
{
  return size_.first;
}

template<typename T>
typename xmaho::std_ext::valmatrix<T>::size_type xmaho::std_ext::valmatrix<T>::col_size() const noexcept
{
  return size_.second;
}

template<typename T>
std::valarray<T> xmaho::std_ext::valmatrix<T>::row(size_type index) const
{
  assert(index < col_size());
  return std::valarray<T>::operator[](std::slice{index * row_size(), row_size(), 1});
}

template<typename T>
std::slice_array<T> xmaho::std_ext::valmatrix<T>::row(size_type index)
{
  assert(index < col_size());
  return std::valarray<T>::operator[](std::slice{index * row_size(), row_size(), 1});
}

template<typename T>
std::valarray<T> xmaho::std_ext::valmatrix<T>::col(size_type index) const
{
  assert(index < row_size());
  return std::valarray<T>::operator[](std::slice{index, col_size(), row_size()});
}

template<typename T>
std::slice_array<T> xmaho::std_ext::valmatrix<T>::col(size_type index)
{
  assert(index < row_size());
  return std::valarray<T>::operator[](std::slice{index, col_size(), row_size()});
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::valmatrix<T>::block(position_type pos, position_type size) const
{
  assert(pos.first + (size.first - 1) < row_size());
  assert(pos.second + (size.second - 1) < col_size());
  const std::gslice block_gslice {detail::get_serial_index(pos, col_size()), {size.second, size.first}, {row_size(), 1}};
  return valmatrix{std::valarray<T>::operator[](block_gslice), size.first, size.second};
}

template<typename T>
const auto xmaho::std_ext::valmatrix<T>::begin() const noexcept
{
  return std::begin(static_cast<const std::valarray<T>&>(*this));
}

template<typename T>
auto xmaho::std_ext::valmatrix<T>::begin() noexcept
{
  return std::begin(static_cast<std::valarray<T>&>(*this));
}

template<typename T>
const auto xmaho::std_ext::valmatrix<T>::end() const noexcept
{
  return std::end(static_cast<const std::valarray<T>&>(*this));
}

template<typename T>
auto xmaho::std_ext::valmatrix<T>::end() noexcept
{
  return std::end(static_cast<std::valarray<T>&>(*this));
}

template<typename T>
void xmaho::std_ext::valmatrix<T>::swap(valmatrix& other) noexcept
{
  valmatrix temp {std::move(other)};
  other = std::move(*this);
  *this = std::move(temp);
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator+(valmatrix<T> lhs, const valmatrix<T>& rhs)
{
  return lhs += rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator+(valmatrix<T> lhs, const std::valarray<T>& rhs)
{
  return lhs += rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator+(valmatrix<T> lhs, const T& rhs)
{
  return lhs += rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator+(const std::valarray<T>& lhs, valmatrix<T> rhs)
{
  return rhs += lhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator+(const T& lhs, valmatrix<T> rhs)
{
  return rhs += lhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator-(valmatrix<T> lhs, const valmatrix<T>& rhs)
{
  return lhs -= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator-(valmatrix<T> lhs, const std::valarray<T>& rhs)
{
  return lhs -= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator-(valmatrix<T> lhs, const T& rhs)
{
  return lhs -= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator-(const std::valarray<T>& lhs, const valmatrix<T>& rhs)
{
  valmatrix<T> tmp {lhs, rhs.row_size(), rhs.col_size()};
  return tmp -= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator-(const T& lhs, const valmatrix<T>& rhs)
{
  valmatrix<T> tmp {lhs, rhs.row_size(), rhs.col_size()};
  return tmp -= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator*(valmatrix<T> lhs, const valmatrix<T>& rhs)
{
  return lhs *= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator*(valmatrix<T> lhs, const std::valarray<T>& rhs)
{
  return lhs *= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator*(valmatrix<T> lhs, const T& rhs)
{
  return lhs *= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator*(const std::valarray<T>& lhs, valmatrix<T> rhs)
{
  return rhs *= lhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator*(const T& lhs, valmatrix<T> rhs)
{
  return rhs *= lhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator/(valmatrix<T> lhs, const valmatrix<T>& rhs)
{
  return lhs /= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator/(valmatrix<T> lhs, const std::valarray<T>& rhs)
{
  return lhs /= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator/(valmatrix<T> lhs, const T& rhs)
{
  return lhs /= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator/(const std::valarray<T>& lhs, const valmatrix<T>& rhs)
{
  valmatrix<T> tmp {lhs, rhs.row_size(), rhs.col_size()};
  return tmp /= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator/(const T& lhs, const valmatrix<T>& rhs)
{
  valmatrix<T> tmp {lhs, rhs.row_size(), rhs.col_size()};
  return tmp /= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator%(valmatrix<T> lhs, const valmatrix<T>& rhs)
{
  return lhs %= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator%(valmatrix<T> lhs, const std::valarray<T>& rhs)
{
  return lhs %= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator%(valmatrix<T> lhs, const T& rhs)
{
  return lhs %= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator%(const std::valarray<T>& lhs, const valmatrix<T>& rhs)
{
  valmatrix<T> tmp {lhs, rhs.row_size(), rhs.col_size()};
  return tmp %= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator%(const T& lhs, const valmatrix<T>& rhs)
{
  valmatrix<T> tmp {lhs, rhs.row_size(), rhs.col_size()};
  return tmp %= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator&(valmatrix<T> lhs, const valmatrix<T>& rhs)
{
  return lhs &= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator&(valmatrix<T> lhs, const std::valarray<T>& rhs)
{
  return lhs &= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator&(valmatrix<T> lhs, const T& rhs)
{
  return lhs &= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator&(const std::valarray<T>& lhs, valmatrix<T> rhs)
{
  return rhs &= lhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator&(const T& lhs, valmatrix<T> rhs)
{
  return rhs &= lhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator|(valmatrix<T> lhs, const valmatrix<T>& rhs)
{
  return lhs |= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator|(valmatrix<T> lhs, const std::valarray<T>& rhs)
{
  return lhs |= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator|(valmatrix<T> lhs, const T& rhs)
{
  return lhs |= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator|(const std::valarray<T>& lhs, valmatrix<T> rhs)
{
  return rhs |= lhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator|(const T& lhs, valmatrix<T> rhs)
{
  return rhs |= lhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator^(valmatrix<T> lhs, const valmatrix<T>& rhs)
{
  return lhs ^= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator^(valmatrix<T> lhs, const std::valarray<T>& rhs)
{
  return lhs ^= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator^(valmatrix<T> lhs, const T& rhs)
{
  return lhs ^= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator^(const std::valarray<T>& lhs, valmatrix<T> rhs)
{
  return rhs ^= lhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator^(const T& lhs, valmatrix<T> rhs)
{
  return rhs ^= lhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator<<(valmatrix<T> lhs, const valmatrix<T>& rhs)
{
  return lhs <<= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator<<(valmatrix<T> lhs, const std::valarray<T>& rhs)
{
  return lhs <<= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator<<(valmatrix<T> lhs, const T& rhs)
{
  return lhs <<= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator<<(const std::valarray<T>& lhs, const valmatrix<T>& rhs)
{
  valmatrix<T> tmp {lhs, rhs.row_size(), rhs.col_size()};
  return tmp <<= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator<<(const T& lhs, const valmatrix<T>& rhs)
{
  valmatrix<T> tmp {lhs, rhs.row_size(), rhs.col_size()};
  return tmp <<= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator>>(valmatrix<T> lhs, const valmatrix<T>& rhs)
{
  return lhs >>= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator>>(valmatrix<T> lhs, const std::valarray<T>& rhs)
{
  return lhs >>= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator>>(valmatrix<T> lhs, const T& rhs)
{
  return lhs >>= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator>>(const std::valarray<T>& lhs, const valmatrix<T>& rhs)
{
  valmatrix<T> tmp {lhs, rhs.row_size(), rhs.col_size()};
  return tmp >>= rhs;
}

template<typename T>
xmaho::std_ext::valmatrix<T> xmaho::std_ext::operator>>(const T& lhs, const valmatrix<T>& rhs)
{
  valmatrix<T> tmp {lhs, rhs.row_size(), rhs.col_size()};
  return tmp >>= rhs;
}

template<typename T>
const auto xmaho::std_ext::begin(const valmatrix<T>& v) noexcept
{
  return v.begin();
}

template<typename T>
auto xmaho::std_ext::begin(valmatrix<T>& v) noexcept
{
  return v.begin();
}

template<typename T>
const auto xmaho::std_ext::end(const valmatrix<T>& v) noexcept
{
  return v.end();
}

template<typename T>
auto xmaho::std_ext::end(valmatrix<T>& v) noexcept
{
  return v.end();
}

template<typename T>
void xmaho::std_ext::swap(valmatrix<T>& a, valmatrix<T>& b) noexcept
{
  a.swap(b);
}

#endif
