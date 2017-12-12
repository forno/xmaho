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
#include "xmaho/std_ext/valmatrix.hpp"

#include <algorithm>
#include <cstddef>
#include <numeric>
#include <random>
#include <vector>

#include <gtest/gtest.h>

namespace
{

using Size = std::pair<std::size_t, std::size_t>;

constexpr std::size_t size_of(const Size& size)
{
  return size.first * size.second;
}

template<typename Container>
std::vector<typename Container::value_type> as_validator(const Container& container)
{
  return std::vector<typename Container::value_type>(std::begin(container), std::end(container));
}

template<typename T>
auto get_positive_uniform_distribution()
{
  if constexpr (std::is_integral_v<T>)
    return std::uniform_int_distribution<T>{1u, 5u};
  else
    return std::uniform_real_distribution<T>{1u, 5u};
}

}

template<typename T>
class ValmatrixTest
  : public ::testing::Test
{
protected:
  using Valmatrix = xmaho::std_ext::valmatrix<T>;
  using Valarray = std::valarray<T>;
  using ValidatorType = std::vector<T>;

  static constexpr Size size {3, 2};

  Valmatrix iota_matrix {size.first, size.second};
  Valarray iota_array;
  Valmatrix operation_matrix {size.first, size.second};
  Valarray operation_array;
  T operation_value;

  ValmatrixTest()
    : iota_array(size_of(size)),
      operation_array(size_of(size))
  {
  }

  void SetUp()
  {
    EXPECT_EQ(iota_array.size(), iota_matrix.size());
    std::iota(std::begin(iota_matrix), std::end(iota_matrix), 1);
    std::iota(std::begin(iota_array), std::end(iota_array), 1);

    std::default_random_engine rand {std::random_device{}()};
    auto dist {get_positive_uniform_distribution<T>()};
    for (auto& e : operation_matrix)
      e = dist(rand);
    for (auto& e : operation_array)
      e = dist(rand);
    operation_value = dist(rand);
  }
};

template<typename T>
constexpr Size ValmatrixTest<T>::size;

using ValmatrixTypes = ::testing::Types<int, unsigned int, std::size_t, double>;
TYPED_TEST_CASE(ValmatrixTest, ValmatrixTypes);

TEST(ValmatrixConstructTest, AllConstructors)
{
  const xmaho::std_ext::valmatrix<int> zero {}; // Default construct
  EXPECT_FALSE(zero.size());
  const xmaho::std_ext::valmatrix<int> one {1, 1}; // mim size
  EXPECT_EQ(1, one.size());
  EXPECT_EQ(static_cast<int>(0), one[0]);

  constexpr Size diff_size {2, 5};
  const xmaho::std_ext::valmatrix<int> diffsize_mat {diff_size.first, diff_size.second}; // different size
  EXPECT_EQ(size_of(diff_size), diffsize_mat.size());

  constexpr Size zero_size {100, 0};
  const xmaho::std_ext::valmatrix<int> zero_mat {zero_size.first, zero_size.second};
  EXPECT_EQ(size_of(zero_size), zero_mat.size());

  constexpr Size square_size {2, 2};
  constexpr auto default_val {250};
  const xmaho::std_ext::valmatrix<int> def_val_mat {default_val, square_size.first, square_size.second};
  const std::vector<int> def_val_mat_correct(size_of(square_size), default_val);
  const std::vector<int> def_val_mat_value(std::begin(def_val_mat), std::end(def_val_mat));
  EXPECT_EQ(def_val_mat_correct, def_val_mat_value);

  std::valarray<int> values {5, 9, 2, 4, 6, 7};
  constexpr Size def_size {3, 2};
  const xmaho::std_ext::valmatrix<int> mat_with_def {values, def_size.first, def_size.second};
  const std::vector<int> mat_with_def_correct(std::begin(values), std::end(values));
  const std::vector<int> mat_with_def_value(std::begin(mat_with_def), std::end(mat_with_def));
  EXPECT_EQ(mat_with_def_correct, mat_with_def_value);
}

TEST(ValmatrixSizeTest, RowAndColSizeCheck)
{
  const xmaho::std_ext::valmatrix<int> zero {100, 0};
  EXPECT_EQ(static_cast<std::size_t>(0u), zero.row_size());
  EXPECT_EQ(static_cast<std::size_t>(0u), zero.col_size());

  const xmaho::std_ext::valmatrix<int> normal {5, 9};
  EXPECT_EQ(5, normal.row_size());
  EXPECT_EQ(9, normal.col_size());
}

