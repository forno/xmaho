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

#ifndef XMAHO_MATH_RESIDUE_SYSTEM_H
#define XMAHO_MATH_RESIDUE_SYSTEM_H

#include <cstddef>
#include <type_traits>
#include <utility>

/**
 * @file math/residue_system.hpp
 * @brief The redidue system functors.
 */

namespace xmaho::math
{

/**
 * @brief The caliculate type on complite redidue system.
 *
 * @pre modulo > 0
 * @tparam modulo The modulo number.
 * @tparam T The value type.
 */
template<std::size_t modulo, typename T = std::size_t>
class residue_system
{
  static_assert(modulo > 0, "Modulo must be over 0");
  static_assert(std::is_nothrow_default_constructible_v<T>, "T must can be make noexcept construct");
  static_assert(std::is_nothrow_move_constructible_v<T> ||
                std::is_nothrow_copy_constructible_v<T>, "T must can be maked by T");
  static_assert(noexcept(T{} + T{}), "T must that can be noexcept addition by T");
  static_assert(noexcept(T{} % T{}), "T must that can be noexcept dividion by T");
public:
  //! @brief The value type.
  using value_type = T;

  //!  @brief Default constructor with T{};
  constexpr residue_system() = default;

  /**
   * @brief Construct by first value.
   *
   * @param[in] value Initalization value.
   */
  constexpr residue_system(const T& value) noexcept;

  //!  @brief Convert function to T.
  explicit constexpr operator T() const
    noexcept(std::is_nothrow_copy_constructible_v<T>);

  /**
   * @brief Unary addition.
   *
   * @return This reference.
   */
  constexpr residue_system& operator+() const noexcept;

  /**
   * @brief Unary minus.
   *
   * @return New value.
   */
  constexpr residue_system operator-() const noexcept(noexcept(-T{}));

  /**
   * @brief Assign combine addition.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  constexpr residue_system& operator+=(const residue_system& rhs) &
    noexcept(noexcept(T{} + T{}));

  /**
   * @brief Assign combine subtraction.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  constexpr residue_system& operator-=(const residue_system& rhs) &
    noexcept(noexcept(T{} - T{}));

  /**
   * @brief Assign combine multiplication.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  constexpr residue_system& operator*=(const residue_system& rhs) &
    noexcept(noexcept(T{} * T{}));

  /**
   * @brief Assign combine divition.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  constexpr residue_system& operator/=(const residue_system& rhs) &
    noexcept(noexcept(T{} / T{}));

  /**
   * @brief Addition.
   *
   * @param[in] rhs Value.
   * @return New value.
   */
  constexpr residue_system operator+(const residue_system& rhs) const
    noexcept(noexcept(T{} + T{}));

  /**
   * @brief Subtraction.
   *
   * @param[in] rhs Value.
   * @return New value.
   */
  constexpr residue_system operator-(const residue_system& rhs) const
    noexcept(noexcept(T{} - T{}));

  /**
   * @brief Multiplication.
   *
   * @param[in] rhs Value.
   * @return New value.
   */
  constexpr residue_system operator*(const residue_system& rhs) const
    noexcept(noexcept(T{} * T{}));

  /**
   * @brief Divition.
   *
   * @param[in] rhs Value.
   * @return New value.
   */
  constexpr residue_system operator/(const residue_system& rhs) const
    noexcept(noexcept(T{} / T{}));

  /**
   * @brief Swap objects.
   *
   * @param[in,out] other Swap target.
   */
  void swap(residue_system& other) noexcept;

private:
  T value_ {};
};

/**
 * @brief Swap objects.
 *
 * @param[in,out] a Swap target.
 * @param[in,out] b Swap target.
 */
template<std::size_t modulo, typename T = std::size_t>
void swap(residue_system<modulo, T>& a, residue_system<modulo, T>& b) noexcept;

}

#include "detail/residue_system.hpp"

#endif
