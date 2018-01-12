#ifndef XMAHO_MESSAGE_HTTP_HTTP11CLIENT_BUILDER_H
#define XMAHO_MESSAGE_HTTP_HTTP11CLIENT_BUILDER_H
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
 * @file message/http/http11client_builder.hpp
 * @brief HTTP client message generator.
 */

#include "client.hpp"

#include <string>
#include <string_view>
#include <unordered_map>

namespace xmaho::message::http
{

/**
 * @brief HTTP/1.1 message generator.
 *
 * @note
 * This class hold reference from argument of functions.
 * So all arguments of functions require long-term life then this instance.
 *
 * @tparam StringT The string type. ex) std::string, std::wstring or etc...
 *
 * @code
 * xmaho::message::http::HTTP11ClientBuilder client_builder {};
 * std::cout << static_cast<std::string>(client_builder.host("localhost").endpoint("/").get().execute()); // GET / HTTP/1.1
 * std::cout << static_cast<std::string>(client_builder.post("{id:1224}").execute()); // POST / HTTP/1.1\r\nContent-Length:9\r\n\r\n{id:1224}
 *
 * xmaho::message::http::HTTP11ClientBuilder client_builder2 {"localhost", "/"};
 * std::cout << static_cast<std::string>(client_builder2.get().execute()); // GET / HTTP/1.1
 * std::cout << static_cast<std::string>(client_builder2.endpoint("/endpoint").get().execute()); // GET /endpoint HTTP/1.1
 * @endcode
 */
template<typename StringT>
class BasicHTTP11ClientBuilder
{
public:
  //! @brief The string_view type for arguments.
  using value_type = std::basic_string_view<typename StringT::value_type, typename StringT::traits_type>;
  //! @brief The header type for add_headers.
  using header_type = typename std::unordered_map<StringT, StringT>::value_type;

  //! @brief The default constructor.
  BasicHTTP11ClientBuilder() = default;

  /**
   * @brief The constructor with some values.
   *
   * @param[in] host The hostname of server.
   * @param[in] endpoint The endpoint of HTTP/1.1.
   */
  explicit BasicHTTP11ClientBuilder(value_type host, value_type endpoint = {});

  /**
   * @brief The host header setter.
   *
   * @param[in] value The hostname of server.
   * @return The this object.
   */
  BasicHTTP11ClientBuilder& host(value_type value);

  /**
   * @brief The endpoint setter.
   *
   * @param[in] value The endpoint of HTTP/1.1
   * @return The this object.
   */
  BasicHTTP11ClientBuilder& endpoint(value_type value);

  /**
   * @brief The header setter.
   *
   * This function can overwrite old value.
   *
   * @deprecated This function will are replaced some header name functions, like host function.
   * @param[in] name The header's name.
   * @param[in] value The header's value.
   * @return The this object.
   */
  [[deprecated]]
  BasicHTTP11ClientBuilder& header(const StringT& name, value_type value);

  /**
   * @brief Add headers with iterator.
   *
   * This function don't overwrite old values for safety.
   *
   * @deprecated This function will are replaced some header name functions, like host function.
   * @tparam Iterator1 Type of headers iterator.
   * @tparam Iterator2 Type of headers iterator.
   * @param[in] first The first iterator of headers.
   * @param[in] last The last iterator of headers.
   */
  template<typename Iterator1, typename Iterator2>
  [[deprecated]]
  BasicHTTP11ClientBuilder& add_headers(Iterator1 first, Iterator2 last);

  /**
   * @brief The GET method setter.
   *
   * @return The this object.
   */
  BasicHTTP11ClientBuilder& get();

  /**
   * @brief The POST method setter.
   *
   * @param[in] value The body of POST.
   * @return The this object.
   */
  BasicHTTP11ClientBuilder& post(value_type value = {});

  /**
   * @brief Create HTTP/1.1 message.
   *
   * @tparam SizetostrF The StringT::size_type to StringT convertion functor.
   * @param[in] converter The convertion functor.
   * @return HTTP/1.1 message.
   */
  template<typename SizetostrF = to_string<StringT>>
  BasicClient<StringT, SizetostrF> execute(SizetostrF converter = {}) const;

private:
  std::unordered_map<StringT, StringT> headers_ {};
  value_type endpoint_ {};
  value_type method_ {};
  value_type body_ {};
};

using HTTP11ClientBuilder = BasicHTTP11ClientBuilder<std::string>;
using wHTTP11ClientBuilder = BasicHTTP11ClientBuilder<std::wstring>;
using u16HTTP11ClientBuilder = BasicHTTP11ClientBuilder<std::u16string>;
using u32HTTP11ClientBuilder = BasicHTTP11ClientBuilder<std::u32string>;

}

#include "detail/http11client_builder.hpp"

#endif
