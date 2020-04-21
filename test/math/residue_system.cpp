/*
BSD 2-Clause License

Copyright (c) 2020, FORNO
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

#include "xmaho/math/residue_system.hpp"

#include <gtest/gtest.h>

TEST(ResidueSystemTest, Construct)
{
  xmaho::math::residue_system<10> rs1{};
  xmaho::math::residue_system<10, long long> rs2{};
  xmaho::math::residue_system<10, unsigned long long> rs3{};
  xmaho::math::residue_system<10> rs4{5};
}

TEST(ResidueSystemTest, Congruence)
{
  xmaho::math::residue_system<10> rs{};
  EXPECT_EQ(rs, rs);
  EXPECT_EQ(rs, xmaho::math::residue_system<10>{0});
  EXPECT_EQ(xmaho::math::residue_system<10>{0}, rs);
  EXPECT_EQ(rs, xmaho::math::residue_system<10>{10});
  EXPECT_EQ(xmaho::math::residue_system<10>{10}, rs);
  EXPECT_EQ(rs, 0);
  EXPECT_EQ(0, rs);
  EXPECT_EQ(rs, 10);
  EXPECT_EQ(10, rs);
  xmaho::math::residue_system<10> rs2{10};
  EXPECT_EQ(rs, rs2);
}

TEST(ResidueSystemTest, Addtion)
{
  xmaho::math::residue_system<10> rs{};
  xmaho::math::residue_system<10> ans{5};
  EXPECT_EQ(rs + 5, ans);
  EXPECT_EQ(rs + 15, ans);
  rs += 5;
  EXPECT_EQ(rs, ans);
  rs += 10;
  EXPECT_EQ(rs, ans);
}

TEST(ResidueSystemTest, Substraction)
{
  xmaho::math::residue_system<10> rs{};
  xmaho::math::residue_system<10> ans{5};
  EXPECT_EQ(rs - 5, ans);
  EXPECT_EQ(rs - 15, ans);
  rs -= 5;
  EXPECT_EQ(rs, ans);
  rs -= 10;
  EXPECT_EQ(rs, ans);
}

TEST(REsidueSytemTest, Multiplication)
{
  xmaho::math::residue_system<10> rs{0};
  xmaho::math::residue_system<10> ans{0};
  EXPECT_EQ(rs * 5, ans);
  EXPECT_EQ(rs * 15, ans);
  rs *= 5;
  EXPECT_EQ(rs, ans);
  rs *= 10;
  EXPECT_EQ(rs, ans);
  xmaho::math::residue_system<10> rs1{1};
  xmaho::math::residue_system<10> ans1{5};
  EXPECT_EQ(rs1 * 5, ans1);
  EXPECT_EQ(rs1 * 15, ans1);
  rs1 *= 5;
  EXPECT_EQ(rs1, ans1);
  rs1 *= 10;
  EXPECT_EQ(rs1, ans);
  xmaho::math::residue_system<10> rs2{3};
  xmaho::math::residue_system<10> ans2{5};
}
