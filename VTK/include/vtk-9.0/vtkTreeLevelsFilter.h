/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkTreeLevelsFilter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/*-------------------------------------------------------------------------
  Copyright 2008 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
  the U.S. Government retains certain rights in this software.
-------------------------------------------------------------------------*/
/**
 * @class   vtkTreeLevelsFilter
 * @brief   adds level and leaf fields to a vtkTree
 *
 *
 * The filter currently add two arrays to the incoming vtkTree datastructure.
 * 1) "levels" this is the distance from the root of the vertex. Root = 0
 * and you add 1 for each level down from the root
 * 2) "leaf" this array simply indicates whether the vertex is a leaf or not
 *
 * @par Thanks:
 * Thanks to Brian Wylie from Sandia National Laboratories for creating this
 * class.
 */

#ifndef vtkTreeLevelsFilter_h
#define vtkTreeLevelsFilter_h

#include "vtkInfovisCoreModule.h" // For export macro
#include "vtkTreeAlgorithm.h"

class VTKINFOVISCORE_EXPORT vtkTreeLevelsFilter : public vtkTreeAlgorithm
{
public:
  static vtkTreeLevelsFilter* New();
  vtkTypeMacro(vtkTreeLevelsFilter, vtkTreeAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

protected:
  vtkTreeLevelsFilter();
  ~vtkTreeLevelsFilter() override {}

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

private:
  vtkTreeLevelsFilter(const vtkTreeLevelsFilter&) = delete;
  void operator=(const vtkTreeLevelsFilter&) = delete;
};

#endif
