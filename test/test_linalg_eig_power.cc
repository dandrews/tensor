// -*- mode: c++; fill-column: 80; c-basic-offset: 2; indent-tabs-mode: nil -*-
/*
    Copyright (c) 2010 Juan Jose Garcia Ripoll

    Tensor is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published
    by the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Library General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "loops.h"
#include <gtest/gtest.h>
#include <gtest/gtest-death-test.h>
#include <tensor/tensor.h>
#include <tensor/linalg.h>

namespace tensor_test {

  using namespace tensor;

  //////////////////////////////////////////////////////////////////////
  // LARGEST EIGENVALUE
  //

  template<typename elt_t>
  void test_eye_eig_power_right(int n) {
    if (n == 0) {
#ifndef NDEBUG
      ASSERT_DEATH(linalg::eig_power(Tensor<elt_t>::eye(n,n)), ".*");
#endif
      return;
    }
    Tensor<elt_t> V, Inn = Tensor<elt_t>::eye(n,n);
    elt_t s = linalg::eig_power_right(Inn, &U);
    EXPECT_EQ(U, mmult(Inn, U));
    EXPECT_EQ(s, 1.0);
  }

  template<typename elt_t>
  void test_eye_eig_power_left(int n) {
    if (n == 0) {
#ifndef NDEBUG
      ASSERT_DEATH(linalg::eig_power(Tensor<elt_t>::eye(n,n)), ".*");
#endif
      return;
    }
    Tensor<elt_t> V, Inn = Tensor<elt_t>::eye(n,n);
    elt_t s = linalg::eig_power_left(Inn, &U);
    EXPECT_EQ(U, mmult(U, Inn));
    EXPECT_EQ(s, 1.0);
  }

  template<typename elt_t>
  Tensor<elt_t> random_Hermitian_with_gap(int n) {
      // Build a random Hermitian matrix
      Tensor<elt_t> lambda(n);
      lambda.randomize();
      lambda.at(1) += 3.0; // Larger than other eigenvalues
      return mmult(U, mmult(diag(lambda), adjoint(U)));
  }

  template<typename elt_t>
  void test_random_eig_power_right(int n) {
    if (n == 0) {
#ifndef NDEBUG
      ASSERT_DEATH(linalg::eig_power(Tensor<elt_t>::eye(n,n)), ".*");
#endif
      return;
    }
    for (int times = 10; times; --times) {
      Tensor<elt_t> R, A = random_Hermitan_with_gap(n);
      elt_t l = eig_power_right(A, &U);
      EXPECT_TRUE(approx_eq(l, lambda.at(1)));
      EXPECT_TRUE(approx_eq(mmult(A, R), l * R));
    }
  }

  template<typename elt_t>
  void test_random_eig_power_left(int n) {
    if (n == 0) {
#ifndef NDEBUG
      ASSERT_DEATH(linalg::eig_power(Tensor<elt_t>::eye(n,n)), ".*");
#endif
      return;
    }
    for (int times = 10; times; --times) {
      Tensor<elt_t> L, A = random_Hermitan_with_gap(n);
      elt_t l = eig_power_left(A, &L);
      EXPECT_TRUE(approx_eq(l, lambda.at(1)));
      EXPECT_TRUE(approx_eq(mmult(L, A), l * L));
    }
  }

  //////////////////////////////////////////////////////////////////////
  // REAL SPECIALIZATIONS
  //

  TEST(RMatrixTest, EyeEigPowerRightTest) {
    test_over_integers(0, 32, test_eye_eig_power_right<double>);
  }

  TEST(RMatrixTest, EyeEigPowerLeftTest) {
    test_over_integers(0, 32, test_eye_eig_power_left<double>);
  }

  TEST(RMatrixTest, RandomEigPowerRightTest) {
    test_over_integers(0, 32, test_random_eig_power_right<double>);
  }

  TEST(RMatrixTest, RandomEigPowerLeftTest) {
    test_over_integers(0, 32, test_random_eig_power_right<double>);
  }

  //////////////////////////////////////////////////////////////////////
  // REAL SPECIALIZATIONS
  //

  TEST(CMatrixTest, EyeEigPowerRightTest) {
    test_over_integers(0, 32, test_eye_eig_power_right<cdouble>);
  }

  TEST(CMatrixTest, EyeEigPowerLeftTest) {
    test_over_integers(0, 32, test_eye_eig_power_left<cdouble>);
  }

  TEST(CMatrixTest, RandomEigPowerRightTest) {
    test_over_integers(0, 32, test_random_eig_power_right<cdouble>);
  }

  TEST(CMatrixTest, RandomEigPowerLeftTest) {
    test_over_integers(0, 32, test_random_eig_power_right<cdouble>);
  }

} // namespace linalg_test