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

#include <gtest/gtest.h>

namespace
{

using Size = std::pair<std::size_t, std::size_t>;

constexpr std::size_t size_of(const Size& size)
{
  return size.first * size.second;
}

}

template<typename T>
class ValmatrixTest
  : public ::testing::Test
{
protected:
  using Valmatrix = xmaho::std_ext::valmatrix<T>;
  using Valarray = std::valarray<T>;

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
    EXPECT_EQ(iota_matrix.size(), iota_array.size());
    std::iota(std::begin(iota_matrix), std::end(iota_matrix), 1);
    std::iota(std::begin(iota_array), std::end(iota_array), 1);

    std::default_random_engine rand {std::random_device{}()};
    std::uniform_int_distribution<> dist {1, 5};
    for (auto& e : operation_matrix)
      e = dist(rand);
    for (auto& e : operation_array)
      e = dist(rand);
    operation_value = dist(rand);
  }
};

using ValmatrixTypes = ::testing::Types<int, unsigned int, std::size_t>;
TYPED_TEST_CASE(ValmatrixTest, ValmatrixTypes);

template<typename T>
constexpr Size ValmatrixTest<T>::size;

TEST(ValmatrixConstructTest, AllConstructors)
{
  const xmaho::std_ext::valmatrix<int> zero {}; // Default construct
  ASSERT_FALSE(zero.size());
  const xmaho::std_ext::valmatrix<int> one {1, 1}; // mim size
  ASSERT_EQ(one.size(), 1);
  ASSERT_EQ(one[0], 0);

  constexpr Size diff_size {2, 5};
  const xmaho::std_ext::valmatrix<int> diffsize_mat {diff_size.first, diff_size.second}; // different size
  ASSERT_EQ(diffsize_mat.size(), size_of(diff_size));

  constexpr Size zero_size {100, 0};
  const xmaho::std_ext::valmatrix<int> zero_mat {zero_size.first, zero_size.second};
  ASSERT_EQ(zero_mat.size(), size_of(zero_size));

  constexpr Size square_size {2, 2};
  constexpr auto default_val {250};
  const xmaho::std_ext::valmatrix<int> def_val_mat {default_val, square_size.first, square_size.second};
  ASSERT_EQ(def_val_mat.size(), size_of(square_size));
  for (auto i {0}; i < size_of(square_size); ++i)
    ASSERT_EQ(def_val_mat[i], default_val);

  std::valarray<int> values {5, 9, 2, 4, 6, 7};
  constexpr Size def_size {3, 2};
  const xmaho::std_ext::valmatrix<int> mat_with_def {values, def_size.first, def_size.second};
  ASSERT_EQ(mat_with_def.size(), size_of(def_size));
  for (auto i {0}; i < size_of(def_size); ++i)
    ASSERT_EQ(mat_with_def[i], values[i]);
}

TEST(ValmatrixSizeTest, RowAndColSizeCheck)
{
  const xmaho::std_ext::valmatrix<int> zero {100, 0};
  ASSERT_EQ(zero.row_size(), 0);
  ASSERT_EQ(zero.col_size(), 0);

  const xmaho::std_ext::valmatrix<int> normal {5, 9};
  ASSERT_EQ(normal.row_size(), 5);
  ASSERT_EQ(normal.col_size(), 9);
}

TYPED_TEST(ValmatrixTest, ReadValueByIndex)
{
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ReadArrayBySlice)
{
  constexpr auto start {0u};
  constexpr auto length {3u};
  constexpr auto stride {2u};
  static_assert(start + (length - 1) * stride < size_of(TestFixture::size), "Over size");
  std::slice specification {start, length, stride};

  const typename TestFixture::Valarray value {this->iota_matrix[specification]};
  const typename TestFixture::Valarray correct_value {this->iota_array[specification]};
  ASSERT_EQ(value.size(), correct_value.size());
  for (auto i {0}; i < value.size(); ++i)
    ASSERT_EQ(value[i], correct_value[i]);
}

TYPED_TEST(ValmatrixTest, ReadArrayByGslice)
{
  constexpr auto start {1u};
  const std::valarray<std::size_t> length {2, 2};
  const std::valarray<std::size_t> stride {3, 1};
  assert(start + (length[0] - 1) * stride[0] + (length[1] - 1) * stride[1] < size_of(TestFixture::size));
  std::gslice specification {start, length, stride};

  const typename TestFixture::Valarray value {this->iota_matrix[specification]};
  const typename TestFixture::Valarray correct_value {this->iota_array[specification]};
  ASSERT_EQ(value.size(), correct_value.size());
  for (auto i {0}; i < value.size(); ++i)
    ASSERT_EQ(value[i], correct_value[i]);
}

