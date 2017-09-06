#include "xmaho/std_ext/valmatrix.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <numeric>
#include <random>
#include <utility>

#include <gtest/gtest.h>

namespace
{

using Valmatrixi = xmaho::std_ext::valmatrix<int>;
using Valarrayi = std::valarray<int>;
using Size = std::pair<std::size_t, std::size_t>;

constexpr std::size_t size(Size s)
{
  return s.first * s.second;
}

constexpr Size iota_size {10, 10};
static_assert(size(iota_size) == 100, "Precondition: iota size by above value");

constexpr Size assign_size {2, 2};
static_assert(size(assign_size) == 4, "Preconfition: assign test size by above value");
constexpr auto base_bias {1000};
constexpr auto threshold {5};

class C
{
};

using Valmatrixc = xmaho::std_ext::valmatrix<C>;
using Valarrayc = std::valarray<C>;

}

class ValmatrixTest
  : public ::testing::Test
{
protected:
  Valmatrixi iota_matrix {iota_size.first, iota_size.second};
  Valarrayi iota_array;

  ValmatrixTest()
    : iota_array(size(iota_size))
  {
    assert(iota_matrix.size() == iota_array.size());
    std::iota(std::begin(iota_matrix), std::end(iota_matrix), 0);
    std::iota(std::begin(iota_array), std::end(iota_array), 0);
    for (auto i {0}; i < iota_array.size(); ++i)
      assert(iota_array[i] == i);
  }
};

class ValmatrixOperatorWithValmatrixTest
  : public ::testing::Test
{
protected:
  Valmatrixi base_matrix {base_bias, assign_size.first, assign_size.second};
  Valmatrixi values;
  Valarrayi base_array;

  ValmatrixOperatorWithValmatrixTest()
    : values(assign_size.first, assign_size.second),
      base_array(base_bias, size(assign_size))
  {
    std::default_random_engine rand {std::random_device{}()};
    std::uniform_int_distribution<> dist {1, threshold};
    for (auto& e : values)
      e = dist(rand);
  }
};

class ValmatrixOperatorWithValarrayTest
  : public ::testing::Test
{
protected:
  Valmatrixi base_matrix {base_bias, assign_size.first, assign_size.second};
  Valarrayi values;
  Valarrayi base_array;

  ValmatrixOperatorWithValarrayTest()
    : values(size(assign_size)),
      base_array(base_bias, size(assign_size))
  {
    std::default_random_engine rand {std::random_device{}()};
    std::uniform_int_distribution<> dist {1, threshold};
    for (auto& e : values)
      e = dist(rand);
  }
};

class ValmatrixOperatorWithValueTest
  : public ::testing::Test
{
protected:
  Valmatrixi base_matrix {base_bias, assign_size.first, assign_size.second};
  Valmatrixi::value_type value;
  Valmatrixi::value_type base_value {base_bias};

  ValmatrixOperatorWithValueTest()
  {
    base_matrix = base_bias;
    std::random_device rand {};
    value = std::uniform_int_distribution<>{1, threshold}(rand);
  }
};

TEST(ValmatrixConstruct, AllConstructors)
{
  const Valmatrixi zero {}; // Default construct
  ASSERT_FALSE(zero.size());
  const Valmatrixi one {1, 1}; // mim size
  ASSERT_EQ(one.size(), 1);
  ASSERT_EQ(one[0], 0);

  constexpr Size diff_size {2, 5};
  const Valmatrixi diffsize_mat {diff_size.first, diff_size.second}; // different size
  ASSERT_EQ(diffsize_mat.size(), size(diff_size));

  constexpr Size zero_size {100, 0};
  const Valmatrixi zero_mat {zero_size.first, zero_size.second};
  ASSERT_EQ(zero_mat.size(), size(zero_size));

  constexpr Size square_size {2, 2};
  constexpr auto default_val {250};
  const Valmatrixi def_val_mat {default_val, square_size.first, square_size.second};
  ASSERT_EQ(def_val_mat.size(), size(square_size));
  for (auto i {0}; i < size(square_size); ++i)
    ASSERT_EQ(def_val_mat[i], default_val);

  Valarrayi values {5, 9, 2, 4, 6, 7};
  constexpr Size def_size {3, 2};
  const Valmatrixi mat_with_def {values, def_size.first, def_size.second};
  ASSERT_EQ(mat_with_def.size(), size(def_size));
  for (auto i {0}; i < size(def_size); ++i)
    ASSERT_EQ(mat_with_def[i], values[i]);
}

