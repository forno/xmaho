#ifndef XMAHO_MESSAGE_HTTP_CLIENT_H
#define XMAHO_MESSAGE_HTTP_CLIENT_H
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
 * @file message/http/client.hpp
 * @brief HTTP client message generator.
 */

#include <string>
#include <string_view>
#include <unordered_map>

namespace xmaho::message::http
{

/**
 * @brief HTTP 1.1 message holder.
 *
 * @note This class don't protect values.
 *
 * @tparam StringT The string type. ex) std::string or std::wstring
 * @code
 * xmaho::message::http::Client client_message {"GET", "/", "HTTP/1.1"};
 * std::cout << static_cast<std::string>(client_message); //!< GET / HTTP/1.1
 * xmaho::message::http::Client client_message {"POST", "/", "HTTP/1.1", "{id:1224}"};
 * std::cout << static_cast<std::string>(client_message); //!< GET / HTTP/1.1\r\nHost: localhost\r\n\r\n{id:1224}
 * @endcode
 */
template<template<typename...> typename StringT, typename CharT, typename... Args>
struct BasicClient
{
  //! @brief The internal string type. This class is usable as StringT.
  using value_type = StringT<CharT, Args...>;

  //! @brief The headers of HTTP request.
  std::unordered_map<value_type, value_type> headers_ {};
  //! @brief The HTTP method.
  value_type method_;
  //! @brief The content endpoint.
  value_type endpoint_;
  //! @brief The HTTP version.
  value_type version_;
  //! @brief The body message of HTTP.
  value_type body_;

  //! @brief The internal string type. This class is usable as StringT.
  using string_view_type = std::basic_string_view<typename value_type::value_type, typename value_type::traits_type>;

  /**
   * @brief HTTP 1.1 message constructor with body.
   *
   * @param[in] method The HTTP request method.
   * @param[in] endpoint The HTTP contents location.
   * @param[in] version The HTTP version.
   * @param[in] body The body message of HTTP.
   */
  constexpr BasicClient(string_view_type method,
                        string_view_type endpoint,
                        string_view_type version = {},
                        string_view_type body = {});

  constexpr operator value_type() const;
};

using Client = BasicClient<std::basic_string, char>;
using wClient = BasicClient<std::basic_string, wchar_t>;
using u16Client = BasicClient<std::basic_string, char16_t>;
using u32Client = BasicClient<std::basic_string, char32_t>;

}

#include "detail/client.hpp"

#endif
