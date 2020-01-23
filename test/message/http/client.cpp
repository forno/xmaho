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

#include "xmaho/message/http/client.hpp"

#include <iterator>
#include <sstream>
#include <string>

#include <gtest/gtest.h>

TEST(MessageHTTPClientTest, StandardConstruct)
{
  xmaho::message::http::Client{"GET", "/"};
}

TEST(MessageHTTPClientTest, NullConstruct)
{
  EXPECT_ANY_THROW((xmaho::message::http::Client{"", ""}));
  EXPECT_ANY_THROW((xmaho::message::http::Client{"", "/"}));
  EXPECT_ANY_THROW((xmaho::message::http::Client{"GET", ""}));
}

TEST(MessageHTTPClientTest, ToString)
{
  using namespace std::literals::string_literals;
  EXPECT_EQ("GET /"s, static_cast<std::string>(xmaho::message::http::Client{"GET", "/"}));
  EXPECT_EQ("GET / HTTP/1.1"s, static_cast<std::string>(xmaho::message::http::Client{"GET", "/", "HTTP/1.1"}));
  EXPECT_EQ("GET / HTTP/1.1\r\nContent-Length:9\r\n\r\n{id:1224}"s, static_cast<std::string>(xmaho::message::http::Client{"GET", "/", "HTTP/1.1", "{id:1224}"}));
}

TEST(MessageHTTPClientTest, NoEffectHeader)
{
  using namespace std::literals::string_literals;
  xmaho::message::http::Client value {"GET", "/"};
  value.add_header("Host", "localhost");
  EXPECT_EQ("GET /"s, static_cast<std::string>(value));
}

TEST(MessageHTTPClientTest, EmplaceMinimumHeader)
{
  using namespace std::literals::string_literals;
  xmaho::message::http::Client value {"GET", "/", "HTTP/1.1"};
  value.add_header("Host", "localhost");
  EXPECT_EQ("GET / HTTP/1.1\r\nHost:localhost"s, static_cast<std::string>(value));
}

TEST(MessageHTTPClientTest, EmplaceMinimumHeaderWithNormalValues)
{
  using namespace std::literals::string_literals;
  xmaho::message::http::Client value {"GET", "/", "HTTP/1.1", "{id:1224}"};
  value.add_header("Host", "localhost");
  EXPECT_EQ("GET / HTTP/1.1\r\nHost:localhost\r\nContent-Length:9\r\n\r\n{id:1224}"s, static_cast<std::string>(value));
}

TEST(MessageHTTPClientTest, EmplaceSomeHeaders)
{
  using namespace std::literals::string_literals;
  xmaho::message::http::Client value {"GET", "/", "HTTP/1.1", "{id:message}"};
  value.add_header("Host", "localhost");
  value.add_header("Content-Type", "application/sparql-query");
  std::istringstream iss {value};
  std::string v;
  std::getline(iss, v);
  EXPECT_EQ("GET / HTTP/1.1\r"s, v);
  std::getline(iss, v);
  EXPECT_TRUE("Host:localhost\r|Content-Type:application/sparql-query\r"s.find(v) != std::string::npos);
  std::getline(iss, v);
  EXPECT_TRUE("Host:localhost\r|Content-Type:application/sparql-query\r"s.find(v) != std::string::npos);
  std::getline(iss, v);
  EXPECT_EQ("Content-Length:12\r"s, v);
  std::getline(iss, v);
  EXPECT_EQ("\r"s, v);
  std::getline(iss, v);
  EXPECT_EQ("{id:message}"s, v);
}

TEST(MessageHTTPClientTest, InsertSomeHeadersWithIterator)
{
  using namespace std::literals::string_literals;
  xmaho::message::http::Client value {"GET", "/", "HTTP/1.1", "{id:message}"};
  std::vector<xmaho::message::http::Client::header_type> headers {{"Host", "localhost"}, {"Content-Type", "application/sparql-query"}};
  value.add_headers(std::cbegin(headers), std::cend(headers));
  std::istringstream iss {value};
  std::string v;
  std::getline(iss, v);
  EXPECT_EQ("GET / HTTP/1.1\r"s, v);
  std::getline(iss, v);
  EXPECT_TRUE("Host:localhost\r|Content-Type:application/sparql-query\r"s.find(v) != std::string::npos);
  std::getline(iss, v);
  EXPECT_TRUE("Host:localhost\r|Content-Type:application/sparql-query\r"s.find(v) != std::string::npos);
  std::getline(iss, v);
  EXPECT_EQ("Content-Length:12\r"s, v);
  std::getline(iss, v);
  EXPECT_EQ("\r"s, v);
  std::getline(iss, v);
  EXPECT_EQ("{id:message}"s, v);
}
