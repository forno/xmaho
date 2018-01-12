#ifndef XMAHO_MESSAGE_HTTP_DETAIL_CLIENT_H
#define XMAHO_MESSAGE_HTTP_DETAIL_CLIENT_H
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

#include "../client.hpp"

#include <iterator>
#include <stdexcept>
#include <utility>

template<typename StringT>
xmaho::message::http::BasicClient<StringT>::BasicClient(string_view_type method, string_view_type endpoint, string_view_type version, string_view_type body)
  : method_ {method.empty() ? throw std::invalid_argument{"xmaho::message::http::BasicClient::BasicClient : Method is empty"} : std::cbegin(method), std::cend(method)},
    endpoint_ {endpoint.empty() ? throw std::invalid_argument{"xmaho::message::http::BasicClient::BasicClient : Endpoint is empty"} : std::cbegin(endpoint), std::cend(endpoint)},
    version_ {std::cbegin(version), std::cend(version)},
    body_ {std::cbegin(body), std::cend(body)}
{
}

template<typename StringT>
template<typename... Args>
auto xmaho::message::http::BasicClient<StringT>::add_header(Args... args)
{
  return headers_.emplace(std::forward<Args>(args)...);
}

template<typename StringT>
template<typename Iterator1, typename Iterator2>
void xmaho::message::http::BasicClient<StringT>::add_headers(Iterator1 first, Iterator2 last)
{
  headers_.insert(std::move(first), std::move(last));
}

namespace xmaho::message::http::detail
{

template<typename T>
T space;

template<>
auto space<char> {' '};

template<>
auto space<wchar_t> {L' '};

template<>
auto space<char16_t> {u' '};

template<>
auto space<char32_t> {U' '};

template<typename T>
T colon;

template<>
auto colon<char> {':'};

template<>
auto colon<wchar_t> {L':'};

template<>
auto colon<char16_t> {u':'};

template<>
auto colon<char32_t> {U':'};

template<typename T>
T newline {};

template<>
auto newline<char> {"\r\n"};

template<>
auto newline<wchar_t> {L"\r\n"};

template<>
auto newline<char16_t> {u"\r\n"};

template<>
auto newline<char32_t> {U"\r\n"};

template<typename T>
T twice_newline {};

template<>
auto twice_newline<char> {"\r\n\r\n"};

template<>
auto twice_newline<wchar_t> {L"\r\n\r\n"};

template<>
auto twice_newline<char16_t> {u"\r\n\r\n"};

template<>
auto twice_newline<char32_t> {U"\r\n\r\n"};

}

template<typename StringT>
xmaho::message::http::BasicClient<StringT>::operator value_type() const
{
  using char_type = typename value_type::value_type;
  value_type v {method_};
  v += detail::space<char_type>;
  v += endpoint_;
  if (version_.empty())
    return v;
  v += detail::space<char_type>;
  v += version_;
  for (const auto& e : headers_) {
    v += detail::newline<char_type>;
    v += e.first;
    v += detail::colon<char_type>;
    v += e.second;
  }
  if (body_.empty())
    return v;
  v += detail::twice_newline<char_type>;
  v += body_;
  return v;
}

#endif
