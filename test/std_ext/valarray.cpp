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

#include "xmaho/std_ext/valarray.hpp"

#include <cmath>
#include <limits>
#include <random>
#include <vector>

#include <gtest/gtest.h>

namespace
{

constexpr std::size_t testing_max_norm {5};

template<typename T>
auto get_uniform_distribution(std::size_t data_count)
{
  constexpr auto reciprocal {1. / (testing_max_norm + 1)};
  const auto max_value {std::pow(static_cast<long double>(std::numeric_limits<T>::max()) / static_cast<long double>(data_count), reciprocal)};
  EXPECT_GT(std::pow(std::numeric_limits<T>::max(), reciprocal), 2);
  EXPECT_GT(max_value, 1);

  if constexpr (std::is_unsigned_v<T>)
    return std::uniform_int_distribution<T>{0u, static_cast<T>(max_value)};
  else if constexpr (std::is_integral_v<T>)
    return std::uniform_int_distribution<T>{-static_cast<T>(max_value), static_cast<T>(max_value)};
  else
    return std::uniform_real_distribution<T>{-static_cast<T>(max_value), static_cast<T>(max_value)};
}

}

template<typename T>
class ValarrayTest
  : public ::testing::Test
{
protected:
  using Valarray = std::valarray<T>;
  using ValidatorType = std::vector<T>;

  std::size_t size_;
  Valarray a_;
  Valarray b_;

  void SetUp()
  {
    std::default_random_engine rand {std::random_device{}()};

    std::uniform_int_distribution<std::size_t> size_dist {1u, 4u};
    size_ = size_dist(rand);
    a_.resize(size_);
    b_.resize(size_);

    auto dist {get_uniform_distribution<T>(size_)};
    for (auto& e : a_) e = dist(rand);
    for (auto& e : b_) e = dist(rand);
  }
};

using ValarrayTypes = ::testing::Types<int, unsigned int, std::size_t, long long, double>;
TYPED_TEST_CASE(ValarrayTest, ValarrayTypes);

TYPED_TEST(ValarrayTest, InnerProduct)
{
  EXPECT_EQ(xmaho::std_ext::inner_product(this->a_, this->b_), (this->a_ * this->b_).sum());
}

TYPED_TEST(ValarrayTest, VectorProduct)
{
  const auto res {xmaho::std_ext::vector_product(this->a_, this->b_)};
  const typename TestFixture::ValidatorType ans(std::begin(res), std::end(res));
  const typename TestFixture::Valarray correct_raw {this->a_.cshift(1) * this->b_.cshift(-1) - this->a_.cshift(-1) * this->b_.cshift(1)};
  const typename TestFixture::ValidatorType correct(std::begin(correct_raw), std::end(correct_raw));
  EXPECT_EQ(ans, correct);
}

TYPED_TEST(ValarrayTest, DistanceNorm1)
{
  const auto ans {xmaho::std_ext::norm<1>(this->a_)};
  if constexpr (std::is_unsigned_v<TypeParam>)
    EXPECT_EQ(ans, this->a_.sum());
  else
    EXPECT_EQ(ans, std::abs(this->a_).sum());
}

TYPED_TEST(ValarrayTest, DistanceNorm2)
{
  EXPECT_EQ(std::sqrt(std::pow<TypeParam>(this->a_, 2).sum()), xmaho::std_ext::norm<2>(this->a_));
}

TYPED_TEST(ValarrayTest, DistanceNorm3)
{
  const auto ans {xmaho::std_ext::norm<3>(this->a_)};
  if constexpr (std::is_unsigned_v<TypeParam>)
    EXPECT_EQ(std::cbrt(std::pow<TypeParam>(this->a_, 3).sum()), ans);
  else if constexpr (std::is_same_v<double, TypeParam>)
    EXPECT_DOUBLE_EQ(std::cbrt(std::pow<TypeParam>(std::abs(this->a_), 3).sum()), ans);
  else
    EXPECT_EQ(std::cbrt(std::pow<TypeParam>(std::abs(this->a_), 3).sum()), ans);
}

TYPED_TEST(ValarrayTest, DistanceNorm4)
{
  EXPECT_EQ(xmaho::std_ext::norm<4>(this->a_), std::pow(std::pow<TypeParam>(this->a_, 4).sum(), 1. / 4));
}

TYPED_TEST(ValarrayTest, DistanceNorm5)
{
  if constexpr (std::is_unsigned_v<TypeParam>)
    EXPECT_EQ(xmaho::std_ext::norm<5>(this->a_), std::pow(std::pow<TypeParam>(this->a_, 5).sum(), 1. / 5));
  else
    EXPECT_EQ(xmaho::std_ext::norm<5>(this->a_), std::pow(std::pow<TypeParam>(std::abs(this->a_), 5).sum(), 1. / 5));
}

TYPED_TEST(ValarrayTest, DistanceNormMax)
{
  if constexpr (std::is_unsigned_v<TypeParam>)
    EXPECT_EQ(xmaho::std_ext::norm<std::numeric_limits<std::size_t>::max()>(this->a_), this->a_.max());
  else
    EXPECT_EQ(xmaho::std_ext::norm<std::numeric_limits<std::size_t>::max()>(this->a_), std::abs(this->a_).max());
}