TYPED_TEST(ValmatrixTest, ReadValueByIndex)
{
  for (auto i {0u}; i < size_of(TestFixture::size); ++i)
    EXPECT_EQ(this->iota_array[i], this->iota_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ReadArrayBySlice)
{
  constexpr auto start {0u};
  constexpr auto length {3u};
  constexpr auto stride {2u};
  static_assert(start + (length - 1) * stride < size_of(TestFixture::size), "Over size");
  const std::slice specification {start, length, stride};

  const typename TestFixture::Valarray correct_raw {this->iota_array[specification]};
  const typename TestFixture::Valarray value_raw {this->iota_matrix[specification]};
  EXPECT_EQ(as_validator(correct_raw), as_validator(value_raw));
}

TYPED_TEST(ValmatrixTest, ReadArrayByGslice)
{
  constexpr auto start {1u};
  const std::valarray<std::size_t> length {2, 2};
  const std::valarray<std::size_t> stride {3, 1};
  assert(start + (length[0] - 1) * stride[0] + (length[1] - 1) * stride[1] < size_of(TestFixture::size));
  const std::gslice specification {start, length, stride};

  const typename TestFixture::Valarray correct_raw {this->iota_array[specification]};
  const typename TestFixture::Valarray value_raw {this->iota_matrix[specification]};
  EXPECT_EQ(as_validator(correct_raw), as_validator(value_raw));
}

TYPED_TEST(ValmatrixTest, ReadArrayByMask)
{
  std::valarray<bool> specification(size_of(TestFixture::size));

  std::default_random_engine rand {std::random_device{}()};
  std::bernoulli_distribution dist {0.7};
  for (auto& e : specification)
    e = dist(rand);

  const typename TestFixture::Valarray correct_raw {this->iota_array[specification]};
  const typename TestFixture::Valarray value_raw {this->iota_matrix[specification]};
  EXPECT_EQ(as_validator(correct_raw), as_validator(value_raw));
}

TYPED_TEST(ValmatrixTest, ReadArrayByIndirect)
{
  std::default_random_engine rand {std::random_device{}()};
  std::uniform_int_distribution<std::size_t> size_dist {0u, size_of(TestFixture::size)};
  std::valarray<std::size_t> specification(size_dist(rand));
  std::uniform_int_distribution<std::size_t> index_dist {0u, size_of(TestFixture::size) - 1};
  for (auto& e : specification) e = index_dist(rand);

  const typename TestFixture::Valarray correct_raw {this->iota_array[specification]};
  const typename TestFixture::Valarray value_raw {this->iota_matrix[specification]};
  EXPECT_EQ(as_validator(correct_raw), as_validator(value_raw));
}

TYPED_TEST(ValmatrixTest, ReadValueByPosition)
{
  for (auto i {0u}; i < TestFixture::size.first; ++i)
    for (auto j {0u}; j < TestFixture::size.second; ++j) {
      const auto value {this->iota_matrix[typename TestFixture::Valmatrix::position_type{i, j}]};
      EXPECT_EQ(this->iota_array[j * TestFixture::size.second + i], value);
    }
}

TYPED_TEST(ValmatrixTest, UnaryAddOperation)
{
  const typename TestFixture::Valmatrix effected {+this->iota_matrix};
  EXPECT_EQ(as_validator(this->iota_array), as_validator(effected));
}

TYPED_TEST(ValmatrixTest, UnarySubOperation)
{
  if constexpr (!std::is_unsigned_v<TypeParam>) {
    const typename TestFixture::Valarray correct_raw {-this->iota_array};
    const typename TestFixture::Valmatrix effected {-this->iota_matrix};
    EXPECT_EQ(as_validator(correct_raw), as_validator(effected));
  }
}

TYPED_TEST(ValmatrixTest, UnaryNegationOperation)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    const typename TestFixture::Valarray correct_raw {~this->iota_array};
    const typename TestFixture::Valmatrix effected {~this->iota_matrix};
    EXPECT_EQ(as_validator(correct_raw), as_validator(effected));
  }
}

TYPED_TEST(ValmatrixTest, AdditionAssign)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_matrix),
                 std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs + rhs;});
  this->iota_matrix += this->operation_matrix;
  EXPECT_EQ(correct, as_validator(this->iota_matrix));
}

