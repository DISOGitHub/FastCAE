/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPolyLineRepresentation.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkPolyLineRepresentation
 * @brief   vtkWidgetRepresentation for a poly line.
 *
 * vtkPolyLineRepresentation is a vtkCurveRepresentation for a poly
 * line. This 3D widget defines a poly line that can be interactively
 * placed in a scene. The poly line has handles, the number of which
 * can be changed, plus the widget can be picked on the poly line
 * itself to translate or rotate it in the scene.
 * Based on vtkCurveRepresentation
 * @sa
 * vtkSplineRepresentation
 */

#ifndef vtkPolyLineRepresentation_h
#define vtkPolyLineRepresentation_h

#include "vtkCurveRepresentation.h"
#include "vtkInteractionWidgetsModule.h" // For export macro

class vtkPolyLineSource;
class vtkPoints;
class vtkPolyData;

class VTKINTERACTIONWIDGETS_EXPORT vtkPolyLineRepresentation : public vtkCurveRepresentation
{
public:
  static vtkPolyLineRepresentation* New();
  vtkTypeMacro(vtkPolyLineRepresentation, vtkCurveRepresentation);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Grab the polydata (including points) that defines the poly line.
   * polydata consists of points and line segments between consecutive
   * points. Points are guaranteed to be up-to-date when either the
   * InteractionEvent or EndInteraction events are invoked. The user
   * provides the vtkPolyData and the points and polyline are added to
   * it.
   */
  void GetPolyData(vtkPolyData* pd) override;

  /**
   * Set the number of handles for this widget.
   */
  void SetNumberOfHandles(int npts) override;

  /**
   * Get the positions of the handles.
   */
  vtkDoubleArray* GetHandlePositions() override;

  /**
   * Get the true length of the poly line. Calculated as the summed
   * lengths of the individual straight line segments.
   */
  double GetSummedLength() override;

  /**
   * Convenience method to allocate and set the handles from a
   * vtkPoints instance.  If the first and last points are the same,
   * the poly line sets Closed to on and disregards the last point,
   * otherwise Closed remains unchanged.
   */
  void InitializeHandles(vtkPoints* points) override;

  /**
   * Build the representation for the poly line.
   */
  void BuildRepresentation() override;

protected:
  vtkPolyLineRepresentation();
  ~vtkPolyLineRepresentation() override;

  // The poly line source
  vtkPolyLineSource* PolyLineSource;

  // Specialized method to insert a handle on the poly line.
  int InsertHandleOnLine(double* pos) override;

private:
  vtkPolyLineRepresentation(const vtkPolyLineRepresentation&) = delete;
  void operator=(const vtkPolyLineRepresentation&) = delete;
};

#endif
