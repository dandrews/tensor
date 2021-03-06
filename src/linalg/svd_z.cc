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

#include <tensor/tensor.h>
#include <tensor/tensor_lapack.h>
#include <tensor/linalg.h>

namespace linalg {

  using namespace lapack;

  /**Singular value decomposition of a complex matrix.

     The singular value decomposition of a matrix A, consists in finding two
     unitary matrices U and V, and diagonal one S with nonnegative elements, such
     that \f$A = U S V\f$. The svd() routine computes the diagonal elements of
     the matrix S and puts them in a 1D tensor, which is the output of the
     routine.  Optionally, the matrices U and V are also computed, and stored in
     the variables pointed to by U and VT.

     Unless otherwise specified, if the matrix A has \c MxN elements, then U is
     \c MxM, V is \c NxN and the vector S will have \c min(M,N) elements. However
     if flag \c economic is different from zero, then we get smaller matrices,
     U being \c MxR, V being \c RxN and S will have \c R=min(M,N) elements.
     
     \ingroup Linalg
  */
  RTensor
  svd(CTensor A, CTensor *U, CTensor *VT, bool economic)
  {
    /*
    if (accurate_svd) {
      return block_svd(A, U, VT, economic);
    }
    */
    assert(A.rows() > 0);
    assert(A.columns() > 0);
    assert(A.rank() == 2);

    integer m = A.rows();
    integer n = A.columns();
    integer k = std::min(m, n);
    integer lwork, ldu, lda, ldv, info;
    RTensor output(k);
    cdouble *work, *u, *v, *a = tensor_pointer(A), foo;
    double *rwork, *s = tensor_pointer(output);
    char jobv[1], jobu[1];
    
    if (U) {
      *U = CTensor(m, economic? k : m);
      u = tensor_pointer(*U);
      jobu[0] = economic? 'S' : 'A';
      ldu = m;
    } else {
      jobu[0] = 'N';
      u = &foo;
      ldu = 1;
    }
    if (VT) {
      (*VT) = CTensor(economic? k : n, n);
      v = tensor_pointer(*VT);
      jobv[0] = economic? 'S' : 'A';
      ldv = economic? k : n;
    } else {
	jobv[0] = 'N';
	v = &foo;
	ldv = 1;
    }
    lda = m;
    lwork = -1;
    work = &foo;
    rwork = (double *)&foo;
    F77NAME(zgesvd)(jobu, jobv, &m, &n, a, &m, s, u, &ldu, v, &ldv,
		    work, &lwork, rwork, &info);
    lwork = lapack::real(work[0]);
    work = new cdouble[lwork];
    rwork = new double[5 * k];
    F77NAME(zgesvd)(jobu, jobv, &m, &n, a, &m, s, u, &ldu, v, &ldv,
		    work, &lwork, rwork, &info);
    delete[] work;
    delete[] rwork;
    return output;
  }


} // namespace linalg
