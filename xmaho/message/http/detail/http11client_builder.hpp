#ifndef XMAHO_MESSAGE_HTTP_DETAIL_HTTP11CLIENT_BUILDER_H
#define XMAHO_MESSAGE_HTTP_DETAIL_HTTP11CLIENT_BUILDER_H
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
#include "../http11client_builder.hpp"

#include <iterator>
#include <stdexcept>
#include <tuple>
#include <utility>

namespace xmaho::message::http::detail
{

template<typename T>
constexpr T host_str;

template<>
constexpr auto host_str<char> {"Host"};

template<>
constexpr auto host_str<wchar_t> {L"Host"};

template<>
constexpr auto host_str<char16_t> {u"Host"};

template<>
constexpr auto host_str<char32_t> {U"Host"};

}

template<typename StringT>
xmaho::message::http::BasicHTTP11ClientBuilder<StringT>::BasicHTTP11ClientBuilder(value_type host, value_type endpoint)
  : headers_ {{!host.empty() ? std::piecewise_construct : throw std::invalid_argument{"xmaho::message::http::BasicHTTP11ClientBuilder::BasicHTTP11ClientBuilder : host must be no empty"},
               std::forward_as_tuple(detail::host_str<typename StringT::value_type>),
               std::forward_as_tuple(std::cbegin(host), std::cend(host))}},
    endpoint_ {std::move(endpoint)}
{
}

template<typename StringT>
xmaho::message::http::BasicHTTP11ClientBuilder<StringT>&
xmaho::message::http::BasicHTTP11ClientBuilder<StringT>::host(value_type value)
{
  if (value.empty())
    throw std::invalid_argument{"xmaho::message::http::BasicHTTP11ClientBuilder::host : host must be no empty"};
  headers_[detail::host_str<typename StringT::value_type>].assign(std::cbegin(value), std::cend(value));
  return *this;
}

template<typename StringT>
xmaho::message::http::BasicHTTP11ClientBuilder<StringT>&
xmaho::message::http::BasicHTTP11ClientBuilder<StringT>::endpoint(value_type value)
{
  if (value.empty())
    throw std::invalid_argument{"xmaho::message::http::BasicHTTP11ClientBuilder::endpoint : endpoint must be no empty"};
  endpoint_ = std::move(value);
  return *this;
}

template<typename StringT>
xmaho::message::http::BasicHTTP11ClientBuilder<StringT>&
xmaho::message::http::BasicHTTP11ClientBuilder<StringT>::header(const StringT& name, value_type value)
{
  if (name.empty())
    throw std::invalid_argument{"xmaho::message::http::BasicHTTP11ClientBuilder::header : name must be no empty"};
  if (value.empty())
    throw std::invalid_argument{"xmaho::message::http::BasicHTTP11ClientBuilder::header : value must be no empty"};
  headers_[name].assign(std::cbegin(value), std::cend(value));
  return *this;
}

template<typename StringT>
template<typename Iterator1, typename Iterator2>
xmaho::message::http::BasicHTTP11ClientBuilder<StringT>&
xmaho::message::http::BasicHTTP11ClientBuilder<StringT>::add_headers(Iterator1 first, Iterator2 last)
{
  headers_.insert(std::move(first), std::move(last));
  return *this;
}

namespace xmaho::message::http::detail
{

template<typename T>
constexpr T get_str;

template<>
constexpr auto get_str<char> {"GET"};

template<>
constexpr auto get_str<wchar_t> {L"GET"};

template<>
constexpr auto get_str<char16_t> {u"GET"};

template<>
constexpr auto get_str<char32_t> {U"GET"};

}

template<typename StringT>
xmaho::message::http::BasicHTTP11ClientBuilder<StringT>&
xmaho::message::http::BasicHTTP11ClientBuilder<StringT>::get()
{
  method_ = detail::get_str<typename StringT::value_type>;
  body_ = {};
  return *this;
}

namespace xmaho::message::http::detail
{

template<typename T>
constexpr T post_str;

template<>
constexpr auto post_str<char> {"POST"};

template<>
constexpr auto post_str<wchar_t> {L"POST"};

template<>
constexpr auto post_str<char16_t> {u"POST"};

template<>
constexpr auto post_str<char32_t> {U"POST"};

}

template<typename StringT>
xmaho::message::http::BasicHTTP11ClientBuilder<StringT>&
xmaho::message::http::BasicHTTP11ClientBuilder<StringT>::post(value_type value)
{
  method_ = detail::post_str<typename StringT::value_type>;
  body_ = std::move(value);
  return *this;
}

namespace xmaho::message::http::detail
{

template<typename T>
constexpr T http11_str;

template<>
constexpr auto http11_str<char> {"HTTP/1.1"};

template<>
constexpr auto http11_str<wchar_t> {L"HTTP/1.1"};

template<>
constexpr auto http11_str<char16_t> {u"HTTP/1.1"};

template<>
constexpr auto http11_str<char32_t> {U"HTTP/1.1"};

}

template<typename StringT>
template<typename SizetostrF>
xmaho::message::http::BasicClient<StringT, SizetostrF>
xmaho::message::http::BasicHTTP11ClientBuilder<StringT>::execute(SizetostrF converter) const
{
  if (method_.empty())
    throw std::logic_error{"xmaho::message::http::BasicHTTP11ClientBuilder::execute : method must be set"};
  if (endpoint_.empty())
    throw std::logic_error{"xmaho::message::http::BasicHTTP11ClientBuilder::execute : endpoint must be set"};
  if (!headers_.count(detail::host_str<typename StringT::value_type>))
    throw std::logic_error{"xmaho::message::http::BasicHTTP11ClientBuilder::execute : host must be set"};

  BasicClient<StringT, SizetostrF> v {method_, endpoint_, detail::http11_str<typename StringT::value_type>, body_, std::move(converter)};
  v.add_headers(std::cbegin(headers_), std::cend(headers_));
  return v;
}

#endif