TEST(ValmatrixSize, RowAndColSizeCheck)
{
  const Valmatrixi zero {100, 0};
  ASSERT_EQ(zero.row_size(), 0);
  ASSERT_EQ(zero.col_size(), 0);

  const Valmatrixi normal {5, 9};
  ASSERT_EQ(normal.row_size(), 5);
  ASSERT_EQ(normal.col_size(), 9);
}

TEST_F(ValmatrixTest, ReadValueByIndex)
{
  for (auto i {0}; i < size(iota_size); ++i)
    ASSERT_EQ(iota_matrix[i], iota_array[i]);
}

TEST_F(ValmatrixTest, ReadArrayBySlice)
{
  constexpr auto start {0};
  constexpr auto length {12};
  constexpr auto stride {7};
  static_assert(start + length * stride < size(iota_size));
  std::slice specification {start, length, stride};

  const Valarrayi value {iota_matrix[specification]};
  const Valarrayi correct_value {iota_array[specification]};
  ASSERT_EQ(value.size(), correct_value.size());
  for (auto i {0}; i < value.size(); ++i)
    ASSERT_EQ(value[i], correct_value[i]);
}

TEST_F(ValmatrixTest, ReadArrayByGslice)
{
  constexpr auto start {3};
  const std::valarray<std::size_t> length {4, 4};
  const std::valarray<std::size_t> stride {iota_size.second, 1};
  assert(start + length[0] * stride[0] + length[1] * stride[1] < size(iota_size));
  std::gslice specification {start, length, stride};

  const Valarrayi value {iota_matrix[specification]};
  const Valarrayi correct_value {iota_array[specification]};
  ASSERT_EQ(value.size(), correct_value.size());
  for (auto i {0}; i < value.size(); ++i)
    ASSERT_EQ(value[i], correct_value[i]);
}

TEST_F(ValmatrixTest, ReadArrayByMask)
{
  std::valarray<bool> specification(size(iota_size));

  std::default_random_engine rand {std::random_device{}()};
  std::bernoulli_distribution dist {0.7};
  for (auto& e : specification)
    e = dist(rand);

  const Valarrayi value {iota_matrix[specification]};
  const Valarrayi correct_value {iota_array[specification]};
  ASSERT_EQ(value.size(), correct_value.size());
  for (auto i {0}; i < value.size(); ++i)
    ASSERT_EQ(value[i], correct_value[i]);
}

TEST_F(ValmatrixTest, ReadArrayByIndirect)
{
  std::default_random_engine rand {std::random_device{}()};
  std::uniform_int_distribution<> size_dist {0, size(iota_size)};
  std::valarray<std::size_t> specification(size_dist(rand));
  std::sample(std::begin(iota_array), std::end(iota_array), std::begin(specification), specification.size(), rand);

  const Valarrayi value {iota_matrix[specification]};
  const Valarrayi correct_value {iota_array[specification]};
  ASSERT_EQ(value.size(), correct_value.size());
  for (auto i {0}; i < value.size(); ++i)
    ASSERT_EQ(value[i], correct_value[i]);
}

TEST_F(ValmatrixTest, ReadValueByPosition)
{
  for (auto i {0}; i < iota_size.first; ++i)
    for (auto j {0}; j < iota_size.second; ++j) {
      const auto value {iota_matrix[Valmatrixi::position_type{i, j}]};
      ASSERT_EQ(value, iota_array[i * iota_size.second + j]);
    }
}

TEST_F(ValmatrixTest, UnaryAddOperation)
{
  const Valmatrixi effected {+iota_matrix};
  for (auto i {0}; i < size(iota_size); ++i)
    ASSERT_EQ(effected[i], iota_array[i]);
}

TEST_F(ValmatrixTest, UnarySubOperation)
{
  const Valmatrixi effected {-iota_matrix};
  const Valarrayi correct = -iota_array;
  for (auto i {0}; i < size(iota_size); ++i)
    ASSERT_EQ(effected[i], correct[i]);
}

