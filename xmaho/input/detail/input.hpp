/*
BSD 2-Clause License

Copyright (c) 2017 - 2018, Doi Yusuke
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
#include <utility>

template<typename T>
inline T xmaho::input::get_value(std::istream& is)
{
  T v {};
  is >> v;
  return v;
}

// This function is complexity. It isn't inline function.
template<typename C>
C xmaho::input::get_container(std::istream& is, typename C::size_type length)
{
  C v {};
  typename C::value_type e {};
  for (auto i {length}; i != 0 && is >> e; --i) {
    using std::cend;
    // e aren't moved. e can be assigned by is after move,
    // but user class may are not support it.
    v.insert(cend(v), e);
  }
  return v;
}

#endif
