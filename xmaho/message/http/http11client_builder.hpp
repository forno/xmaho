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

template<typename StringT>
class BasicHTTP11ClientBuilder
{
public:
  //! @brief The string_view type for arguments.
  using value_type = std::basic_string_view<typename StringT::value_type, typename StringT::traits_type>;

  //! @brief The default constructor.
  constexpr BasicHTTP11ClientBuilder() = default;

  /**
   * @brief The construct with endpoint (It stable change)
   *
   * @param[in] host The hostname of server.
   * @param[in] endpoint The endpoint of HTTP/1.1.
   */
  constexpr BasicHTTP11ClientBuilder(value_type host, value_type endpoint = {});

  /**
   * @brief The host header setter.
   *
   * @param[in] value The hostname of server.
   * @return The this object.
   */
  constexpr BasicHTTP11ClientBuilder& host(value_type value);

  /**
   * @brief The endpoint setter.
   *
   * @param[in] value The endpoint of HTTP/1.1
   * @return The this object.
   */
  constexpr BasicHTTP11ClientBuilder& endpoint(value_type value);

  /**
   * @brief The get method setter.
   *
   * @return The this object.
   */
  constexpr BasicHTTP11ClientBuilder& get();

  /**
   * @brief The POST method setter.
   *
   * @param[in] value The body of POST.
   * @return The this object.
   */
  constexpr BasicHTTP11ClientBuilder& post(value_type value = {});

  /**
   * @brief Create HTTP/1.1 message.
   *
   * @return HTTP/1.1 message.
   */
  constexpr BasicClient<StringT> execute() const;

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
