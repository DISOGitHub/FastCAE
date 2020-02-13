/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkRenderingFreeTypeObjectFactory.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef __vtkRenderingFreeTypeObjectFactory_h
#define __vtkRenderingFreeTypeObjectFactory_h

#include "vtkRenderingFreeTypeModule.h" // For export macro
#include "vtkObjectFactory.h"

class VTKRENDERINGFREETYPE_EXPORT vtkRenderingFreeTypeObjectFactory : public vtkObjectFactory
{
public:
  static vtkRenderingFreeTypeObjectFactory * New();
  vtkTypeMacro(vtkRenderingFreeTypeObjectFactory, vtkObjectFactory)

  const char * GetDescription() { return "vtkRenderingFreeType factory overrides."; }

  const char * GetVTKSourceVersion();

  void PrintSelf(ostream &os, vtkIndent indent);

protected:
  vtkRenderingFreeTypeObjectFactory();

private:
  vtkRenderingFreeTypeObjectFactory(const vtkRenderingFreeTypeObjectFactory&) VTK_DELETE_FUNCTION;
  void operator=(const vtkRenderingFreeTypeObjectFactory&) VTK_DELETE_FUNCTION;
};

#endif // __vtkRenderingFreeTypeObjectFactory_h