TEST_F(ValmatrixTest, UnaryNegationOperation)
{
  const Valmatrixi effected {~iota_matrix};
  const Valarrayi correct = ~iota_array;
  for (auto i {0}; i < size(iota_size); ++i)
    ASSERT_EQ(effected[i], correct[i]);
}

TEST_F(ValmatrixOperatorWithValmatrixTest, AdditionAssign)
{
  base_matrix += values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i] + values[i]);
}

TEST_F(ValmatrixOperatorWithValarrayTest, AdditionAssign)
{
  base_matrix += values;
  base_array += values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i]);
}

TEST_F(ValmatrixOperatorWithValueTest, AdditionAssign)
{
  base_matrix += value;
  const auto correct {base_value + value};
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], correct);
}

TEST_F(ValmatrixOperatorWithValmatrixTest, SubtractAssign)
{
  base_matrix -= values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i] - values[i]);
}

TEST_F(ValmatrixOperatorWithValarrayTest, SubtractAssign)
{
  base_matrix -= values;
  base_array -= values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i]);
}

TEST_F(ValmatrixOperatorWithValueTest, SubtractAssign)
{
  base_matrix -= value;
  const auto correct {base_value - value};
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], correct);
}

TEST_F(ValmatrixOperatorWithValmatrixTest, MultiplicationAssign)
{
  base_matrix *= values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i] * values[i]);
}

TEST_F(ValmatrixOperatorWithValarrayTest, MultiplicationAssign)
{
  base_matrix *= values;
  base_array *= values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i]);
}

TEST_F(ValmatrixOperatorWithValueTest, MultiplicationAssign)
{
  base_matrix *= value;
  const auto correct {base_value * value};
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], correct);
}

TEST_F(ValmatrixOperatorWithValmatrixTest, DivisionAssign)
{
  base_matrix /= values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i] / values[i]);
}

TEST_F(ValmatrixOperatorWithValarrayTest, DivisionAssign)
{
  base_matrix /= values;
  base_array /= values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i]);
}

TEST_F(ValmatrixOperatorWithValueTest, DivisionAssign)
{
  base_matrix /= value;
  const auto correct {base_value / value};
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], correct);
}

TEST_F(ValmatrixOperatorWithValmatrixTest, ResidueAssign)
{
  base_matrix %= values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i] % values[i]);
}

TEST_F(ValmatrixOperatorWithValarrayTest, ResidueAssign)
{
  base_matrix %= values;
  base_array %= values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i]);
}

TEST_F(ValmatrixOperatorWithValueTest, ResidueAssign)
{
  base_matrix %= value;
  const auto correct {base_value % value};
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], correct);
}

TEST_F(ValmatrixOperatorWithValmatrixTest, BitwiseAndAssign)
{
  base_matrix &= values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i] & values[i]);
}

TEST_F(ValmatrixOperatorWithValarrayTest, BitwiseAndAssign)
{
  base_matrix &= values;
  base_array &= values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i]);
}

TEST_F(ValmatrixOperatorWithValueTest, BitwiseAndAssign)
{
  base_matrix &= value;
  const auto correct {base_value & value};
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], correct);
}

TEST_F(ValmatrixOperatorWithValmatrixTest, BitwiseOrAssign)
{
  base_matrix |= values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i] | values[i]);
}

TEST_F(ValmatrixOperatorWithValarrayTest, BitwiseOrAssign)
{
  base_matrix |= values;
  base_array |= values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i]);
}

TEST_F(ValmatrixOperatorWithValueTest, BitwiseOrAssign)
{
  base_matrix |= value;
  const auto correct {base_value | value};
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], correct);
}

TEST_F(ValmatrixOperatorWithValmatrixTest, XorAssign)
{
  base_matrix ^= values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i] ^ values[i]);
}

TEST_F(ValmatrixOperatorWithValarrayTest, XorAssign)
{
  base_matrix ^= values;
  base_array ^= values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i]);
}

TEST_F(ValmatrixOperatorWithValueTest, XorAssign)
{
  base_matrix ^= value;
  const auto correct {base_value ^ value};
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], correct);
}

TEST_F(ValmatrixOperatorWithValmatrixTest, ShiftAssign)
{
  base_matrix <<= values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i] << values[i]);
}

TEST_F(ValmatrixOperatorWithValarrayTest, ShiftAssign)
{
  base_matrix <<= values;
  base_array <<= values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i]);
}