TYPED_TEST(ValmatrixTest, ArrayAdditionAssign)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                 std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs + rhs;});
  this->iota_matrix += this->operation_array;
  EXPECT_EQ(correct, as_validator(this->iota_matrix));
}

TYPED_TEST(ValmatrixTest, ValueAdditionAssign)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                 std::begin(correct), [v = this->operation_value](const auto& e){return e + v;});
  this->iota_matrix += this->operation_value;
  EXPECT_EQ(correct, as_validator(this->iota_matrix));
}

TYPED_TEST(ValmatrixTest, MatrixSubtractAssign)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_matrix),
                 std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs - rhs;});
  this->iota_matrix -= this->operation_matrix;
  EXPECT_EQ(correct, as_validator(this->iota_matrix));
}

TYPED_TEST(ValmatrixTest, ArraySubtractAssign)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                 std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs - rhs;});
  this->iota_matrix -= this->operation_array;
  EXPECT_EQ(correct, as_validator(this->iota_matrix));
}

TYPED_TEST(ValmatrixTest, ValueSubtractAssign)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                 std::begin(correct), [v = this->operation_value](const auto& e){return e - v;});
  this->iota_matrix -= this->operation_value;
  EXPECT_EQ(correct, as_validator(this->iota_matrix));
}

TYPED_TEST(ValmatrixTest, MatrixMultiplicationAssign)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_matrix),
                 std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs * rhs;});
  this->iota_matrix *= this->operation_matrix;
  EXPECT_EQ(correct, as_validator(this->iota_matrix));
}

TYPED_TEST(ValmatrixTest, ArrayMultiplicationAssign)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                 std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs * rhs;});
  this->iota_matrix *= this->operation_array;
  EXPECT_EQ(correct, as_validator(this->iota_matrix));
}

TYPED_TEST(ValmatrixTest, ValueMultiplicationAssign)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                 std::begin(correct), [v = this->operation_value](const auto& e){return e * v;});
  this->iota_matrix *= this->operation_value;
  EXPECT_EQ(correct, as_validator(this->iota_matrix));
}

TYPED_TEST(ValmatrixTest, MatrixDivisionAssign)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_matrix),
                 std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs / rhs;});
  this->iota_matrix /= this->operation_matrix;
  EXPECT_EQ(correct, as_validator(this->iota_matrix));
}

TYPED_TEST(ValmatrixTest, ArrayDivisionAssign)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                 std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs / rhs;});
  this->iota_matrix /= this->operation_array;
  EXPECT_EQ(correct, as_validator(this->iota_matrix));
}

TYPED_TEST(ValmatrixTest, ValueDivisionAssign)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                 std::begin(correct), [v = this->operation_value](const auto& e){return e / v;});
  this->iota_matrix /= this->operation_value;
  EXPECT_EQ(correct, as_validator(this->iota_matrix));
}

TYPED_TEST(ValmatrixTest, MatrixResidueAssign)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_matrix),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs % rhs;});
    this->iota_matrix %= this->operation_matrix;
    EXPECT_EQ(correct, as_validator(this->iota_matrix));
  }
}

TYPED_TEST(ValmatrixTest, ArrayResidueAssign)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs % rhs;});
    this->iota_matrix %= this->operation_array;
    EXPECT_EQ(correct, as_validator(this->iota_matrix));
  }
}

TYPED_TEST(ValmatrixTest, ValueResidueAssign)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                   std::begin(correct), [v = this->operation_value](const auto& e){return e % v;});
    this->iota_matrix %= this->operation_value;
    EXPECT_EQ(correct, as_validator(this->iota_matrix));
  }
}

TYPED_TEST(ValmatrixTest, MatrixBitwiseAndAssign)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_matrix),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs & rhs;});
    this->iota_matrix &= this->operation_matrix;
    EXPECT_EQ(correct, as_validator(this->iota_matrix));
  }
}

TYPED_TEST(ValmatrixTest, ArrayBitwiseAndAssign)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs & rhs;});
    this->iota_matrix &= this->operation_array;
    EXPECT_EQ(correct, as_validator(this->iota_matrix));
  }
}

TYPED_TEST(ValmatrixTest, ValueBitwiseAndAssign)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                   std::begin(correct), [v = this->operation_value](const auto& e){return e & v;});
    this->iota_matrix &= this->operation_value;
    EXPECT_EQ(correct, as_validator(this->iota_matrix));
  }
}

