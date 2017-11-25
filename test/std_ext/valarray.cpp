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
#include "xmaho/std_ext/valarray.hpp"

#include <cmath>
#include <limits>
#include <vector>

#include <gtest/gtest.h>

class ValarrayIntegerEmptysTest
  : public ::testing::Test
{
protected:
  std::valarray<int> a {};
  std::valarray<int> b {};
  const int inner_product_correct {0};
  const std::vector<int> vector_product_correct {};
  const int distance_a_norm1_correct {0};
  const int distance_a_norm2_correct {0};
  const int distance_a_norm3_correct {0};
  const int distance_a_norm_max_correct {0};
};

class ValarrayIntegerSinglesTest
  : public ::testing::Test
{
protected:
  std::valarray<int> a {7};
  std::valarray<int> b {5};
  const int inner_product_correct {7 * 5};
  const std::vector<int> vector_product_correct {0};
  const int distance_a_norm1_correct {7};
  const int distance_a_norm2_correct {7};
  const int distance_a_norm3_correct {7};
  const int distance_a_norm_max_correct {7};
};

class ValarrayIntegerMultiplesTest
  : public ::testing::Test
{
protected:
  std::valarray<int> a {2, 4, 6};
  std::valarray<int> b {1, 3, 5};
  const int inner_product_correct {2*1 + 4*3 + 6*5};
  const std::vector<int> vector_product_correct {4*5-6*3, 6*1-2*5, 2*3-4*1};
  const int distance_a_norm1_correct {12};
  const int distance_a_norm2_correct {static_cast<int>(std::hypot(2, 4, 6))};
  const int distance_a_norm3_correct {static_cast<int>(std::cbrt(2*2*2 + 4*4*4 + 6*6*6))};
  const int distance_a_norm_max_correct {6};
};

class ValarrayRealEmptysTest
  : public ::testing::Test
{
protected:
  std::valarray<double> a {};
  std::valarray<double> b {};
  const double inner_product_correct {0};
  const std::vector<double> vector_product_correct {};
  const double distance_a_norm1_correct {0};
  const double distance_a_norm2_correct {0};
  const double distance_a_norm3_correct {0};
  const double distance_a_norm_max_correct {0};
};

class ValarrayRealSinglesTest
  : public ::testing::Test
{
protected:
  std::valarray<double> a {7};
  std::valarray<double> b {5};
  const double inner_product_correct {7 * 5};
  const std::vector<double> vector_product_correct {0};
  const double distance_a_norm1_correct {7};
  const double distance_a_norm2_correct {7};
  const double distance_a_norm3_correct {7};
  const double distance_a_norm_max_correct {7};
};

class ValarrayRealMultiplesTest
  : public ::testing::Test
{
protected:
  std::valarray<double> a {2, 4, 6};
  std::valarray<double> b {1, 3, 5};
  const double inner_product_correct {2*1 + 3*4 + 6*5};
  const std::vector<double> vector_product_correct {4*5-6*3, 6*1-2*5, 2*3-4*1};
  const double distance_a_norm1_correct {2 + 4 + 6};
  const double distance_a_norm2_correct {std::hypot(2, 4, 6)};
  const double distance_a_norm3_correct {std::cbrt(2*2*2 + 4*4*4 + 6*6*6)};
  const double distance_a_norm_max_correct {6};
};

TEST_F(ValarrayIntegerEmptysTest, InnerProduct)
{
  ASSERT_EQ(xmaho::std_ext::inner_product(a, b), inner_product_correct);
}

TEST_F(ValarrayIntegerSinglesTest, InnerProduct)
{
  ASSERT_EQ(xmaho::std_ext::inner_product(a, b), inner_product_correct);
}

TEST_F(ValarrayIntegerMultiplesTest, InnerProduct)
{
  ASSERT_EQ(xmaho::std_ext::inner_product(a, b), inner_product_correct);
}

TEST_F(ValarrayRealEmptysTest, InnerProduct)
{
  ASSERT_DOUBLE_EQ(xmaho::std_ext::inner_product(a, b), inner_product_correct);
}

TEST_F(ValarrayRealSinglesTest, InnerProduct)
{
  ASSERT_DOUBLE_EQ(xmaho::std_ext::inner_product(a, b), inner_product_correct);
}

TEST_F(ValarrayRealMultiplesTest, InnerProduct)
{
  ASSERT_DOUBLE_EQ(xmaho::std_ext::inner_product(a, b), inner_product_correct);
}

TEST_F(ValarrayIntegerEmptysTest, VectorProduct)
{
  const auto res {xmaho::std_ext::vector_product(a, b)};
  const std::vector<decltype(res)::value_type> ans(std::begin(res), std::end(res));
  ASSERT_EQ(ans, vector_product_correct);
}

TEST_F(ValarrayIntegerSinglesTest, VectorProduct)
{
  const auto res {xmaho::std_ext::vector_product(a, b)};
  const std::vector<decltype(res)::value_type> ans(std::begin(res), std::end(res));
  ASSERT_EQ(ans, vector_product_correct);
}

TEST_F(ValarrayIntegerMultiplesTest, VectorProduct)
{
  const auto res {xmaho::std_ext::vector_product(a, b)};
  const std::vector<decltype(res)::value_type> ans(std::begin(res), std::end(res));
  ASSERT_EQ(ans, vector_product_correct);
}

TEST_F(ValarrayRealEmptysTest, VectorProduct)
{
  const auto res {xmaho::std_ext::vector_product(a, b)};
  const std::vector<decltype(res)::value_type> ans(std::begin(res), std::end(res));
  ASSERT_EQ(ans, vector_product_correct);
}