TEST_F(ValmatrixOperatorWithValueTest, ShiftAssign)
{
  base_matrix <<= value;
  const auto correct {base_value << value};
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], correct);
}

TEST_F(ValmatrixOperatorWithValmatrixTest, CounterShiftAssign)
{
  base_matrix >>= values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i] >> values[i]);
}

TEST_F(ValmatrixOperatorWithValarrayTest, CounterShiftAssign)
{
  base_matrix >>= values;
  base_array >>= values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i]);
}

TEST_F(ValmatrixOperatorWithValueTest, CounterShiftAssign)
{
  base_matrix >>= value;
  const auto correct {base_value >> value};
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], correct);
}

TEST_F(ValmatrixTest, ReadRow)
{
  for (auto i {0}; i < iota_size.first; ++i) {
    Valarrayi value {iota_matrix.row(i)};
    Valarrayi correct {iota_array[std::slice{i * iota_size.second, iota_size.second, 1}]};
    ASSERT_EQ(value.size(), correct.size());
    for (auto j {0}; j < iota_size.second; ++j)
      ASSERT_EQ(value[j], correct[j]);
  }
}

TEST_F(ValmatrixTest, ReadColumn)
{
  for (auto i {0u}; i < iota_size.second; ++i) {
    Valarrayi value {iota_matrix.col(i)};
    Valarrayi correct {iota_array[std::slice{i, iota_size.first, iota_size.second}]};
    ASSERT_EQ(value.size(), correct.size());
    for (auto j {0}; j < iota_size.first; ++j)
      ASSERT_EQ(value[j], correct[j]);
  }
}

TEST_F(ValmatrixTest, WriteRow)
{
  constexpr auto new_value {5};
  iota_matrix.row(iota_size.first - 1) = new_value;
  iota_array[std::slice{iota_size.first * (iota_size.second - 1), iota_size.second, 1}] = new_value;

  for (auto i {0}; i < iota_size.first; ++i) {
    Valarrayi value {iota_matrix.row(i)};
    Valarrayi correct {iota_array[std::slice{i * iota_size.second, iota_size.second, 1}]};
    ASSERT_EQ(value.size(), correct.size());
    for (auto j {0}; j < iota_size.second; ++j)
      ASSERT_EQ(value[j], correct[j]);
  }
}

TEST_F(ValmatrixTest, WriteColumn)
{
  constexpr auto new_value {5};
  iota_matrix.col(iota_size.second - 1) = new_value;
  iota_array[std::slice{iota_size.second - 1, iota_size.first, iota_size.second}] = new_value;

  for (auto i {0u}; i < iota_size.second; ++i) {
    Valarrayi value {iota_matrix.col(i)};
    Valarrayi correct {iota_array[std::slice{i, iota_size.first, iota_size.second}]};
    ASSERT_EQ(value.size(), correct.size());
    for (auto j {0}; j < iota_size.first; ++j)
      ASSERT_EQ(value[j], correct[j]);
  }
}

TEST_F(ValmatrixTest, IteratorAccess)
{
  ASSERT_EQ(iota_matrix.begin(), begin(iota_matrix)); // ADL test
  ASSERT_EQ(iota_matrix.begin(), std::begin(iota_matrix)); // std::begin test
  ASSERT_EQ(iota_matrix.end(), end(iota_matrix)); // ALD test
  ASSERT_EQ(iota_matrix.end(), std::end(iota_matrix)); // std::end test
}

TEST_F(ValmatrixTest, VoidSwap)
{
  // std::swap test
  Valmatrixi swap_target {};
  std::swap(iota_matrix, swap_target);

  ASSERT_FALSE(iota_matrix.size());
  for (auto i {0}; i < size(iota_size); ++i)
    ASSERT_EQ(swap_target[i], iota_array[i]);

  // ADL test
  swap(swap_target, swap_target);
  for (auto i {0}; i < size(iota_size); ++i)
    ASSERT_EQ(swap_target[i], iota_array[i]);

  // member swap
  Valmatrixi{}.swap(swap_target);
  ASSERT_FALSE(swap_target.size());
}

TEST(ValmatrixcTest, Construct)
{
  Valmatrixc{};
  Valmatrixc{1, 1};
  Valmatrixc{2, 2};
  Valmatrixc{10, 10};
}
