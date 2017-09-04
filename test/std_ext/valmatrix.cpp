#include "xmaho/std_ext/valmatrix.hpp"

#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>

#include <gtest/gtest.h>

namespace
{

using Valmatrixi = xmaho::std_ext::valmatrix<int>;
using Valarrayi = std::valarray<int>;
using Size = Valmatrixi::position_type;

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
  Valmatrixi base_matrix {assign_size.first, assign_size.second};
  Valmatrixi values;
  Valarrayi base_array;

  ValmatrixOperatorWithValmatrixTest()
    : values(assign_size.first, assign_size.second),
      base_array(size(assign_size))
  {
    base_matrix = base_bias;
    base_array = base_bias;
    std::default_random_engine rand {std::random_device{}()};
    std::uniform_int_distribution<> dist {-threshold, threshold};
    for (auto& e : values)
      e = dist(rand);
  }
};

class ValmatrixOperatorWithValarrayTest
  : public ::testing::Test
{
protected:
  Valmatrixi base_matrix {assign_size.first, assign_size.second};
  Valarrayi values;
  Valarrayi base_array;

  ValmatrixOperatorWithValarrayTest()
    : values(size(assign_size)),
      base_array(size(assign_size))
  {
    base_matrix = base_bias;
    base_array = base_bias;
    std::default_random_engine rand {std::random_device{}()};
    std::uniform_int_distribution<> dist {-5, 5};
    for (auto& e : values)
      e = dist(rand);
  }
};

class ValmatrixOperatorWithValueTest
  : public ::testing::Test
{
protected:
  Valmatrixi base_matrix {assign_size.first, assign_size.second};
  Valmatrixi::value_type value;
  Valmatrixi::value_type base_value {base_bias};

  ValmatrixOperatorWithValueTest()
  {
    base_matrix = base_bias;
    std::random_device rand {};
    value = std::uniform_int_distribution<>{-5, 5}(rand);
  }
};

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

TEST_F(ValmatrixOperatorWithValmatrixTest, AddAssign)
{
  base_matrix += values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i] + values[i]);
}

TEST_F(ValmatrixOperatorWithValarrayTest, AddAssign)
{
  base_matrix += values;
  base_array += values;
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], base_array[i]);
}

TEST_F(ValmatrixOperatorWithValueTest, AddAssign)
{
  base_matrix += value;
  const auto correct {base_value + value};
  for (auto i {0}; i < size(assign_size); ++i)
    ASSERT_EQ(base_matrix[i], correct);
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
