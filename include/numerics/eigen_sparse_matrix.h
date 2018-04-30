// The libMesh Finite Element Library.
// Copyright (C) 2002-2018 Benjamin S. Kirk, John W. Peterson, Roy H. Stogner

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



#ifndef LIBMESH_EIGEN_SPARSE_MATRIX_H
#define LIBMESH_EIGEN_SPARSE_MATRIX_H

#include "libmesh/libmesh_config.h"

#ifdef LIBMESH_HAVE_EIGEN

// Local includes
#include "libmesh/sparse_matrix.h"
#include "libmesh/eigen_core_support.h"

// Eigen includes

// C++ includes
#include <algorithm>
#include <cstddef>

namespace libMesh
{

// Forward declarations
template <typename T> class DenseMatrix;
template <typename T> class EigenSparseVector;
template <typename T> class EigenSparseLinearSolver;

/**
 * The EigenSparseMatrix class wraps a sparse matrix object from the
 * Eigen library. All overridden virtual functions are documented in
 * sparse_matrix.h.
 *
 * \author Benjamin S. Kirk
 * \date 2013
 */
template <typename T>
class EigenSparseMatrix libmesh_final : public SparseMatrix<T>
{

public:
  /**
   * Constructor; initializes the matrix to be empty, without any
   * structure, i.e.  the matrix is not usable at all. This
   * constructor is therefore only useful for matrices which are
   * members of a class. All other matrices should be created at a
   * point in the data flow where all necessary information is
   * available.
   *
   * You have to initialize the matrix before usage with \p init(...).
   */
  EigenSparseMatrix (const Parallel::Communicator & comm);

  /**
   * Destructor. Free all memory, but do not release the memory of the
   * sparsity structure.
   */
  ~EigenSparseMatrix ();

  /**
   * Convenient typedefs
   */
  typedef EigenSM DataType;
  typedef Eigen::Triplet<T,eigen_idx_type> TripletType;

  virtual void init (const numeric_index_type m,
                     const numeric_index_type n,
                     const numeric_index_type m_l,
                     const numeric_index_type n_l,
                     const numeric_index_type nnz=30,
                     const numeric_index_type noz=10,
                     const numeric_index_type blocksize=1) libmesh_override;

  virtual void init () libmesh_override;

  virtual void clear () libmesh_override;

  virtual void zero () libmesh_override;

  virtual void close () libmesh_override { this->_closed = true; }

  virtual numeric_index_type m () const libmesh_override;

  virtual numeric_index_type n () const libmesh_override;

  virtual numeric_index_type row_start () const libmesh_override;

  virtual numeric_index_type row_stop () const libmesh_override;

  virtual void set (const numeric_index_type i,
                    const numeric_index_type j,
                    const T value) libmesh_override;

  virtual void add (const numeric_index_type i,
                    const numeric_index_type j,
                    const T value) libmesh_override;

  virtual void add_matrix (const DenseMatrix<T> & dm,
                           const std::vector<numeric_index_type> & rows,
                           const std::vector<numeric_index_type> & cols) libmesh_override;

  virtual void add_matrix (const DenseMatrix<T> & dm,
                           const std::vector<numeric_index_type> & dof_indices) libmesh_override;

  virtual void add (const T a, SparseMatrix<T> & X) libmesh_override;

  virtual T operator () (const numeric_index_type i,
                         const numeric_index_type j) const libmesh_override;

  virtual Real l1_norm () const libmesh_override;

  virtual Real linfty_norm () const libmesh_override;

  virtual bool closed() const libmesh_override { return _closed; }

  virtual void print_personal(std::ostream & os=libMesh::out) const libmesh_override { this->print(os); }

  virtual void get_diagonal (NumericVector<T> & dest) const libmesh_override;

  virtual void get_transpose (SparseMatrix<T> & dest) const libmesh_override;

private:

  /**
   * Actual Eigen::SparseMatrix<> we are wrapping.
   */
  DataType _mat;

  /**
   * Flag indicating if the matrix has been closed yet.
   */
  bool _closed;

  /**
   * Make other Eigen datatypes friends
   */
  friend class EigenSparseVector<T>;
  friend class EigenSparseLinearSolver<T>;
};

} // namespace libMesh

#endif // #ifdef LIBMESH_HAVE_EIGEN
#endif // #ifdef LIBMESH_EIGEN_SPARSE_MATRIX_H