TYPED_TEST(ValmatrixTest, ReadArrayByMask)
{
  std::valarray<bool> specification(size_of(TestFixture::size));

  std::default_random_engine rand {std::random_device{}()};
  std::bernoulli_distribution dist {0.7};
  for (auto& e : specification)
    e = dist(rand);

  const typename TestFixture::Valarray value {this->iota_matrix[specification]};
  const typename TestFixture::Valarray correct_value {this->iota_array[specification]};
  ASSERT_EQ(value.size(), correct_value.size());
  for (auto i {0}; i < value.size(); ++i)
    ASSERT_EQ(value[i], correct_value[i]);
}

TYPED_TEST(ValmatrixTest, ReadArrayByIndirect)
{
  std::default_random_engine rand {std::random_device{}()};
  std::uniform_int_distribution<> size_dist {0, static_cast<int>(size_of(TestFixture::size))};
  std::valarray<std::size_t> specification(size_dist(rand));
  std::uniform_int_distribution<> index_dist {0, static_cast<int>(size_of(TestFixture::size) - 1)};
  std::generate(std::begin(specification), std::end(specification), [&index_dist, &rand]{return index_dist(rand);});

  const typename TestFixture::Valarray value {this->iota_matrix[specification]};
  const typename TestFixture::Valarray correct_value {this->iota_array[specification]};
  ASSERT_EQ(value.size(), correct_value.size());
  for (auto i {0}; i < value.size(); ++i)
    ASSERT_EQ(value[i], correct_value[i]);
}

TYPED_TEST(ValmatrixTest, ReadValueByPosition)
{
  for (auto i {0}; i < TestFixture::size.first; ++i)
    for (auto j {0}; j < TestFixture::size.second; ++j) {
      const auto value {this->iota_matrix[typename TestFixture::Valmatrix::position_type{i, j}]};
      ASSERT_EQ(value, this->iota_array[j * TestFixture::size.second + i]);
    }
}

TYPED_TEST(ValmatrixTest, UnaryAddOperation)
{
  const typename TestFixture::Valmatrix effected {+this->iota_matrix};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(effected[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, UnarySubOperation)
{
  if constexpr (std::is_signed_v<TypeParam>) {
    const typename TestFixture::Valmatrix effected{ -this->iota_matrix };
    const typename TestFixture::Valarray correct = -this->iota_array;
    for (auto i{ 0 }; i < size_of(TestFixture::size); ++i)
      ASSERT_EQ(effected[i], correct[i]);
  }
}

TYPED_TEST(ValmatrixTest, UnaryNegationOperation)
{
  const typename TestFixture::Valmatrix effected {~this->iota_matrix};
  const typename TestFixture::Valarray correct = ~this->iota_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(effected[i], correct[i]);
}

TYPED_TEST(ValmatrixTest, AdditionAssign)
{
  this->iota_matrix += this->operation_matrix;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i] + this->operation_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ArrayAdditionAssign)
{
  this->iota_matrix += this->operation_array;
  this->iota_array += this->operation_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueAdditionAssign)
{
  this->iota_matrix += this->operation_value;
  this->iota_array += this->operation_value;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, MatrixSubtractAssign)
{
  this->iota_matrix -= this->operation_matrix;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i] - this->operation_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ArraySubtractAssign)
{
  this->iota_matrix -= this->operation_array;
  this->iota_array -= this->operation_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueSubtractAssign)
{
  this->iota_matrix -= this->operation_value;
  this->iota_array -= this->operation_value;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, MatrixMultiplicationAssign)
{
  this->iota_matrix *= this->operation_matrix;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i] * this->operation_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ArrayMultiplicationAssign)
{
  this->iota_matrix *= this->operation_array;
  this->iota_array *= this->operation_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueMultiplicationAssign)
{
  this->iota_matrix *= this->operation_value;
  this->iota_array *= this->operation_value;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, MatrixDivisionAssign)
{
  this->iota_matrix /= this->operation_matrix;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i] / this->operation_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ArrayDivisionAssign)
{
  this->iota_matrix /= this->operation_array;
  this->iota_array /= this->operation_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueDivisionAssign)
{
  this->iota_matrix /= this->operation_value;
  this->iota_array /= this->operation_value;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, MatrixResidueAssign)
{
  this->iota_matrix %= this->operation_matrix;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i] % this->operation_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ArrayResidueAssign)
{
  this->iota_matrix %= this->operation_array;
  this->iota_array %= this->operation_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueResidueAssign)
{
  this->iota_matrix %= this->operation_value;
  this->iota_array %= this->operation_value;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, MatrixBitwiseAndAssign)
{
  this->iota_matrix &= this->operation_matrix;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i] & this->operation_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ArrayBitwiseAndAssign)
{
  this->iota_matrix &= this->operation_array;
  this->iota_array &= this->operation_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueBitwiseAndAssign)
{
  this->iota_matrix &= this->operation_value;
  this->iota_array &= this->operation_value;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, MatrixBitwiseOrAssign)
{
  this->iota_matrix |= this->operation_matrix;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i] | this->operation_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ArrayBitwiseOrAssign)
{
  this->iota_matrix |= this->operation_array;
  this->iota_array |= this->operation_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueBitwiseOrAssign)
{
  this->iota_matrix |= this->operation_value;
  this->iota_array |= this->operation_value;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, MatrixXorAssign)
{
  this->iota_matrix ^= this->operation_matrix;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i] ^ this->operation_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ArrayXorAssign)
{
  this->iota_matrix ^= this->operation_array;
  this->iota_array ^= this->operation_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueXorAssign)
{
  this->iota_matrix ^= this->operation_value;
  this->iota_array ^= this->operation_value;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, MatrixShiftAssign)
{
  this->iota_matrix <<= this->operation_matrix;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i] << this->operation_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ArrayShiftAssign)
{
  this->iota_matrix <<= this->operation_array;
  this->iota_array <<= this->operation_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueShiftAssign)
{
  this->iota_matrix <<= this->operation_value;
  this->iota_array <<= this->operation_value;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, MatrixCounterShiftAssign)
{
  this->iota_matrix >>= this->operation_matrix;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i] >> this->operation_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ArrayCounterShiftAssign)
{
  this->iota_matrix >>= this->operation_array;
  this->iota_array >>= this->operation_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueCounterShiftAssign)
{
  this->iota_matrix >>= this->operation_value;
  this->iota_array >>= this->operation_value;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, MatrixAdditionOperator)
{
  const auto result {this->iota_matrix + this->operation_matrix};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i] + this->operation_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ArrayAdditionOperator)
{
  const auto result {this->iota_matrix + this->operation_array};
  this->iota_array += this->operation_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueAdditionOperator)
{
  const auto result {this->iota_matrix + this->operation_value};
  this->iota_array += this->operation_value;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ArrayInverseAdditionOperator)
{
  const auto result {this->operation_array + this->iota_matrix};
  this->operation_array += this->iota_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->operation_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueInverseAdditionOperator)
{
  const auto result {this->operation_value + this->iota_matrix};
  const auto correct {this->operation_value + this->iota_array};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], correct[i]);
}

