/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkSpherePuzzleArrows.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkSpherePuzzleArrows
 * @brief   Visualize permutation of the sphere puzzle.
 *
 * vtkSpherePuzzleArrows creates
 */

#ifndef vtkSpherePuzzleArrows_h
#define vtkSpherePuzzleArrows_h

#include "vtkFiltersModelingModule.h" // For export macro
#include "vtkPolyDataAlgorithm.h"

class vtkCellArray;
class vtkPoints;
class vtkSpherePuzzle;

class VTKFILTERSMODELING_EXPORT vtkSpherePuzzleArrows : public vtkPolyDataAlgorithm
{
public:
  vtkTypeMacro(vtkSpherePuzzleArrows, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  static vtkSpherePuzzleArrows* New();

  //@{
  /**
   * Permutation is an array of puzzle piece ids.
   * Arrows will be generated for any id that does not contain itself.
   * Permutation[3] = 3 will produce no arrow.
   * Permutation[3] = 10 will draw an arrow from location 3 to 10.
   */
  vtkSetVectorMacro(Permutation, int, 32);
  vtkGetVectorMacro(Permutation, int, 32);
  void SetPermutationComponent(int comp, int val);
  void SetPermutation(vtkSpherePuzzle* puz);
  //@}

protected:
  vtkSpherePuzzleArrows();
  ~vtkSpherePuzzleArrows() override;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  void AppendArrow(int id1, int id2, vtkPoints* pts, vtkCellArray* polys);

  int Permutation[32];

  double Radius;

private:
  vtkSpherePuzzleArrows(const vtkSpherePuzzleArrows&) = delete;
  void operator=(const vtkSpherePuzzleArrows&) = delete;
};

#endif
