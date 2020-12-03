/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkWarpTo.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkWarpTo
 * @brief   deform geometry by warping towards a point
 *
 * vtkWarpTo is a filter that modifies point coordinates by moving the
 * points towards a user specified position.
 */

#ifndef vtkWarpTo_h
#define vtkWarpTo_h

#include "vtkFiltersGeneralModule.h" // For export macro
#include "vtkPointSetAlgorithm.h"

class VTKFILTERSGENERAL_EXPORT vtkWarpTo : public vtkPointSetAlgorithm
{
public:
  static vtkWarpTo* New();
  vtkTypeMacro(vtkWarpTo, vtkPointSetAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Set/Get the value to scale displacement.
   */
  vtkSetMacro(ScaleFactor, double);
  vtkGetMacro(ScaleFactor, double);
  //@}

  //@{
  /**
   * Set/Get the position to warp towards.
   */
  vtkGetVectorMacro(Position, double, 3);
  vtkSetVector3Macro(Position, double);
  //@}

  //@{
  /**
   * Set/Get the Absolute ivar. Turning Absolute on causes scale factor
   * of the new position to be one unit away from Position.
   */
  vtkSetMacro(Absolute, vtkTypeBool);
  vtkGetMacro(Absolute, vtkTypeBool);
  vtkBooleanMacro(Absolute, vtkTypeBool);
  //@}

  int FillInputPortInformation(int port, vtkInformation* info) override;

protected:
  vtkWarpTo();
  ~vtkWarpTo() override {}

  int RequestDataObject(vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector) override;
  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  double ScaleFactor;
  double Position[3];
  vtkTypeBool Absolute;

private:
  vtkWarpTo(const vtkWarpTo&) = delete;
  void operator=(const vtkWarpTo&) = delete;
};

#endif
