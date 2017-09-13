#ifndef XMAHO_MATH_MODULO_H
#define XMAHO_MATH_MODULO_H
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
 * @file math/residue_system.hpp
 * @brief The redidue system functors.
 */

#include "detail/residue_system_base.hpp"

#include <cstddef>

namespace xmaho
{
namespace math
{

/**
 * @brief The caliculate type on complite redidue system.
 *
 * @pre modulo > 0
 * @tparam modulo The modulo number.
 * @tparam T The value type.
 */
template<std::size_t modulo, typename T = std::size_t>
class ResidueSystem : private detail::ResidueSystemBase
{
  static_assert(modulo > 0, "Modulo must be over 0");
public:
  //! @brief The value type.
  using value_type = T;

  /**
   * @brief Default constructor with T{};
   */
  ResidueSystem();

  /**
   * @brief Construct by first value.
   */
  explicit ResidueSystem(const T& value);

  //! @brief Default copy assign for overload.
  ResidueSystem& operator=(const ResidueSystem&) & = default;
  //! @brief Default move assign for overload.
  ResidueSystem& operator=(ResidueSystem&&) & noexcept = default;

  /**
   * @brief Assign value_type value on modulo.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  ResidueSystem& operator=(const T& rhs) &;

  /**
   * @brief Assign value_type value on modulo.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  ResidueSystem& operator=(T&& rhs) & noexcept;

  /**
   * @brief Assign combine addition.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  ResidueSystem& operator+=(const ResidueSystem& rhs) &;

  /**
   * @brief Assign combine addition.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  ResidueSystem& operator+=(const T& rhs) &;

  /**
   * @brief Assign combine subtraction.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  ResidueSystem& operator-=(const ResidueSystem& rhs) &;

  /**
   * @brief Assign combine subtraction.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  ResidueSystem& operator-=(const T& rhs) &;

  /**
   * @brief Assign combine multiplication.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  ResidueSystem& operator*=(const ResidueSystem& rhs) &;

  /**
   * @brief Assign combine multiplication.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  ResidueSystem& operator*=(const T& rhs) &;

  /**
   * @brief Assign combine divition.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  ResidueSystem& operator/=(const ResidueSystem& rhs) &;

  /**
   * @brief Assign combine divition.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  ResidueSystem& operator/=(const T& rhs) &;


  /**
   * @brief Assign combine residue.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  ResidueSystem& operator%=(const ResidueSystem& rhs) &;

  /**
   * @brief Assign combine residue.
   *
   * @param[in] rhs Value.
   * @return This reference.
   */
  ResidueSystem& operator%=(const T& rhs) &;

  /**
   * @brief Swap objects.
   *
   * @param[in,out] other Swap target.
   */
  void swap(ResidueSystem& other) noexcept;
};

/**
 * @brief Swap objects.
 *
 * @param[in,out] a Swap target.
 * @param[in,out] b Swap target.
 */
template<typename T>
void swap(ResidueSystem<T>& a, ResidueSystem<T>& b) noexcept;

}
}

#include "detail/residue_system.hpp"

#endif
