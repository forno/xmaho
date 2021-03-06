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

#ifndef XMAHO_MESSAGE_HTTP_CLIENT_H
#define XMAHO_MESSAGE_HTTP_CLIENT_H

#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>

/**
 * @file message/http/client.hpp
 * @brief HTTP client message generator.
 */

namespace xmaho::message::http
{

/**
 * @brief The StringT::size_type to StringT convertion class.
 *
 * Users can add template specializations for your strings.
 *
 * @note
 * Librarians will add specializations for u16string and u32string when std namespace get converter.
 * Users may use user define struct to avoid conflict at the time.
 *
 * @tparam StringT The string type.
 */
template<typename StringT>
struct to_string
{
  /**
   * @brief The convertion function.
   *
   * @param[in] value The StringT::size_type value.
   * @return The value as StringT.
   */
  StringT operator()(typename StringT::size_type value) const;
};

/**
 * @brief HTTP 0.9, 1.0 or 1.1 message holder.
 *
 * @tparam StringT The string type. ex) std::string, std::wstring or etc...
 * @tparam SizetostrF The StringT::size_type to StringT convertion functor.
 *
 * @code
 * xmaho::message::http::Client client_message {"GET", "/"}; // HTTP 0.9
 * std::cout << static_cast<std::string>(client_message); // GET /
 * xmaho::message::http::Client client_message {"GET", "/", "HTTP/1.1"};
 * std::cout << static_cast<std::string>(client_message); // GET / HTTP/1.1
 * xmaho::message::http::Client client_message {"POST", "/", "HTTP/1.1", "{id:1224}"};
 * std::cout << static_cast<std::string>(client_message); // GET / HTTP/1.1\r\nContent-Length:9\r\n\r\n{id:1224}
 * @endcode
 */
template<typename StringT, typename SizetostrF = to_string<StringT>>
class BasicClient
{
public:
  //! @brief The internal string type.
  using value_type = StringT;
  //! @brief The string_view type. This class is usable as StringT.
  using string_view_type = std::basic_string_view<typename value_type::value_type, typename value_type::traits_type>;
  //! @brief The header type for add_headers.
  using header_type = typename std::unordered_map<value_type, value_type>::value_type;

  /**
   * @brief HTTP message constructor.
   *
   * @param[in] method The HTTP request method.
   * @param[in] endpoint The HTTP contents location.
   * @param[in] version The HTTP version.
   * @param[in] body The body message of HTTP.
   */
  BasicClient(string_view_type method,
              string_view_type endpoint,
              string_view_type version = {},
              string_view_type body = {},
              SizetostrF converter = {});

  /**
   * @brief Add header with emplace.
   *
   * This function forward arguments to unordered_map::emplace.
   *
   * @tparam Args Types of input values.
   * @param[in] args Values for headere.
   * @return The return value of unordered_map::emplace.
   */
  template<typename... Args>
  auto add_header(Args... args);

  /**
   * @brief Add headers with insert.
   *
   * This function forward arguments to unordered_map.
   *
   * @tparam Iterator1 Type of headers iterator.
   * @tparam Iterator2 Type of headers iterator.
   * @param[in] first The first iterator of headers.
   * @param[in] last The last iterator of headers.
   */
  template<typename Iterator1, typename Iterator2>
  void add_headers(Iterator1 first, Iterator2 last);

  /**
   * @brief Create StringT value.
   *
   * This function isn't explicit. So you can use this as StringT when require StringT.
   */
  operator value_type() const;

private:
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
  //! @brief The converter for StringT::size_type to StringT.
  std::decay_t<SizetostrF> converter_;
};

using Client = BasicClient<std::string>;
using wClient = BasicClient<std::wstring>;
using u16Client = BasicClient<std::u16string>;
using u32Client = BasicClient<std::u32string>;

}

#include "detail/client.hpp"

#endif
