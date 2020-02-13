/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkInteractionStyleObjectFactory.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef __vtkInteractionStyleObjectFactory_h
#define __vtkInteractionStyleObjectFactory_h

#include "vtkInteractionStyleModule.h" // For export macro
#include "vtkObjectFactory.h"

class VTKINTERACTIONSTYLE_EXPORT vtkInteractionStyleObjectFactory : public vtkObjectFactory
{
public:
  static vtkInteractionStyleObjectFactory * New();
  vtkTypeMacro(vtkInteractionStyleObjectFactory, vtkObjectFactory)

  const char * GetDescription() { return "vtkInteractionStyle factory overrides."; }

  const char * GetVTKSourceVersion();

  void PrintSelf(ostream &os, vtkIndent indent);

protected:
  vtkInteractionStyleObjectFactory();

private:
  vtkInteractionStyleObjectFactory(const vtkInteractionStyleObjectFactory&) VTK_DELETE_FUNCTION;
  void operator=(const vtkInteractionStyleObjectFactory&) VTK_DELETE_FUNCTION;
};

#endif // __vtkInteractionStyleObjectFactory_h
