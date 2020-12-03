/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkMatrix4x4.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkMatrix4x4
 * @brief   represent and manipulate 4x4 transformation matrices
 *
 * vtkMatrix4x4 is a class to represent and manipulate 4x4 matrices.
 * Specifically, it is designed to work on 4x4 transformation matrices
 * found in 3D rendering using homogeneous coordinates [x y z w].
 * Many of the methods take an array of 16 doubles in row-major format.
 * Note that OpenGL stores matrices in column-major format, so the matrix
 * contents must be transposed when they are moved between OpenGL and VTK.
 * @sa
 * vtkTransform
 */

#ifndef vtkMatrix4x4_h
#define vtkMatrix4x4_h

#include "vtkCommonMathModule.h" // For export macro
#include "vtkObject.h"

class VTKCOMMONMATH_EXPORT vtkMatrix4x4 : public vtkObject
{
public:
  /// The internal data is public for historical reasons. Do not use!
  double Element[4][4];

  /**
   * Construct a 4x4 identity matrix.
   */
  static vtkMatrix4x4* New();

  vtkTypeMacro(vtkMatrix4x4, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Set the elements of the matrix to the same values as the elements
   * of the given source matrix.
   */
  void DeepCopy(const vtkMatrix4x4* source)
  {
    vtkMatrix4x4::DeepCopy(*this->Element, source);
    this->Modified();
  }

  /**
   * Set the elements of the given destination buffer to the same values
   * as the elements of the given source matrix.
   */
  static void DeepCopy(double destination[16], const vtkMatrix4x4* source)
  {
    vtkMatrix4x4::DeepCopy(destination, *source->Element);
  }

  /**
   * Copies the given source buffer to the given destination buffer.
   * The memory ranges must not overlap.  Does not affect any matrix.
   */
  static void DeepCopy(double destination[16], const double source[16]);

  /**
   * Non-static member function. Assigns *to* the matrix *from*
   * the given elements array.
   */
  void DeepCopy(const double elements[16])
  {
    this->DeepCopy(*this->Element, elements);
    this->Modified();
  }

  /**
   * Set all of the elements to zero.
   */
  void Zero()
  {
    vtkMatrix4x4::Zero(*this->Element);
    this->Modified();
  }
  static void Zero(double elements[16]);

  /**
   * Set equal to Identity matrix
   */
  void Identity()
  {
    vtkMatrix4x4::Identity(*this->Element);
    this->Modified();
  }
  static void Identity(double elements[16]);

  /**
   * Returns true if this matrix is equal to the identity matrix.
   */
  bool IsIdentity();

  /**
   * Matrix Inversion (adapted from Richard Carling in "Graphics Gems,"
   * Academic Press, 1990).
   */
  static void Invert(const vtkMatrix4x4* in, vtkMatrix4x4* out)
  {
    vtkMatrix4x4::Invert(*in->Element, *out->Element);
    out->Modified();
  }
  void Invert() { vtkMatrix4x4::Invert(this, this); }
  static void Invert(const double inElements[16], double outElements[16]);

  /**
   * Transpose the matrix and put it into out.
   */
  static void Transpose(const vtkMatrix4x4* in, vtkMatrix4x4* out)
  {
    vtkMatrix4x4::Transpose(*in->Element, *out->Element);
    out->Modified();
  }
  void Transpose() { vtkMatrix4x4::Transpose(this, this); }
  static void Transpose(const double inElements[16], double outElements[16]);

  /**
   * Multiply a homogeneous coordinate by this matrix, i.e. out = A*in.
   * The in[4] and out[4] can be the same array.
   */
  void MultiplyPoint(const float in[4], float out[4])
  {
    vtkMatrix4x4::MultiplyPoint(*this->Element, in, out);
  }
  void MultiplyPoint(const double in[4], double out[4])
  {
    vtkMatrix4x4::MultiplyPoint(*this->Element, in, out);
  }

  static void MultiplyPoint(const double elements[16], const float in[4], float out[4]);
  static void MultiplyPoint(const double elements[16], const double in[4], double out[4]);

  /**
   * For use in Java or Python.
   */
  float* MultiplyPoint(const float in[4]) VTK_SIZEHINT(4) { return this->MultiplyFloatPoint(in); }
  double* MultiplyPoint(const double in[4]) VTK_SIZEHINT(4)
  {
    return this->MultiplyDoublePoint(in);
  }
  float* MultiplyFloatPoint(const float in[4]) VTK_SIZEHINT(4)
  {
    this->MultiplyPoint(in, this->FloatPoint);
    return this->FloatPoint;
  }
  double* MultiplyDoublePoint(const double in[4]) VTK_SIZEHINT(4)
  {
    this->MultiplyPoint(in, this->DoublePoint);
    return this->DoublePoint;
  }

  //@{
  /**
   * Multiplies matrices a and b and stores the result in c.
   */
  static void Multiply4x4(const vtkMatrix4x4* a, const vtkMatrix4x4* b, vtkMatrix4x4* c);
  static void Multiply4x4(const double a[16], const double b[16], double c[16]);
  //@}

  /**
   * Compute adjoint of the matrix and put it into out.
   */
  void Adjoint(const vtkMatrix4x4* in, vtkMatrix4x4* out)
  {
    vtkMatrix4x4::Adjoint(*in->Element, *out->Element);
  }
  static void Adjoint(const double inElements[16], double outElements[16]);

  /**
   * Compute the determinant of the matrix and return it.
   */
  double Determinant() { return vtkMatrix4x4::Determinant(*this->Element); }
  static double Determinant(const double elements[16]);

  /**
   * Sets the element i,j in the matrix.
   */
  void SetElement(int i, int j, double value);

  /**
   * Returns the element i,j from the matrix.
   */
  double GetElement(int i, int j) const { return this->Element[i][j]; }

  /**
   * Returns the raw double array holding the matrix.
   */
  double* GetData() { return *this->Element; }

  /**
   * Returns the raw double array holding the matrix.
   */
  const double* GetData() const { return *this->Element; }

protected:
  vtkMatrix4x4() { vtkMatrix4x4::Identity(*this->Element); }
  ~vtkMatrix4x4() override {}

  float FloatPoint[4];
  double DoublePoint[4];

private:
  vtkMatrix4x4(const vtkMatrix4x4&) = delete;
  void operator=(const vtkMatrix4x4&) = delete;
};

//----------------------------------------------------------------------------
// Multiplies matrices a and b and stores the result in c.
inline void vtkMatrix4x4::Multiply4x4(const double a[16], const double b[16], double c[16])
{
  double tmp[16];

  for (int i = 0; i < 16; i += 4)
  {
    for (int j = 0; j < 4; j++)
    {
      tmp[i + j] =
        a[i + 0] * b[j + 0] + a[i + 1] * b[j + 4] + a[i + 2] * b[j + 8] + a[i + 3] * b[j + 12];
    }
  }

  for (int k = 0; k < 16; k++)
  {
    c[k] = tmp[k];
  }
}

//----------------------------------------------------------------------------
inline void vtkMatrix4x4::Multiply4x4(const vtkMatrix4x4* a, const vtkMatrix4x4* b, vtkMatrix4x4* c)
{
  vtkMatrix4x4::Multiply4x4(*a->Element, *b->Element, *c->Element);
}

//----------------------------------------------------------------------------
inline void vtkMatrix4x4::SetElement(int i, int j, double value)
{
  if (this->Element[i][j] != value)
  {
    this->Element[i][j] = value;
    this->Modified();
  }
}

//----------------------------------------------------------------------------
inline bool vtkMatrix4x4::IsIdentity()
{
  double* M = *this->Element;
  return M[0] == 1.0 && M[1] == 0.0 && M[2] == 0.0 && M[3] == 0.0 && M[4] == 0.0 && M[5] == 1.0 &&
    M[6] == 0.0 && M[7] == 0.0 && M[8] == 0.0 && M[9] == 0.0 && M[10] == 1.0 && M[11] == 0.0 &&
    M[12] == 0.0 && M[13] == 0.0 && M[14] == 0.0 && M[15] == 1.0;
}

#endif
