/*
BSD 2-Clause License

Copyright (c) 2020, Doi Yusuke
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

#include "xmaho/floating/compare.hpp"

#include <gtest/gtest.h>

TEST(FloatingCompare, FloatValues)
{
  xmaho::floating::equal<float> equal{};
  EXPECT_PRED2(equal, 0.f, 0.f);
  EXPECT_PRED2(equal, 1.f, 1.f);
  EXPECT_PRED2(equal, 10.f, 10.f);
  EXPECT_PRED2(equal, 1e6f, 1e6f);
  {
    const auto value {[](){auto v {0.f}; for (auto i {10u}; i != 0; --i) v += 0.1f; return v;}()};
    EXPECT_PRED2(equal, 1.f, value);
    EXPECT_FLOAT_EQ(1.f, value);
    EXPECT_NE(1.f, value);
  }
}

TEST(FloatingCompare, DoubleValues)
{
  xmaho::floating::equal<double> equal{};
  EXPECT_PRED2(equal, 0., 0.);
  EXPECT_PRED2(equal, 1., 1.);
  EXPECT_PRED2(equal, 10., 10.);
  EXPECT_PRED2(equal, 1e6, 1e6);
  {
    const auto value {[](){auto v {0.}; for (auto i {10u}; i != 0; --i) v += 0.1; return v;}()};
    EXPECT_PRED2(equal, 1., value);
    EXPECT_DOUBLE_EQ(1., value);
    EXPECT_NE(1., value);
  }
}

class FloatingPointNumber
{
public:
  using value_type = float;

  explicit constexpr FloatingPointNumber(value_type v) noexcept : v_ {v} {}

  constexpr bool operator<(const FloatingPointNumber& rhs) const noexcept
  {
    return v_ < rhs.v_;
  }

  constexpr bool operator<=(const FloatingPointNumber& rhs) const noexcept
  {
    return v_ <= rhs.v_;
  }

  constexpr bool operator!=(const FloatingPointNumber& rhs) const noexcept
  {
    return v_ != rhs.v_;
  }

  constexpr FloatingPointNumber operator*(const FloatingPointNumber& rhs) const noexcept
  {
    return FloatingPointNumber{v_ * rhs.v_};
  }

  constexpr FloatingPointNumber& operator+=(const FloatingPointNumber& rhs) noexcept
  {
    v_ += rhs.v_;
    return *this;
  }

  constexpr FloatingPointNumber operator-(const FloatingPointNumber& rhs) const noexcept
  {
    return FloatingPointNumber{v_ - rhs.v_};
  }

  constexpr FloatingPointNumber operator-() const noexcept
  {
    return FloatingPointNumber{-v_};
  }

  explicit constexpr operator value_type() const noexcept
  {
    return v_;
  }

private:
  value_type v_;
};

template<>
constexpr FloatingPointNumber std::numeric_limits<FloatingPointNumber>::epsilon() noexcept
{
  return FloatingPointNumber{numeric_limits<FloatingPointNumber::value_type>::epsilon()};
}

FloatingPointNumber operator"" _fpn(long double v) noexcept;
FloatingPointNumber operator"" _fpn(long double v) noexcept
{
  return FloatingPointNumber{static_cast<FloatingPointNumber::value_type>(v)};
}

FloatingPointNumber abs(const FloatingPointNumber& value) noexcept;
FloatingPointNumber abs(const FloatingPointNumber& value) noexcept
{
  return value < 0._fpn ? -value : value;
}

const FloatingPointNumber& max(const FloatingPointNumber& lhs, const FloatingPointNumber& rhs) noexcept;
const FloatingPointNumber& max(const FloatingPointNumber& lhs, const FloatingPointNumber& rhs) noexcept
{
  return lhs < rhs ? rhs : lhs;
}

TEST(FloatingCompare, UserDefinedValues)
{
  xmaho::floating::equal<FloatingPointNumber> equal{};
  EXPECT_PRED2(equal, 0._fpn, 0._fpn);
  EXPECT_PRED2(equal, 1._fpn, 1._fpn);
  EXPECT_PRED2(equal, 10._fpn, 10._fpn);
  EXPECT_PRED2(equal, 1e6_fpn, 1e6_fpn);
  {
    const auto value {[](){auto v {0._fpn}; for (auto i {10u}; i != 0; --i) v += 0.1_fpn; return v;}()};
    EXPECT_PRED2(equal, 1._fpn, value);
    EXPECT_FLOAT_EQ(1.f, static_cast<float>(value));
    EXPECT_NE(1._fpn, value);
  }
}