TYPED_TEST(ValmatrixTest, MatrixSubtractOperator)
{
  const auto result {this->iota_matrix - this->operation_matrix};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i] - this->operation_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ArraySubtractOperator)
{
  const auto result {this->iota_matrix - this->operation_array};
  this->iota_array -= this->operation_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueSubtractOperator)
{
  const auto result {this->iota_matrix - this->operation_value};
  this->iota_array -= this->operation_value;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ArrayInverseSubtractOperator)
{
  const auto result {this->operation_array - this->iota_matrix};
  this->operation_array -= this->iota_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->operation_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueInverseSubtractOperator)
{
  const auto result {this->operation_value - this->iota_matrix };
  const auto correct {this->operation_value - this->iota_array};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], correct[i]);
}

TYPED_TEST(ValmatrixTest, MatrixMultiplicationOperator)
{
  const auto result {this->iota_matrix * this->operation_matrix};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i] * this->operation_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ArrayMultiplicationOperator)
{
  const auto result {this->iota_matrix * this->operation_array};
  this->iota_array *= this->operation_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueMultiplicationOperator)
{
  const auto result {this->iota_matrix * this->operation_value};
  this->iota_array *= this->operation_value;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ArrayInverseMultiplicationOperator)
{
  const auto result {this->operation_array * this->iota_matrix};
  this->operation_array *= this->iota_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->operation_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueInverseMultiplicationOperator)
{
  const auto result {this->operation_value * this->iota_matrix};
  const auto correct {this->operation_value * this->iota_array};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], correct[i]);
}

TYPED_TEST(ValmatrixTest, MatrixDivisionOperator)
{
  const auto result {this->iota_matrix / this->operation_matrix};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i] / this->operation_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ArrayDivisionOperator)
{
  const auto result {this->iota_matrix / this->operation_array};
  this->iota_array /= this->operation_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueDivisionOperator)
{
  const auto result {this->iota_matrix / this->operation_value};
  this->iota_array /= this->operation_value;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ArrayInverseDivisionOperator)
{
  const auto result {this->operation_array / this->iota_matrix};
  this->operation_array /= this->iota_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->operation_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueInverseDivisionOperator)
{
  const auto result {this->operation_value / this->iota_matrix};
  const auto correct {this->operation_value / this->iota_array};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], correct[i]);
}