TEST_F(ValarrayRealSinglesTest, VectorProduct)
{
  const auto res {xmaho::std_ext::vector_product(a, b)};
  const std::vector<decltype(res)::value_type> ans(std::begin(res), std::end(res));
  ASSERT_EQ(ans, vector_product_correct);
}

TEST_F(ValarrayRealMultiplesTest, VectorProduct)
{
  const auto res {xmaho::std_ext::vector_product(a, b)};
  const std::vector<decltype(res)::value_type> ans(std::begin(res), std::end(res));
  ASSERT_EQ(ans, vector_product_correct);
}

TEST_F(ValarrayIntegerEmptysTest, DistanceNorm1)
{
  ASSERT_EQ(xmaho::std_ext::distance<1>(a), distance_a_norm1_correct);
}

TEST_F(ValarrayIntegerSinglesTest, DistanceNorm1)
{
  ASSERT_EQ(xmaho::std_ext::distance<1>(a), distance_a_norm1_correct);
}

TEST_F(ValarrayIntegerMultiplesTest, DistanceNorm1)
{
  ASSERT_EQ(xmaho::std_ext::distance<1>(a), distance_a_norm1_correct);
}

TEST_F(ValarrayRealEmptysTest, DistanceNorm1)
{
  ASSERT_DOUBLE_EQ(xmaho::std_ext::distance<1>(a), distance_a_norm1_correct);
}

TEST_F(ValarrayRealSinglesTest, DistanceNorm1)
{
  ASSERT_DOUBLE_EQ(xmaho::std_ext::distance<1>(a), distance_a_norm1_correct);
}

TEST_F(ValarrayRealMultiplesTest, DistanceNorm1)
{
  ASSERT_DOUBLE_EQ(xmaho::std_ext::distance<1>(a), distance_a_norm1_correct);
}

TEST_F(ValarrayIntegerEmptysTest, DistanceNorm2)
{
  ASSERT_EQ(xmaho::std_ext::distance<2>(a), distance_a_norm2_correct);
}

TEST_F(ValarrayIntegerSinglesTest, DistanceNorm2)
{
  ASSERT_EQ(xmaho::std_ext::distance<2>(a), distance_a_norm2_correct);
}

TEST_F(ValarrayIntegerMultiplesTest, DistanceNorm2)
{
  ASSERT_EQ(xmaho::std_ext::distance<2>(a), distance_a_norm2_correct);
}

TEST_F(ValarrayRealEmptysTest, DistanceNorm2)
{
  ASSERT_DOUBLE_EQ(xmaho::std_ext::distance<2>(a), distance_a_norm2_correct);
}

TEST_F(ValarrayRealSinglesTest, DistanceNorm2)
{
  ASSERT_DOUBLE_EQ(xmaho::std_ext::distance<2>(a), distance_a_norm2_correct);
}

TEST_F(ValarrayRealMultiplesTest, DistanceNorm2)
{
  ASSERT_DOUBLE_EQ(xmaho::std_ext::distance<2>(a), distance_a_norm2_correct);
}

TEST_F(ValarrayIntegerEmptysTest, DistanceNorm3)
{
  ASSERT_EQ(xmaho::std_ext::distance<3>(a), distance_a_norm3_correct);
}

TEST_F(ValarrayIntegerSinglesTest, DistanceNorm3)
{
  ASSERT_EQ(xmaho::std_ext::distance<3>(a), distance_a_norm3_correct);
}

TEST_F(ValarrayIntegerMultiplesTest, DistanceNorm3)
{
  ASSERT_EQ(xmaho::std_ext::distance<3>(a), distance_a_norm3_correct);
}

TEST_F(ValarrayRealEmptysTest, DistanceNorm3)
{
  ASSERT_DOUBLE_EQ(xmaho::std_ext::distance<3>(a), distance_a_norm3_correct);
}

TEST_F(ValarrayRealSinglesTest, DistanceNorm3)
{
  ASSERT_DOUBLE_EQ(xmaho::std_ext::distance<3>(a), distance_a_norm3_correct);
}

TEST_F(ValarrayRealMultiplesTest, DistanceNorm3)
{
  ASSERT_DOUBLE_EQ(xmaho::std_ext::distance<3>(a), distance_a_norm3_correct);
}

TEST_F(ValarrayIntegerEmptysTest, DistanceNormMax)
{
  ASSERT_EQ(xmaho::std_ext::distance<std::numeric_limits<std::size_t>::max()>(a), distance_a_norm_max_correct);
}

TEST_F(ValarrayIntegerSinglesTest, DistanceNormMax)
{
  ASSERT_EQ(xmaho::std_ext::distance<std::numeric_limits<std::size_t>::max()>(a), distance_a_norm_max_correct);
}

TEST_F(ValarrayIntegerMultiplesTest, DistanceNormMax)
{
  ASSERT_EQ(xmaho::std_ext::distance<std::numeric_limits<std::size_t>::max()>(a), distance_a_norm_max_correct);
}

TEST_F(ValarrayRealEmptysTest, DistanceNormMax)
{
  ASSERT_DOUBLE_EQ(xmaho::std_ext::distance<std::numeric_limits<std::size_t>::max()>(a), distance_a_norm_max_correct);
}

TEST_F(ValarrayRealSinglesTest, DistanceNormMax)
{
  ASSERT_DOUBLE_EQ(xmaho::std_ext::distance<std::numeric_limits<std::size_t>::max()>(a), distance_a_norm_max_correct);
}

TEST_F(ValarrayRealMultiplesTest, DistanceNormMax)
{
  ASSERT_DOUBLE_EQ(xmaho::std_ext::distance<std::numeric_limits<std::size_t>::max()>(a), distance_a_norm_max_correct);
}
