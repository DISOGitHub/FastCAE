/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCompositePolyDataMapper.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkCompositePolyDataMapper
 * @brief   a class that renders hierarchical polygonal data
 *
 * This class uses a set of vtkPolyDataMappers to render input data
 * which may be hierarchical. The input to this mapper may be
 * either vtkPolyData or a vtkCompositeDataSet built from
 * polydata. If something other than vtkPolyData is encountered,
 * an error message will be produced.
 * @sa
 * vtkPolyDataMapper
 */

#ifndef vtkCompositePolyDataMapper_h
#define vtkCompositePolyDataMapper_h

#include "vtkMapper.h"
#include "vtkRenderingCoreModule.h" // For export macro

class vtkPolyDataMapper;
class vtkInformation;
class vtkRenderer;
class vtkActor;
class vtkCompositePolyDataMapperInternals;

class VTKRENDERINGCORE_EXPORT vtkCompositePolyDataMapper : public vtkMapper
{

public:
  static vtkCompositePolyDataMapper* New();
  vtkTypeMacro(vtkCompositePolyDataMapper, vtkMapper);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Standard method for rendering a mapper. This method will be
   * called by the actor.
   */
  void Render(vtkRenderer* ren, vtkActor* a) override;

  //@{
  /**
   * Standard vtkProp method to get 3D bounds of a 3D prop
   */
  double* GetBounds() VTK_SIZEHINT(6) override;
  void GetBounds(double bounds[6]) override { this->Superclass::GetBounds(bounds); }
  //@}

  /**
   * Release the underlying resources associated with this mapper
   */
  void ReleaseGraphicsResources(vtkWindow*) override;

  //@{
  /**
   * Some introspection on the type of data the mapper will render
   * used by props to determine if they should invoke the mapper
   * on a specific rendering pass.
   */
  bool HasOpaqueGeometry() override;
  bool HasTranslucentPolygonalGeometry() override;
  //@}

protected:
  vtkCompositePolyDataMapper();
  ~vtkCompositePolyDataMapper() override;

  /**
   * We need to override this method because the standard streaming
   * demand driven pipeline is not what we want - we are expecting
   * hierarchical data as input
   */
  vtkExecutive* CreateDefaultExecutive() override;

  /**
   * Need to define the type of data handled by this mapper.
   */
  int FillInputPortInformation(int port, vtkInformation* info) override;

  /**
   * This is the build method for creating the internal polydata
   * mapper that do the actual work
   */
  void BuildPolyDataMapper();

  /**
   * BuildPolyDataMapper uses this for each mapper. It is broken out so we can change types.
   */
  virtual vtkPolyDataMapper* MakeAMapper();

  /**
   * Need to loop over the hierarchy to compute bounds
   */
  void ComputeBounds();

  /**
   * Time stamp for computation of bounds.
   */
  vtkTimeStamp BoundsMTime;

  /**
   * These are the internal polydata mapper that do the
   * rendering. We save then so that they can keep their
   * display lists.
   */
  vtkCompositePolyDataMapperInternals* Internal;

  /**
   * Time stamp for when we need to update the
   * internal mappers
   */
  vtkTimeStamp InternalMappersBuildTime;

private:
  vtkCompositePolyDataMapper(const vtkCompositePolyDataMapper&) = delete;
  void operator=(const vtkCompositePolyDataMapper&) = delete;
};

#endif
