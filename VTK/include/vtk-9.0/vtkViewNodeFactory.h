/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkViewNodeFactory.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkViewNodeFactory
 * @brief   factory that chooses vtkViewNodes to create
 *
 * Class tells VTK which specific vtkViewNode subclass to make when it is
 * asked to make a vtkViewNode for a particular renderable. modules for
 * different rendering backends are expected to use this to customize the
 * set of instances for their own purposes
 */

#ifndef vtkViewNodeFactory_h
#define vtkViewNodeFactory_h

#include "vtkObject.h"
#include "vtkRenderingSceneGraphModule.h" // For export macro

class vtkViewNode;

class VTKRENDERINGSCENEGRAPH_EXPORT vtkViewNodeFactory : public vtkObject
{
public:
  static vtkViewNodeFactory* New();
  vtkTypeMacro(vtkViewNodeFactory, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Give a function pointer to a class that will manufacture a
   * vtkViewNode when given a class name string.
   */
  void RegisterOverride(const char* name, vtkViewNode* (*func)());

  /**
   * Creates and returns a vtkViewNode for the provided renderable.
   */
  vtkViewNode* CreateNode(vtkObject*);

  /**
   * @deprecated As of 9.0, No longer equivalent to CreateNode(vtkObject*).
   * Unused in 8.2.
   */
  VTK_LEGACY(vtkViewNode* CreateNode(const char*));

protected:
  vtkViewNodeFactory();
  ~vtkViewNodeFactory() override;

private:
  vtkViewNodeFactory(const vtkViewNodeFactory&) = delete;
  void operator=(const vtkViewNodeFactory&) = delete;

  class vtkInternals;
  vtkInternals* Internals;
};

#endif
