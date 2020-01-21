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

#include "xmaho/message/http/http11client_builder.hpp"

#include <string>

#include <gtest/gtest.h>

TEST(MessageHttpHTTP11ClientBuilderTest, MinimumConstruct)
{
  xmaho::message::http::HTTP11ClientBuilder{};
}

TEST(MessageHttpHttp11ClientBuilderTest, MinimumExecute)
{
  std::string value {xmaho::message::http::HTTP11ClientBuilder{}
                         .host("localhost")
                         .endpoint("/")
                         .get()
                         .execute()};
  using namespace std::literals::string_literals;
  EXPECT_EQ("GET / HTTP/1.1\r\nHost:localhost"s, value);
}

TEST(MessageHttpHttp11ClientBuilderTest, FewArgumentsExecute)
{
  EXPECT_ANY_THROW(xmaho::message::http::HTTP11ClientBuilder{}.execute());
  EXPECT_ANY_THROW(xmaho::message::http::HTTP11ClientBuilder{}.endpoint("/").execute());
  EXPECT_ANY_THROW(xmaho::message::http::HTTP11ClientBuilder{}.host("localhost").execute());
  EXPECT_ANY_THROW(xmaho::message::http::HTTP11ClientBuilder{}.get().execute());
  EXPECT_ANY_THROW(xmaho::message::http::HTTP11ClientBuilder{}.host("localhost").endpoint("/").execute());
  EXPECT_ANY_THROW(xmaho::message::http::HTTP11ClientBuilder{}.endpoint("/").get().execute());
  EXPECT_ANY_THROW(xmaho::message::http::HTTP11ClientBuilder{}.host("localhost").get().execute());
}

TEST(MessageHttpHttp11ClientBuilderTest, ReuseObject)
{
  const auto host {"localhost"};
  const auto endpoint {"/"};
  xmaho::message::http::HTTP11ClientBuilder builder {};
  const std::string value1 {builder.host(host)
                                   .endpoint(endpoint)
                                   .post("first body message")
                                   .execute()};
  using namespace std::literals::string_literals;
  EXPECT_EQ("POST / HTTP/1.1\r\nHost:localhost\r\nContent-Length:18\r\n\r\nfirst body message"s, value1);
  const std::string value2 {builder.post("second body message").execute()};
  EXPECT_EQ("POST / HTTP/1.1\r\nHost:localhost\r\nContent-Length:19\r\n\r\nsecond body message"s, value2);
  const std::string value3 {builder.get().execute()};
  EXPECT_EQ("GET / HTTP/1.1\r\nHost:localhost"s, value3);
}

TEST(MessageHttpHttp11ClientBuilderTest, MultiByteString)
{
  std::wstring value {xmaho::message::http::wHTTP11ClientBuilder{}
                          .host(L"localhost")
                          .endpoint(L"/")
                          .get()
                          .execute()};
  using namespace std::literals::string_literals;
  EXPECT_EQ(L"GET / HTTP/1.1\r\nHost:localhost"s, value);
}