TYPED_TEST(ValmatrixTest, MatrixBitwiseOrAssign)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_matrix),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs | rhs;});
    this->iota_matrix |= this->operation_matrix;
    EXPECT_EQ(correct, as_validator(this->iota_matrix));
  }
}

TYPED_TEST(ValmatrixTest, ArrayBitwiseOrAssign)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs | rhs;});
    this->iota_matrix |= this->operation_array;
    EXPECT_EQ(correct, as_validator(this->iota_matrix));
  }
}

TYPED_TEST(ValmatrixTest, ValueBitwiseOrAssign)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                   std::begin(correct), [v = this->operation_value](const auto& e){return e | v;});
    this->iota_matrix |= this->operation_value;
    EXPECT_EQ(correct, as_validator(this->iota_matrix));
  }
}

TYPED_TEST(ValmatrixTest, MatrixXorAssign)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_matrix),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs ^ rhs;});
    this->iota_matrix ^= this->operation_matrix;
    EXPECT_EQ(correct, as_validator(this->iota_matrix));
  }
}

TYPED_TEST(ValmatrixTest, ArrayXorAssign)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs ^ rhs;});
    this->iota_matrix ^= this->operation_array;
    EXPECT_EQ(correct, as_validator(this->iota_matrix));
  }
}

TYPED_TEST(ValmatrixTest, ValueXorAssign)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                   std::begin(correct), [v = this->operation_value](const auto& e){return e ^ v;});
    this->iota_matrix ^= this->operation_value;
    EXPECT_EQ(correct, as_validator(this->iota_matrix));
  }
}

TYPED_TEST(ValmatrixTest, MatrixShiftAssign)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_matrix),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs << rhs;});
    this->iota_matrix <<= this->operation_matrix;
    EXPECT_EQ(correct, as_validator(this->iota_matrix));
  }
}

TYPED_TEST(ValmatrixTest, ArrayShiftAssign)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs << rhs;});
    this->iota_matrix <<= this->operation_array;
    EXPECT_EQ(correct, as_validator(this->iota_matrix));
  }
}

TYPED_TEST(ValmatrixTest, ValueShiftAssign)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                   std::begin(correct), [v = this->operation_value](const auto& e){return e << v;});
    this->iota_matrix <<= this->operation_value;
    EXPECT_EQ(correct, as_validator(this->iota_matrix));
  }
}

TYPED_TEST(ValmatrixTest, MatrixCounterShiftAssign)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_matrix),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs >> rhs;});
    this->iota_matrix >>= this->operation_matrix;
    EXPECT_EQ(correct, as_validator(this->iota_matrix));
  }
}

TYPED_TEST(ValmatrixTest, ArrayCounterShiftAssign)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs >> rhs;});
    this->iota_matrix >>= this->operation_array;
    EXPECT_EQ(correct, as_validator(this->iota_matrix));
  }
}

TYPED_TEST(ValmatrixTest, ValueCounterShiftAssign)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                   std::begin(correct), [v = this->operation_value](const auto& e){return e >> v;});
    this->iota_matrix >>= this->operation_value;
    EXPECT_EQ(correct, as_validator(this->iota_matrix));
  }
}

TYPED_TEST(ValmatrixTest, MatrixAdditionOperator)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_matrix),
                 std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs + rhs;});
  const auto result {this->iota_matrix + this->operation_matrix};
  EXPECT_EQ(correct, as_validator(result));
}

TYPED_TEST(ValmatrixTest, ArrayAdditionOperator)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                 std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs + rhs;});
  const auto result {this->iota_matrix + this->operation_array};
  EXPECT_EQ(correct, as_validator(result));
}

TYPED_TEST(ValmatrixTest, ValueAdditionOperator)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                 std::begin(correct), [v = this->operation_value](const auto& e){return e + v;});
  const auto result {this->iota_matrix + this->operation_value};
  EXPECT_EQ(correct, as_validator(result));
}

TYPED_TEST(ValmatrixTest, ArrayInverseAdditionOperator)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                 std::begin(correct), [](const auto& lhs, const auto& rhs){return rhs + lhs;});
  const auto result {this->operation_array + this->iota_matrix};
  EXPECT_EQ(correct, as_validator(result));
}

TYPED_TEST(ValmatrixTest, ValueInverseAdditionOperator)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                 std::begin(correct), [v = this->operation_value](const auto& e){return v + e;});
  const auto result {this->operation_value + this->iota_matrix};
  EXPECT_EQ(correct, as_validator(result));
}

