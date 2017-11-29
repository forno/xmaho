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
#include "xmaho/point/point.hpp"

#include <limits>
#include <random>

#include <gtest/gtest.h>

template<typename T>
class PointTest
  : public ::testing::Test
{
protected:
  using Point = xmaho::point::BasicPoint<T>;

  T first_value;
  T second_value;
  Point point;

  void SetUp()
  {
    std::default_random_engine rand {std::random_device{}()};
    std::uniform_int_distribution<T> dist {std::numeric_limits<T>::min(), std::numeric_limits<T>::max()};

    first_value = dist(rand);
    second_value = dist(rand);
    point = std::make_pair(first_value, second_value);
  }
};

using PointTypes = ::testing::Types<int, unsigned int, std::size_t, long long>;
TYPED_TEST_CASE(PointTest, PointTypes);

TYPED_TEST(PointTest, MoveLeft)
{
  const auto new_point {xmaho::point::left(this->point)};
  EXPECT_EQ(new_point.first, this->first_value - 1);
  EXPECT_EQ(new_point.second, this->second_value);
}

TYPED_TEST(PointTest, MoveRight)
{
  const auto new_point {xmaho::point::right(this->point)};
  EXPECT_EQ(new_point.first, this->first_value + 1);
  EXPECT_EQ(new_point.second, this->second_value);
}

TYPED_TEST(PointTest, MoveUp)
{
  const auto new_point {xmaho::point::up(this->point)};
  EXPECT_EQ(new_point.first, this->first_value);
  EXPECT_EQ(new_point.second, this->second_value - 1);
}

TYPED_TEST(PointTest, MoveDown)
{
  const auto new_point {xmaho::point::down(this->point)};
  EXPECT_EQ(new_point.first, this->first_value);
  EXPECT_EQ(new_point.second, this->second_value + 1);
}
