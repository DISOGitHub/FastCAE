/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkRenderingVolumeOpenGL2ObjectFactory.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef __vtkRenderingVolumeOpenGL2ObjectFactory_h
#define __vtkRenderingVolumeOpenGL2ObjectFactory_h

#include "vtkRenderingVolumeOpenGL2Module.h" // For export macro
#include "vtkObjectFactory.h"

class VTKRENDERINGVOLUMEOPENGL2_EXPORT vtkRenderingVolumeOpenGL2ObjectFactory : public vtkObjectFactory
{
public:
  static vtkRenderingVolumeOpenGL2ObjectFactory * New();
  vtkTypeMacro(vtkRenderingVolumeOpenGL2ObjectFactory, vtkObjectFactory)

  const char * GetDescription() { return "vtkRenderingVolumeOpenGL2 factory overrides."; }

  const char * GetVTKSourceVersion();

  void PrintSelf(ostream &os, vtkIndent indent);

protected:
  vtkRenderingVolumeOpenGL2ObjectFactory();

private:
  vtkRenderingVolumeOpenGL2ObjectFactory(const vtkRenderingVolumeOpenGL2ObjectFactory&) VTK_DELETE_FUNCTION;
  void operator=(const vtkRenderingVolumeOpenGL2ObjectFactory&) VTK_DELETE_FUNCTION;
};

#endif // __vtkRenderingVolumeOpenGL2ObjectFactory_h