TYPED_TEST(ValmatrixTest, MatrixSubtractOperator)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_matrix),
                 std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs - rhs;});
  const auto result {this->iota_matrix - this->operation_matrix};
  EXPECT_EQ(correct, as_validator(result));
}

TYPED_TEST(ValmatrixTest, ArraySubtractOperator)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                 std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs - rhs;});
  const auto result {this->iota_matrix - this->operation_array};
  EXPECT_EQ(correct, as_validator(result));
}

TYPED_TEST(ValmatrixTest, ValueSubtractOperator)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                 std::begin(correct), [v = this->operation_value](const auto& e){return e - v;});
  const auto result {this->iota_matrix - this->operation_value};
  EXPECT_EQ(correct, as_validator(result));
}

TYPED_TEST(ValmatrixTest, ArrayInverseSubtractOperator)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                 std::begin(correct), [](const auto& lhs, const auto& rhs){return rhs - lhs;});
  const auto result {this->operation_array - this->iota_matrix};
  EXPECT_EQ(correct, as_validator(result));
}

TYPED_TEST(ValmatrixTest, ValueInverseSubtractOperator)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                 std::begin(correct), [v = this->operation_value](const auto& e){return v - e;});
  const auto result {this->operation_value - this->iota_matrix};
  EXPECT_EQ(correct, as_validator(result));
}

TYPED_TEST(ValmatrixTest, MatrixMultiplicationOperator)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_matrix),
                 std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs * rhs;});
  const auto result {this->iota_matrix * this->operation_matrix};
  EXPECT_EQ(correct, as_validator(result));
}

TYPED_TEST(ValmatrixTest, ArrayMultiplicationOperator)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                 std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs * rhs;});
  const auto result {this->iota_matrix * this->operation_array};
  EXPECT_EQ(correct, as_validator(result));
}

TYPED_TEST(ValmatrixTest, ValueMultiplicationOperator)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                 std::begin(correct), [v = this->operation_value](const auto& e){return e * v;});
  const auto result {this->iota_matrix * this->operation_value};
  EXPECT_EQ(correct, as_validator(result));
}

TYPED_TEST(ValmatrixTest, ArrayInverseMultiplicationOperator)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                 std::begin(correct), [](const auto& lhs, const auto& rhs){return rhs * lhs;});
  const auto result {this->operation_array * this->iota_matrix};
  EXPECT_EQ(correct, as_validator(result));
}

TYPED_TEST(ValmatrixTest, ValueInverseMultiplicationOperator)
{
  typename TestFixture::ValidatorType correct(this->iota_array.size());
  std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                 std::begin(correct), [v = this->operation_value](const auto& e){return v * e;});
  const auto result {this->operation_value * this->iota_matrix};
  EXPECT_EQ(correct, as_validator(result));
}