TYPED_TEST(ValmatrixTest, MatrixResidueOperator)
{
  const auto result {this->iota_matrix % this->operation_matrix};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i] % this->operation_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ArrayResidueOperator)
{
  const auto result {this->iota_matrix % this->operation_array};
  this->iota_array %= this->operation_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueResidueOperator)
{
  const auto result {this->iota_matrix % this->operation_value};
  this->iota_array %= this->operation_value;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ArrayInverseResidueOperator)
{
  const auto result {this->operation_array % this->iota_matrix};
  this->operation_array %= this->iota_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->operation_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueInverseResidueOperator)
{
  const auto result {this->operation_value % this->iota_matrix};
  const auto correct {this->operation_value % this->iota_array};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], correct[i]);
}

TYPED_TEST(ValmatrixTest, MatrixBitwiseAndOperator)
{
  const auto result {this->iota_matrix & this->operation_matrix};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i] & this->operation_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ArrayBitwiseAndOperator)
{
  const auto result {this->iota_matrix & this->operation_array};
  this->iota_array &= this->operation_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueBitwiseAndOperator)
{
  const auto result {this->iota_matrix & this->operation_value};
  this->iota_array &= this->operation_value;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ArrayInverseBitwiseAndOperator)
{
  const auto result {this->operation_array & this->iota_matrix};
  this->operation_array &= this->iota_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->operation_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueInverseBitwiseAndOperator)
{
  const auto result {this->operation_value & this->iota_matrix};
  const auto correct {this->operation_value & this->iota_array};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], correct[i]);
}

TYPED_TEST(ValmatrixTest, MatrixBitwiseOrOperator)
{
  const auto result {this->iota_matrix | this->operation_matrix};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i] | this->operation_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ArrayBitwiseOrOperator)
{
  const auto result {this->iota_matrix | this->operation_array};
  this->iota_array |= this->operation_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueBitwiseOrOperator)
{
  const auto result {this->iota_matrix | this->operation_value};
  this->iota_array |= this->operation_value;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ArrayInverseBitwiseOrOperator)
{
  const auto result {this->operation_array | this->iota_matrix};
  this->operation_array |= this->iota_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->operation_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueInverseBitwiseOrOperator)
{
  const auto result {this->operation_value | this->iota_matrix};
  const auto correct {this->operation_value | this->iota_array};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], correct[i]);
}

TYPED_TEST(ValmatrixTest, MatrixXorOperator)
{
  const auto result {this->iota_matrix ^ this->operation_matrix};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i] ^ this->operation_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ArrayXorOperator)
{
  const auto result {this->iota_matrix ^ this->operation_array};
  this->iota_array ^= this->operation_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueXorOperator)
{
  const auto result {this->iota_matrix ^ this->operation_value};
  this->iota_array ^= this->operation_value;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ArrayInverseXorOperator)
{
  const auto result {this->operation_array ^ this->iota_matrix};
  this->operation_array ^= this->iota_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->operation_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueInverseXorOperator)
{
  const auto result {this->operation_value ^ this->iota_matrix};
  const auto correct {this->operation_value ^ this->iota_array};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], correct[i]);
}

TYPED_TEST(ValmatrixTest, MatrixShiftOperator)
{
  const auto result {this->iota_matrix << this->operation_matrix};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i] << this->operation_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ArrayShiftOperator)
{
  const auto result {this->iota_matrix << this->operation_array};
  this->iota_array <<= this->operation_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueShiftOperator)
{
  const auto result {this->iota_matrix << this->operation_value};
  this->iota_array <<= this->operation_value;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ArrayInverseShiftOperator)
{
  const auto result {this->operation_array << this->iota_matrix};
  this->operation_array <<= this->iota_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->operation_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueInverseShiftOperator)
{
  const auto result {this->operation_value << this->iota_matrix};
  const auto correct {this->operation_value << this->iota_array};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], correct[i]);
}

TYPED_TEST(ValmatrixTest, MatrixCounterShiftOperator)
{
  const auto result {this->iota_matrix >> this->operation_matrix};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i] >> this->operation_matrix[i]);
}

TYPED_TEST(ValmatrixTest, ArrayCounterShiftOperator)
{
  const auto result {this->iota_matrix >> this->operation_array};
  this->iota_array >>= this->operation_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueCounterShiftOperator)
{
  const auto result {this->iota_matrix >> this->operation_value};
  this->iota_array >>= this->operation_value;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->iota_array[i]);
}

