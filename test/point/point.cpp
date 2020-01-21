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

#include "xmaho/point/point.hpp"

#include <limits>
#include <random>

#include <gtest/gtest.h>

namespace
{

constexpr std::size_t testing_max_norm {5};

template<typename T>
auto get_uniform_distribution()
{
  constexpr auto reciprocal {1. / (testing_max_norm + 1)};
  const auto max_value {std::pow(std::numeric_limits<T>::max() / 2, reciprocal)};
  EXPECT_GT(max_value, 2);

  if constexpr (std::is_unsigned_v<T>)
    return std::uniform_int_distribution<T>{0u, static_cast<T>(max_value)};
  else if constexpr (std::is_integral_v<T>)
    return std::uniform_int_distribution<T>{-static_cast<T>(max_value), static_cast<T>(max_value)};
  else
    return std::uniform_real_distribution<T>{-max_value, max_value};
}

}

template<typename T>
class PointTest
  : public ::testing::Test
{
protected:
  xmaho::point::point<T> point_;
  T first_value_;
  T second_value_;

  void SetUp()
  {
    std::default_random_engine rand {std::random_device{}()};
    auto dist {get_uniform_distribution<T>()};

    first_value_ = dist(rand);
    second_value_ = dist(rand);
    point_ = std::make_pair(first_value_, second_value_);
  }
};

using PointTypes = ::testing::Types<int, unsigned int, std::size_t, long long, double>;
TYPED_TEST_CASE(PointTest, PointTypes);

TYPED_TEST(PointTest, MoveLeft)
{
  const auto new_point {xmaho::point::left(this->point_)};
  EXPECT_EQ(new_point.first, this->first_value_ - 1);
  EXPECT_EQ(new_point.second, this->second_value_);
}

TYPED_TEST(PointTest, MoveRight)
{
  const auto new_point {xmaho::point::right(this->point_)};
  EXPECT_EQ(new_point.first, this->first_value_ + 1);
  EXPECT_EQ(new_point.second, this->second_value_);
}

TYPED_TEST(PointTest, MoveUp)
{
  const auto new_point {xmaho::point::up(this->point_)};
  EXPECT_EQ(new_point.first, this->first_value_);
  EXPECT_EQ(new_point.second, this->second_value_ - 1);
}

TYPED_TEST(PointTest, MoveDown)
{
  const auto new_point {xmaho::point::down(this->point_)};
  EXPECT_EQ(new_point.first, this->first_value_);
  EXPECT_EQ(new_point.second, this->second_value_ + 1);
}

TYPED_TEST(PointTest, Norm1)
{
  if constexpr (std::is_unsigned_v<TypeParam>)
    EXPECT_EQ(xmaho::point::norm<1>(this->point_), this->first_value_ + this->second_value_);
  else
    EXPECT_EQ(xmaho::point::norm<1>(this->point_), std::abs(this->first_value_) + std::abs(this->second_value_));
}

TYPED_TEST(PointTest, Norm2)
{
  EXPECT_EQ(xmaho::point::norm<2>(this->point_), std::hypot(this->first_value_, this->second_value_));
}

TYPED_TEST(PointTest, Norm3)
{
  if constexpr (std::is_unsigned_v<TypeParam>)
    EXPECT_EQ(xmaho::point::norm<3>(this->point_), std::cbrt(std::pow(this->first_value_, 3) + std::pow(this->second_value_, 3)));
  else
    EXPECT_EQ(xmaho::point::norm<3>(this->point_), std::cbrt(std::pow(std::abs(this->first_value_), 3) + std::pow(std::abs(this->second_value_), 3)));
}

TYPED_TEST(PointTest, Norm4)
{
  EXPECT_EQ(xmaho::point::norm<4>(this->point_), std::pow(std::pow(this->first_value_, 4) + std::pow(this->second_value_, 4), 1. / 4));
}

TYPED_TEST(PointTest, NormMax)
{
  if constexpr (std::is_unsigned_v<TypeParam>)
    EXPECT_EQ(xmaho::point::norm<std::numeric_limits<std::size_t>::max()>(this->point_),
              std::max(this->first_value_, this->second_value_));
  else
    EXPECT_EQ(xmaho::point::norm<std::numeric_limits<std::size_t>::max()>(this->point_),
              std::max(std::abs(this->first_value_), std::abs(this->second_value_)));
}