TYPED_TEST(ValmatrixTest, MatrixDivisionOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_matrix),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs / rhs;});
    const auto result {this->iota_matrix / this->operation_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ArrayDivisionOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs / rhs;});
    const auto result {this->iota_matrix / this->operation_array};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ValueDivisionOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                   std::begin(correct), [v = this->operation_value](const auto& e){return e / v;});
    const auto result {this->iota_matrix / this->operation_value};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ArrayInverseDivisionOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return rhs / lhs;});
    const auto result {this->operation_array / this->iota_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ValueInverseDivisionOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                   std::begin(correct), [v = this->operation_value](const auto& e){return v / e;});
    const auto result {this->operation_value / this->iota_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, MatrixResidueOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_matrix),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs % rhs;});
    const auto result {this->iota_matrix % this->operation_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ArrayResidueOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs % rhs;});
    const auto result {this->iota_matrix % this->operation_array};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ValueResidueOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                   std::begin(correct), [v = this->operation_value](const auto& e){return e % v;});
    const auto result {this->iota_matrix % this->operation_value};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ArrayInverseResidueOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return rhs % lhs;});
    const auto result {this->operation_array % this->iota_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ValueInverseResidueOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                   std::begin(correct), [v = this->operation_value](const auto& e){return v % e;});
    const auto result {this->operation_value % this->iota_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, MatrixBitwiseAndOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_matrix),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs / rhs;});
    const auto result {this->iota_matrix / this->operation_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ArrayBitwiseAndOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs / rhs;});
    const auto result {this->iota_matrix / this->operation_array};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ValueBitwiseAndOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                   std::begin(correct), [v = this->operation_value](const auto& e){return e / v;});
    const auto result {this->iota_matrix / this->operation_value};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ArrayInverseBitwiseAndOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return rhs / lhs;});
    const auto result {this->operation_array / this->iota_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ValueInverseBitwiseAndOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                   std::begin(correct), [v = this->operation_value](const auto& e){return v / e;});
    const auto result {this->operation_value / this->iota_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, MatrixBitwiseOrOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_matrix),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs | rhs;});
    const auto result {this->iota_matrix | this->operation_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ArrayBitwiseOrOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs | rhs;});
    const auto result {this->iota_matrix | this->operation_array};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ValueBitwiseOrOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                   std::begin(correct), [v = this->operation_value](const auto& e){return e | v;});
    const auto result {this->iota_matrix | this->operation_value};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ArrayInverseBitwiseOrOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return rhs | lhs;});
    const auto result {this->operation_array | this->iota_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ValueInverseBitwiseOrOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                   std::begin(correct), [v = this->operation_value](const auto& e){return v | e;});
    const auto result {this->operation_value | this->iota_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, MatrixXorOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_matrix),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs ^ rhs;});
    const auto result {this->iota_matrix ^ this->operation_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ArrayXorOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs ^ rhs;});
    const auto result {this->iota_matrix ^ this->operation_array};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ValueXorOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                   std::begin(correct), [v = this->operation_value](const auto& e){return e ^ v;});
    const auto result {this->iota_matrix ^ this->operation_value};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ArrayInverseXorOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return rhs ^ lhs;});
    const auto result {this->operation_array ^ this->iota_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ValueInverseXorOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                   std::begin(correct), [v = this->operation_value](const auto& e){return v ^ e;});
    const auto result {this->operation_value ^ this->iota_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, MatrixShiftOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_matrix),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs / rhs;});
    const auto result {this->iota_matrix / this->operation_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ArrayShiftOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs << rhs;});
    const auto result {this->iota_matrix << this->operation_array};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ValueShiftOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                   std::begin(correct), [v = this->operation_value](const auto& e){return e << v;});
    const auto result {this->iota_matrix << this->operation_value};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ArrayInverseShiftOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return rhs << lhs;});
    const auto result {this->operation_array << this->iota_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ValueInverseShiftOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                   std::begin(correct), [v = this->operation_value](const auto& e){return v << e;});
    const auto result {this->operation_value << this->iota_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, MatrixCounterShiftOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_matrix),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs >> rhs;});
    const auto result {this->iota_matrix >> this->operation_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ArrayCounterShiftOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return lhs >> rhs;});
    const auto result {this->iota_matrix >> this->operation_array};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ValueCounterShiftOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                   std::begin(correct), [v = this->operation_value](const auto& e){return e >> v;});
    const auto result {this->iota_matrix >> this->operation_value};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ArrayInverseCounterShiftOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array), std::begin(this->operation_array),
                   std::begin(correct), [](const auto& lhs, const auto& rhs){return rhs >> lhs;});
    const auto result {this->operation_array >> this->iota_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ValueInverseCounterShiftOperator)
{
  if constexpr (!std::is_floating_point_v<TypeParam>) {
    typename TestFixture::ValidatorType correct(this->iota_array.size());
    std::transform(std::begin(this->iota_array), std::end(this->iota_array),
                   std::begin(correct), [v = this->operation_value](const auto& e){return v >> e;});
    const auto result {this->operation_value >> this->iota_matrix};
    EXPECT_EQ(correct, as_validator(result));
  }
}

TYPED_TEST(ValmatrixTest, ReadRow)
{
  for (auto i {0u}; i < TestFixture::size.second; ++i) {
    typename TestFixture::Valarray value_raw {this->iota_matrix.row(i)};
    typename TestFixture::Valarray correct_raw {this->iota_array[std::slice{i * TestFixture::size.first, TestFixture::size.first, 1}]};
    EXPECT_EQ(as_validator(correct_raw), as_validator(value_raw));
  }
}

TYPED_TEST(ValmatrixTest, ReadColumn)
{
  for (auto i {0u}; i < TestFixture::size.first; ++i) {
    typename TestFixture::Valarray value_raw {this->iota_matrix.col(i)};
    typename TestFixture::Valarray correct_raw {this->iota_array[std::slice{i, TestFixture::size.second, TestFixture::size.first}]};
    EXPECT_EQ(as_validator(correct_raw), as_validator(value_raw));
  }
}

