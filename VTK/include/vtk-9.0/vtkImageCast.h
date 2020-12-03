/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkImageCast.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkImageCast
 * @brief    Image Data type Casting Filter
 *
 * vtkImageCast filter casts the input type to match the output type in
 * the image processing pipeline.  The filter does nothing if the input
 * already has the correct type.  To specify the "CastTo" type,
 * use "SetOutputScalarType" method.
 *
 * @warning
 * As vtkImageCast only casts values without rescaling them, its use is not
 * recommended. vtkImageShiftScale is the recommended way to change the type
 * of an image data.
 *
 * @sa
 * vtkImageThreshold vtkImageShiftScale
 */

#ifndef vtkImageCast_h
#define vtkImageCast_h

#include "vtkImagingCoreModule.h" // For export macro
#include "vtkThreadedImageAlgorithm.h"

class VTKIMAGINGCORE_EXPORT vtkImageCast : public vtkThreadedImageAlgorithm
{
public:
  static vtkImageCast* New();
  vtkTypeMacro(vtkImageCast, vtkThreadedImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Set the desired output scalar type to cast to.
   */
  vtkSetMacro(OutputScalarType, int);
  vtkGetMacro(OutputScalarType, int);
  void SetOutputScalarTypeToFloat() { this->SetOutputScalarType(VTK_FLOAT); }
  void SetOutputScalarTypeToDouble() { this->SetOutputScalarType(VTK_DOUBLE); }
  void SetOutputScalarTypeToInt() { this->SetOutputScalarType(VTK_INT); }
  void SetOutputScalarTypeToUnsignedInt() { this->SetOutputScalarType(VTK_UNSIGNED_INT); }
  void SetOutputScalarTypeToLong() { this->SetOutputScalarType(VTK_LONG); }
  void SetOutputScalarTypeToUnsignedLong() { this->SetOutputScalarType(VTK_UNSIGNED_LONG); }
  void SetOutputScalarTypeToShort() { this->SetOutputScalarType(VTK_SHORT); }
  void SetOutputScalarTypeToUnsignedShort() { this->SetOutputScalarType(VTK_UNSIGNED_SHORT); }
  void SetOutputScalarTypeToUnsignedChar() { this->SetOutputScalarType(VTK_UNSIGNED_CHAR); }
  void SetOutputScalarTypeToChar() { this->SetOutputScalarType(VTK_CHAR); }
  //@}

  //@{
  /**
   * When the ClampOverflow flag is on, the data is thresholded so that
   * the output value does not exceed the max or min of the data type.
   * Clamping is safer because otherwise you might invoke undefined
   * behavior (and may crash) if the type conversion is out of range
   * of the data type.  On the other hand, clamping is slower.
   * By default ClampOverflow is off.
   */
  vtkSetMacro(ClampOverflow, vtkTypeBool);
  vtkGetMacro(ClampOverflow, vtkTypeBool);
  vtkBooleanMacro(ClampOverflow, vtkTypeBool);
  //@}

protected:
  vtkImageCast();
  ~vtkImageCast() override {}

  vtkTypeBool ClampOverflow;
  int OutputScalarType;
  int RequestInformation(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  void ThreadedExecute(vtkImageData* inData, vtkImageData* outData, int ext[6], int id) override;

private:
  vtkImageCast(const vtkImageCast&) = delete;
  void operator=(const vtkImageCast&) = delete;
};

#endif
