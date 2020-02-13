/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkIOExportOpenGL2ObjectFactory.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef __vtkIOExportOpenGL2ObjectFactory_h
#define __vtkIOExportOpenGL2ObjectFactory_h

#include "vtkIOExportOpenGL2Module.h" // For export macro
#include "vtkObjectFactory.h"

class VTKIOEXPORTOPENGL2_EXPORT vtkIOExportOpenGL2ObjectFactory : public vtkObjectFactory
{
public:
  static vtkIOExportOpenGL2ObjectFactory * New();
  vtkTypeMacro(vtkIOExportOpenGL2ObjectFactory, vtkObjectFactory)

  const char * GetDescription() { return "vtkIOExportOpenGL2 factory overrides."; }

  const char * GetVTKSourceVersion();

  void PrintSelf(ostream &os, vtkIndent indent);

protected:
  vtkIOExportOpenGL2ObjectFactory();

private:
  vtkIOExportOpenGL2ObjectFactory(const vtkIOExportOpenGL2ObjectFactory&) VTK_DELETE_FUNCTION;
  void operator=(const vtkIOExportOpenGL2ObjectFactory&) VTK_DELETE_FUNCTION;
};

#endif // __vtkIOExportOpenGL2ObjectFactory_h