TYPED_TEST(ValmatrixTest, WriteRow)
{
  constexpr auto new_value {5};
  this->iota_matrix.row(TestFixture::size.second - 1) = new_value;
  this->iota_array[std::slice{TestFixture::size.first * (TestFixture::size.second - 1), TestFixture::size.first, 1}] = new_value;

  for (auto i {0u}; i < TestFixture::size.first; ++i) {
    typename TestFixture::Valarray value_raw {this->iota_matrix.col(i)};
    typename TestFixture::Valarray correct_raw {this->iota_array[std::slice{i, TestFixture::size.second, TestFixture::size.first}]};
    EXPECT_EQ(as_validator(correct_raw), as_validator(value_raw));
  }
}

TYPED_TEST(ValmatrixTest, WriteColumn)
{
  constexpr auto new_value {5};
  this->iota_matrix.col(TestFixture::size.first - 1) = new_value;
  this->iota_array[std::slice{TestFixture::size.first - 1, TestFixture::size.second, TestFixture::size.first}] = new_value;

  for (auto i {0u}; i < TestFixture::size.second; ++i) {
    typename TestFixture::Valarray value_raw {this->iota_matrix.row(i)};
    typename TestFixture::Valarray correct_raw {this->iota_array[std::slice{i * TestFixture::size.first, TestFixture::size.first, 1}]};
    EXPECT_EQ(as_validator(correct_raw), as_validator(value_raw));
  }
}

TYPED_TEST(ValmatrixTest, ReadBlock)
{
  constexpr Size index {1, 0};
  constexpr Size block_size {2, 2};

  typename TestFixture::Valmatrix value {this->iota_matrix.block(index, block_size)};

  const std::gslice specification {
    index.second * TestFixture::size.first + index.first,
      {block_size.second, block_size.first},
      {TestFixture::size.first, 1}};
  typename TestFixture::Valarray correct {this->iota_array[specification]};

  for (auto i {0u}; i < size_of(block_size); ++i)
    EXPECT_EQ(value[i], correct[i]);
}

//TYPED_TEST(ValmatrixTest, WriteBlock)
//{
//  constexpr Size index {2, 3};
//  constexpr Size block_size {4, 3};
//  constexpr auto new_value {1234};
//
//  this->iota_matrix.block(index, block_size) = new_value;
//
//  const std::gslice specification {
//    index.first * this->iota_matrix.row_size() + index.second,
//    {block_size.first, block_size.second},
//    {this->iota_matrix.row_size(), 1}};
//  this->iota_array[specification] = new_value;
//
//  for (auto i {0}; i < size_of(TestFixture::size); ++i)
//    EXPECT_EQ(this->iota_matrix[i], this->iota_array[i]);
//}

TYPED_TEST(ValmatrixTest, IteratorAccess)
{
  EXPECT_EQ(this->iota_matrix.begin(), begin(this->iota_matrix)); // ADL test
  EXPECT_EQ(this->iota_matrix.begin(), std::begin(this->iota_matrix)); // std::begin test
  EXPECT_EQ(this->iota_matrix.end(), end(this->iota_matrix)); // ALD test
  EXPECT_EQ(this->iota_matrix.end(), std::end(this->iota_matrix)); // std::end test
}

TYPED_TEST(ValmatrixTest, VoidSwap)
{
  // std::swap test
  typename TestFixture::Valmatrix swap_target {};
  std::swap(this->iota_matrix, swap_target);

  EXPECT_FALSE(this->iota_matrix.size());
  for (auto i {0u}; i < size_of(TestFixture::size); ++i)
    EXPECT_EQ(swap_target[i], this->iota_array[i]);

  // ADL test
  swap(swap_target, swap_target);
  for (auto i {0u}; i < size_of(TestFixture::size); ++i)
    EXPECT_EQ(swap_target[i], this->iota_array[i]);

  // member swap
  typename TestFixture::Valmatrix{}.swap(swap_target);
  EXPECT_FALSE(swap_target.size());
}

namespace
{

  class C
  {
  };

}

TEST(ValmatrixUserClassTest, Construct)
{
  xmaho::std_ext::valmatrix<C>{};
  xmaho::std_ext::valmatrix<C>{1, 1};
  xmaho::std_ext::valmatrix<C>{2, 2};
  xmaho::std_ext::valmatrix<C>{10, 10};
}
