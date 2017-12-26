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
#include "xmaho/input/input.hpp"

#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include <gtest/gtest.h>

TEST(GetValueTest, NullInput)
{
  std::istringstream is {};
  xmaho::input::get_value<int>(is);
  EXPECT_FALSE(is);
}

TEST(GetValueTest, Numbers)
{
  std::istringstream iss {"150 1 0 -1 -150"};
  EXPECT_EQ(150, xmaho::input::get_value<int>(iss));
  EXPECT_EQ(1, xmaho::input::get_value<int>(iss));
  EXPECT_EQ(static_cast<int>(0u), xmaho::input::get_value<int>(iss));
  EXPECT_EQ(-1, xmaho::input::get_value<int>(iss));
  EXPECT_EQ(-150, xmaho::input::get_value<int>(iss));
}

TEST(GetValueTest, UnsignedNumbers)
{
  std::istringstream iss {"150 1 0"};
  EXPECT_EQ(150u, xmaho::input::get_value<std::size_t>(iss));
  EXPECT_EQ(1u, xmaho::input::get_value<std::size_t>(iss));
  EXPECT_EQ(static_cast<std::size_t>(0u), xmaho::input::get_value<std::size_t>(iss));

  EXPECT_TRUE(iss);
  xmaho::input::get_value<std::size_t>(iss);
  EXPECT_FALSE(iss);
}

TEST(GetValueTest, Strings)
{
  std::istringstream iss {"150 1 0 -1 -150 a bc long_text_input_test"};
  EXPECT_EQ("150", xmaho::input::get_value<std::string>(iss));
  EXPECT_EQ("1", xmaho::input::get_value<std::string>(iss));
  EXPECT_EQ("0", xmaho::input::get_value<std::string>(iss));
  EXPECT_EQ("-1", xmaho::input::get_value<std::string>(iss));
  EXPECT_EQ("-150", xmaho::input::get_value<std::string>(iss));
  EXPECT_EQ("a", xmaho::input::get_value<std::string>(iss));
  EXPECT_EQ("bc", xmaho::input::get_value<std::string>(iss));
  EXPECT_EQ("long_text_input_test", xmaho::input::get_value<std::string>(iss));
}

TEST(GetContainerTest, Null)
{
  std::istringstream is {};
  xmaho::input::get_container<std::vector<int>>(is);
  EXPECT_FALSE(is);
}

TEST(GetValueTest, NumberVector)
{
  std::istringstream iss {"150 1 0 -1 -150"};
  std::vector correct {150, 1, 0, -1, -150};
  EXPECT_EQ(correct, xmaho::input::get_container<std::vector<int>>(iss));
}

TEST(GetContainerTest, UnsignedNumberVector)
{
  std::istringstream iss {"150 1 0"};
  std::vector correct {150u, 1u, 0u};
  EXPECT_EQ(correct, xmaho::input::get_container<std::vector<unsigned int>>(iss));
  EXPECT_FALSE(iss);
}

TEST(GetContainerTest, StringVector)
{
  std::istringstream iss {"150 1 0 -1 150 a bc long_text_input_test"};
  std::vector<std::string> correct {"150", "1", "0", "-1", "150", "a", "bc", "long_text_input_test"};
  EXPECT_EQ(correct,
            xmaho::input::get_container<std::vector<std::string>>(iss));
}

TEST(GetContainerTest, LimitedNumberVector)
{
  std::istringstream iss {"150 1 0 -1 -150"};
  std::vector correct1 {150, 1, 0};
  EXPECT_EQ(correct1, xmaho::input::get_container<std::vector<int>>(iss, 3u));
  std::vector correct2 {-1, -150};
  EXPECT_EQ(correct2, xmaho::input::get_container<std::vector<int>>(iss, 3u));
  EXPECT_FALSE(iss);
}

TEST(GetContainerTest, NumberSet)
{
  std::istringstream iss {"150 1 0 -1 -150"};
  std::unordered_set<int> correct {150, 1, 0, -1, -150};
  EXPECT_EQ(correct, xmaho::input::get_container<std::unordered_set<int>>(iss));
}
