/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkRenderingGL2PSOpenGL2ObjectFactory.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef __vtkRenderingGL2PSOpenGL2ObjectFactory_h
#define __vtkRenderingGL2PSOpenGL2ObjectFactory_h

#include "vtkRenderingGL2PSOpenGL2Module.h" // For export macro
#include "vtkObjectFactory.h"

class VTKRENDERINGGL2PSOPENGL2_EXPORT vtkRenderingGL2PSOpenGL2ObjectFactory : public vtkObjectFactory
{
public:
  static vtkRenderingGL2PSOpenGL2ObjectFactory * New();
  vtkTypeMacro(vtkRenderingGL2PSOpenGL2ObjectFactory, vtkObjectFactory)

  const char * GetDescription() { return "vtkRenderingGL2PSOpenGL2 factory overrides."; }

  const char * GetVTKSourceVersion();

  void PrintSelf(ostream &os, vtkIndent indent);

protected:
  vtkRenderingGL2PSOpenGL2ObjectFactory();

private:
  vtkRenderingGL2PSOpenGL2ObjectFactory(const vtkRenderingGL2PSOpenGL2ObjectFactory&) VTK_DELETE_FUNCTION;
  void operator=(const vtkRenderingGL2PSOpenGL2ObjectFactory&) VTK_DELETE_FUNCTION;
};

#endif // __vtkRenderingGL2PSOpenGL2ObjectFactory_h
