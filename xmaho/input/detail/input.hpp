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

#ifndef XMAHO_INPUT_DETAIL_INPUT_H
#define XMAHO_INPUT_DETAIL_INPUT_H

#include "../input.hpp"

#include <iterator>
#include <type_traits>
#include <utility>

template<typename T, typename... Args>
T xmaho::input::get_value(std::basic_istream<Args...>& is)
{
  T v {};
  is >> v;
  return v;
}

namespace xmaho::input::detail
{

template<typename T>
auto has_push_back_impl(nullptr_t) -> decltype(
  std::declval<T>().push_back(std::declval<typename T::value_type>()),
  std::true_type{});

template<typename T>
auto has_push_back_impl(...) -> std::false_type;

template<typename T>
struct has_push_back
  : decltype(has_push_back_impl<T>(nullptr)) {};

}

template<typename C, typename... Args>
C xmaho::input::get_container(std::basic_istream<Args...>& is, typename C::size_type length)
{
  C v {};
  if (length != std::numeric_limits<typename C::size_type>::max())
    v.reserve(length);
  typename C::value_type e {};
  for (auto i {length}; i != 0 && is >> e; --i) {
    if constexpr (xmaho::input::detail::has_push_back<C>{}) {
      v.push_back(std::move_if_noexcept(e));
    } else {
      using std::cend;
      v.insert(cend(v), std::move_if_noexcept(e));
    }
  }
  return v;
}

#endif
