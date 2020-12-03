/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkRIBLight.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkRIBLight
 * @brief   RIP Light
 *
 * vtkRIBLight is a subclass of vtkLight that allows the user to
 * specify light source shaders and shadow casting lights for use with
 * RenderMan.
 *
 * @sa
 * vtkRIBExporter vtkRIBProperty
 */

#ifndef vtkRIBLight_h
#define vtkRIBLight_h

#include "vtkIOExportModule.h" // For export macro
#include "vtkLight.h"

class vtkRIBRenderer;

class VTKIOEXPORT_EXPORT vtkRIBLight : public vtkLight
{
public:
  static vtkRIBLight* New();
  vtkTypeMacro(vtkRIBLight, vtkLight);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  vtkBooleanMacro(Shadows, vtkTypeBool);
  vtkSetMacro(Shadows, vtkTypeBool);
  vtkGetMacro(Shadows, vtkTypeBool);

  void Render(vtkRenderer* ren, int index) override;

protected:
  vtkRIBLight();
  ~vtkRIBLight() override;

  vtkLight* Light;
  vtkTypeBool Shadows;

private:
  vtkRIBLight(const vtkRIBLight&) = delete;
  void operator=(const vtkRIBLight&) = delete;
};

#endif
