#ifndef XMAHO_INPUT_INPUT_H
#define XMAHO_INPUT_INPUT_H
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
 * @file input/input.hpp
 * @brief Utility library for istream to values.
 */

#include <istream>
#include <limits>

namespace xmaho::input
{

/**
 * @brief Construct T type value from is.
 *
 * @tparam T The value type. Required DefaultConstructible.
 * @param[in] is The input stream for value.
 * @return The T type value.
 *
 * @code
 * istringstream iss {"0 1 abc"};
 * assert(0 == get_value<int>(iss));
 * assert('1' == get_value<char>(iss));
 * assert("abc" == get_value<std::string>(iss));
 * get_value<unsigned int>(std::cin); // get a value from standard input
 * @endcode
 */
template<typename T>
constexpr T get_value(std::istream& is);

/**
 * @brief Construct C type container from is.
 *
 * Construct container until is.good() == true or size() < length.
 * If fail on process then quit function.
 * It mean this function returnable few size container.
 *
 * @tparam C The container type. Required SequenceContainer.
 * @param[in] is The input stream for value.
 * @param[in] length The container size.
 * @return The C type container.
 *
 * @code
 * istringstream iss {"0 2 1 abc 0 1 abc"};
 * assert(vector{0, 2} == get_container<vector<int>>(iss, 2));
 * assert(list{'1'} == get_container<list<char>>(iss, 2)); // return fewer size than length
 * assert(!iss);
 * iss.clear();
 * assert(unordered_set<string>{"abc"} == get_container<unordered_set<string>>(iss, 1));
 * assert(vector{0, 1} == get_container<vector<int>>(iss)); // read until fail
 * assert(!iss); // you not specify length then istream must get fail state
 * iss.clear();
 * assert(string{"abc"} == get_container<string>(iss)); // string is container
 * assert(!iss);
 * assert(iss.eof()); // you can check stream last with eof().
 * get_container<unsigned int>(cin, 5); // get a container from standard input
 * get_container<unsigned int>(cin); // get a container from standard input
 * assert(!cin); // you not specify length then istream must get fail state
 * @endcode
 */
template<typename C>
constexpr C get_container(std::istream& is, typename C::size_type length = std::numeric_limits<typename C::size_type>::max());

}

#include "detail/input.hpp"

#endif
