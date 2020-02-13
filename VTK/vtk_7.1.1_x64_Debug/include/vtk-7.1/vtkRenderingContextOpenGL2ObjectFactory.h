/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkRenderingContextOpenGL2ObjectFactory.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef __vtkRenderingContextOpenGL2ObjectFactory_h
#define __vtkRenderingContextOpenGL2ObjectFactory_h

#include "vtkRenderingContextOpenGL2Module.h" // For export macro
#include "vtkObjectFactory.h"

class VTKRENDERINGCONTEXTOPENGL2_EXPORT vtkRenderingContextOpenGL2ObjectFactory : public vtkObjectFactory
{
public:
  static vtkRenderingContextOpenGL2ObjectFactory * New();
  vtkTypeMacro(vtkRenderingContextOpenGL2ObjectFactory, vtkObjectFactory)

  const char * GetDescription() { return "vtkRenderingContextOpenGL2 factory overrides."; }

  const char * GetVTKSourceVersion();

  void PrintSelf(ostream &os, vtkIndent indent);

protected:
  vtkRenderingContextOpenGL2ObjectFactory();

private:
  vtkRenderingContextOpenGL2ObjectFactory(const vtkRenderingContextOpenGL2ObjectFactory&) VTK_DELETE_FUNCTION;
  void operator=(const vtkRenderingContextOpenGL2ObjectFactory&) VTK_DELETE_FUNCTION;
};

#endif // __vtkRenderingContextOpenGL2ObjectFactory_h
