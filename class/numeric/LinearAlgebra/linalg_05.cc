// file: $isip/class/numeric/LinearAlgebra/linalg_05.cc
// version: $Id: linalg_05.cc 7333 2001-08-27 20:22:06Z hamaker $
//

// isip include files
//
#include "LinearAlgebra.h"

// method: linearSolveTemplate
//
// arguments:
//  TVector& x: (output) unknown quantities
//  const TMatrix& A: (input) multiplier matrix
//  const TVector& b: (input) known quantities
//
// return: bool8 value indicating status
//
// this method computes the solution to linear equations of the form
//     A * x = b
// where
//     A[i,1] * x[1] + A[i,2] * x[2] + ... + A[i,n] * x[n] = b[i]
// for all 0 <= i < n
//
// this routine finds the x vector which satisfies this set of
// equations.  overdetermined, underdetermined, and singular sets use
// singular value decomposition. square matrices use LU
// decomposition. If the LU decomposition fails, singular value
// decomposition is attempted but it is very likely that the results
// will be numerically inaccurate.
//
template <class TMatrix, class TVector>
bool8 LinearAlgebra::linearSolveTemplate(TVector& x_a,
					   const TMatrix& A_a,
					   const TVector& b_a) {

  // if the matrix is square, we can use LU decomposition
  //
  if (A_a.isSquare() && ! A_a.isSingular()) {

    // create temporary storage for the L and U matrices
    //
    TMatrix L;
    TMatrix U;
    VectorLong index;
    int32 sign;
    
    // call the LU decomposition routine and then the lu solver
    //
    if (A_a.decompositionLU(L, U, index, sign) &&
	A_a.luSolve(x_a, L, U, index, b_a)) {
      return true;
    }
    else {

      // display warning and use SVD solution
      //
      Error::handle(name(), L"linearSolveTemplate", ERR_LSOLVE_SINGULAR,
		    __FILE__, __LINE__, Error::WARNING);
    }
  }

  // all other cases use SVD for solution
  //

  // create temporary storage for the V, W, U matrices
  //
  TMatrix U;
  TMatrix W;
  TMatrix V;

  // decompose the matrix
  //
  if (!A_a.decompositionSVD(U, W, V)) {
    Error::handle(name(), L"linearSolveTemplate", ERR,
		  __FILE__, __LINE__);
  }

  // solve the set and return 
  //
  return (A_a.svdSolve(x_a, U, W, V, b_a, true));
}

// explicit instantiations
//
template bool8
LinearAlgebra::linearSolveTemplate<MatrixFloat,VectorFloat>(VectorFloat&,
						    const MatrixFloat&,
						    const VectorFloat&);

template bool8
LinearAlgebra::linearSolveTemplate<MatrixDouble,VectorDouble>(VectorDouble&,
						  const MatrixDouble&,
						  const VectorDouble&);