TYPED_TEST(ValmatrixTest, ArrayInverseCounterShiftOperator)
{
  const auto result {this->operation_array >> this->iota_matrix};
  this->operation_array >>= this->iota_array;
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], this->operation_array[i]);
}

TYPED_TEST(ValmatrixTest, ValueInverseCounterShiftOperator)
{
  const auto result {this->operation_value >> this->iota_matrix};
  const auto correct {this->operation_value >> this->iota_array};
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(result[i], correct[i]);
}

TYPED_TEST(ValmatrixTest, ReadRow)
{
  for (auto i {0}; i < TestFixture::size.second; ++i) {
    typename TestFixture::Valarray value {this->iota_matrix.row(i)};
    typename TestFixture::Valarray correct {this->iota_array[std::slice{i * TestFixture::size.first, TestFixture::size.first, 1}]};
    ASSERT_EQ(value.size(), correct.size());
    for (auto j {0}; j < TestFixture::size.first; ++j)
      ASSERT_EQ(value[j], correct[j]);
  }
}

TYPED_TEST(ValmatrixTest, ReadColumn)
{
  for (auto i {0u}; i < TestFixture::size.first; ++i) {
    typename TestFixture::Valarray value {this->iota_matrix.col(i)};
    typename TestFixture::Valarray correct {this->iota_array[std::slice{i, TestFixture::size.second, TestFixture::size.first}]};
    ASSERT_EQ(value.size(), correct.size());
    for (auto j {0}; j < TestFixture::size.second; ++j)
      ASSERT_EQ(value[j], correct[j]);
  }
}

TYPED_TEST(ValmatrixTest, WriteRow)
{
  constexpr auto new_value {5};
  this->iota_matrix.row(TestFixture::size.second - 1) = new_value;
  this->iota_array[std::slice{TestFixture::size.first * (TestFixture::size.second - 1), TestFixture::size.first, 1}] = new_value;

  for (auto i {0}; i < TestFixture::size.second; ++i) {
    typename TestFixture::Valarray value {this->iota_matrix.row(i)};
    typename TestFixture::Valarray correct {this->iota_array[std::slice{i * TestFixture::size.first, TestFixture::size.first, 1}]};
    ASSERT_EQ(value.size(), correct.size());
    for (auto j {0}; j < TestFixture::size.first; ++j)
      ASSERT_EQ(value[j], correct[j]);
  }
}

TYPED_TEST(ValmatrixTest, WriteColumn)
{
  constexpr auto new_value {5};
  this->iota_matrix.col(TestFixture::size.first - 1) = new_value;
  this->iota_array[std::slice{TestFixture::size.first - 1, TestFixture::size.second, TestFixture::size.first}] = new_value;

  for (auto i {0u}; i < TestFixture::size.first; ++i) {
    typename TestFixture::Valarray value {this->iota_matrix.col(i)};
    typename TestFixture::Valarray correct {this->iota_array[std::slice{i, TestFixture::size.second, TestFixture::size.first}]};
    ASSERT_EQ(value.size(), correct.size());
    for (auto j {0}; j < TestFixture::size.second; ++j)
      ASSERT_EQ(value[j], correct[j]);
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

  for (auto i {0}; i < size_of(block_size); ++i)
    ASSERT_EQ(value[i], correct[i]);
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
//    ASSERT_EQ(this->iota_matrix[i], this->iota_array[i]);
//}

TYPED_TEST(ValmatrixTest, IteratorAccess)
{
  ASSERT_EQ(this->iota_matrix.begin(), begin(this->iota_matrix)); // ADL test
  ASSERT_EQ(this->iota_matrix.begin(), std::begin(this->iota_matrix)); // std::begin test
  ASSERT_EQ(this->iota_matrix.end(), end(this->iota_matrix)); // ALD test
  ASSERT_EQ(this->iota_matrix.end(), std::end(this->iota_matrix)); // std::end test
}

TYPED_TEST(ValmatrixTest, VoidSwap)
{
  // std::swap test
  typename TestFixture::Valmatrix swap_target {};
  std::swap(this->iota_matrix, swap_target);

  ASSERT_FALSE(this->iota_matrix.size());
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(swap_target[i], this->iota_array[i]);

  // ADL test
  swap(swap_target, swap_target);
  for (auto i {0}; i < size_of(TestFixture::size); ++i)
    ASSERT_EQ(swap_target[i], this->iota_array[i]);

  // member swap
  typename TestFixture::Valmatrix{}.swap(swap_target);
  ASSERT_FALSE(swap_target.size());
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
