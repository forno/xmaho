/*
BSD 2-Clause License

Copyright (c) 2020, FORNO
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

#ifndef XMAHO_FLOATING_DETAIL_COMPARE_H
#define XMAHO_FLOATING_DETAIL_COMPARE_H

#include "../compare.hpp"

#include <cmath>

/**
 * @file floating/compare.hpp
 * @brief Compare function for floating point numbers
 */

template<typename T>
constexpr xmaho::floating::equal<T>::equal(const T& epsilon) noexcept
  : epsilon_ {epsilon}
{}

template<typename T>
constexpr bool xmaho::floating::equal<T>::operator()(const T& lhs, const T& rhs) const noexcept
{
  using std::abs;
  using std::max;
  const auto diff {abs(lhs - rhs)};
  if (diff <= epsilon_)
    return true;
  return diff <= (max(abs(lhs), abs(rhs)) * epsilon_);
}

#endif
