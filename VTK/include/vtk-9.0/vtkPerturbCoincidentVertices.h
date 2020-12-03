/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPerturbCoincidentVertices.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/*-------------------------------------------------------------------------
  Copyright 2009 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
  the U.S. Government retains certain rights in this software.
-------------------------------------------------------------------------*/
/**
 * @class   vtkPerturbCoincidentVertices
 * @brief   Perturbs vertices that are coincident.
 *
 *
 * This filter perturbs vertices in a graph that have coincident coordinates.
 * In particular this happens all the time with graphs that are georeferenced,
 * so we need a nice scheme to perturb the vertices so that when the user
 * zooms in the vertices can be distiquished.
 */

#ifndef vtkPerturbCoincidentVertices_h
#define vtkPerturbCoincidentVertices_h

#include "vtkGraphAlgorithm.h"
#include "vtkInfovisLayoutModule.h" // For export macro
#include "vtkSmartPointer.h"        // for ivars

class vtkCoincidentPoints;
class vtkDataSet;

class VTKINFOVISLAYOUT_EXPORT vtkPerturbCoincidentVertices : public vtkGraphAlgorithm
{
public:
  static vtkPerturbCoincidentVertices* New();
  vtkTypeMacro(vtkPerturbCoincidentVertices, vtkGraphAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Specify the perturbation factor (defaults to 1.0)
   */
  vtkSetMacro(PerturbFactor, double);
  vtkGetMacro(PerturbFactor, double);
  //@}

protected:
  vtkPerturbCoincidentVertices();
  ~vtkPerturbCoincidentVertices() override;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

private:
  // This class might have more than one method of coincident resolution
  void SpiralPerturbation(vtkGraph* input, vtkGraph* output);
  void SimpleSpiralPerturbation(vtkGraph* input, vtkGraph* output, float perturbFactor);

  float PerturbFactor;

  vtkPerturbCoincidentVertices(const vtkPerturbCoincidentVertices&) = delete;
  void operator=(const vtkPerturbCoincidentVertices&) = delete;
};

#endif
