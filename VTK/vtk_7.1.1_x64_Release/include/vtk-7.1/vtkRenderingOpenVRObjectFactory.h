/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkRenderingOpenVRObjectFactory.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef __vtkRenderingOpenVRObjectFactory_h
#define __vtkRenderingOpenVRObjectFactory_h

#include "vtkRenderingOpenVRModule.h" // For export macro
#include "vtkObjectFactory.h"

class VTKRENDERINGOPENVR_EXPORT vtkRenderingOpenVRObjectFactory : public vtkObjectFactory
{
public:
  static vtkRenderingOpenVRObjectFactory * New();
  vtkTypeMacro(vtkRenderingOpenVRObjectFactory, vtkObjectFactory)

  const char * GetDescription() { return "vtkRenderingOpenVR factory overrides."; }

  const char * GetVTKSourceVersion();

  void PrintSelf(ostream &os, vtkIndent indent);

protected:
  vtkRenderingOpenVRObjectFactory();

private:
  vtkRenderingOpenVRObjectFactory(const vtkRenderingOpenVRObjectFactory&) VTK_DELETE_FUNCTION;
  void operator=(const vtkRenderingOpenVRObjectFactory&) VTK_DELETE_FUNCTION;
};

#endif // __vtkRenderingOpenVRObjectFactory_h
