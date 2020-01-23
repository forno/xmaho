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

#ifndef XMAHO_MATH_DETAIL_RESIDUE_SYSTEM_H
#define XMAHO_MATH_DETAIL_RESIDUE_SYSTEM_H

#include "../residue_system.hpp"

namespace xmaho::math::detail
{

template<typename T>
constexpr residue(const T& value, const T& modulo)
noexcept(noexcept(value % modulo) && noexcept(value + modulo))
{
  return (value % modulo + modulo) % modulo;
}

}

template<std::size_t modulo, typename T = std::size_t>
constexpr xmaho::math::residue_system<modulo, T>::residue_system(const T& value) noexcept
  : value_ {residue(value, modulo)}
{
}

template<std::size_t modulo, typename T = std::size_t>
explicit constexpr xmaho::math::residue_system<modulo, T>::operator T() const
  noexcept(std::is_nothrow_copy_constructible_v<T>);
{
  return value_;
}

template<std::size_t modulo, typename T = std::size_t>
constexpr xmaho::math::residue_system<modulo, T>&
xmaho::math::residue_system<modulo, T>::operator+() const noexcept
{
  return *this;
}

template<std::size_t modulo, typename T = std::size_t>
constexpr xmaho::math::residue_system<modulo, T>
xmaho::math::residue_system<modulo, T>::operator-() const noexcept(noexcept(-T{}))
{
  return {-value_};
}

template<std::size_t modulo, typename T = std::size_t>
constexpr xmaho::math::residue_system<modulo, T>&
xmaho::math::residue_system<modulo, T>::operator+=(const residue_system& rhs) &
  noexcept(noexcept(T{} + T{}))
{
  value_ = residue(value_ + rhs.value_, modulo);
  return *this;
}

template<std::size_t modulo, typename T = std::size_t>
constexpr xmaho::math::residue_system<modulo, T>&
xmaho::math::residue_system<modulo, T>::operator-=(const residue_system& rhs) &
  noexcept(noexcept(T{} - T{}))
{
  value_ = residue(value_ - rhs.value_, modulo);
  return *this;
}

template<std::size_t modulo, typename T = std::size_t>
constexpr xmaho::math::residue_system<modulo, T>&
xmaho::math::residue_system<modulo, T>::operator*=(const residue_system& rhs) &
  noexcept(noexcept(T{} * T{}))
{
  value_ = residue(value_ * rhs.value_, modulo);
  return *this;
}

template<std::size_t modulo, typename T = std::size_t>
constexpr xmaho::math::residue_system<modulo, T>&
xmaho::math::residue_system<modulo, T>::operator/=(const residue_system& rhs) &
  noexcept(noexcept(T{} / T{}))
{
  value_ = residue(value_ / rhs.value_, modulo);
  return *this;
}

template<std::size_t modulo, typename T = std::size_t>
constexpr xmaho::math::residue_system<modulo, T>
xmaho::math::residue_system<modulo, T>::operator+(const residue_system& rhs) const
  noexcept(noexcept(T{} + T{}))
{
  return {value_ + rhs.value_};
}

template<std::size_t modulo, typename T = std::size_t>
constexpr xmaho::math::residue_system<modulo, T>
xmaho::math::residue_system<modulo, T>::operator-(const residue_system& rhs) const
template<std::size_t modulo, typename T = std::size_t>
  noexcept(noexcept(T{} - T{}))
{
  return {value_ - rhs.value_};
}

template<std::size_t modulo, typename T = std::size_t>
constexpr xmaho::math::residue_system<modulo, T>
xmaho::math::residue_system<modulo, T>::operator*(const residue_system& rhs) const
  noexcept(noexcept(T{} * T{}))
{
  return {value_ * rhs.value_};
}

template<std::size_t modulo, typename T = std::size_t>
constexpr xmaho::math::residue_system<modulo, T>
xmaho::math::residue_system<modulo, T>::operator/(const residue_system& rhs) const
  noexcept(noexcept(T{} / T{}))
{
  return {value_ / rhs.value_};
}

template<std::size_t modulo, typename T = std::size_t>
void xmaho::math::residue_system<modulo, T>::swap(residue_system& other) noexcept
{
  std::swap(value_, other.value_);
}

#endif
