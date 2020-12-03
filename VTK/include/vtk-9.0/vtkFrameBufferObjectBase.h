/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkFrameBufferObjectBase.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkFrameBufferObjectBase
 * @brief   abstract interface to OpenGL FBOs
 *
 * API for classes that encapsulate an OpenGL Frame Buffer Object.
 */

#ifndef vtkFrameBufferObjectBase_h
#define vtkFrameBufferObjectBase_h

#include "vtkObject.h"
#include "vtkRenderingCoreModule.h" // For export macro

class vtkRenderer;
class vtkProp;
class vtkInformation;

class VTKRENDERINGCORE_EXPORT vtkFrameBufferObjectBase : public vtkObject
{
public:
  vtkTypeMacro(vtkFrameBufferObjectBase, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Dimensions in pixels of the framebuffer.
   */
  virtual int* GetLastSize() = 0;
  virtual void GetLastSize(int& _arg1, int& _arg2) = 0;
  virtual void GetLastSize(int _arg[2]) = 0;
  //@}

protected:
  vtkFrameBufferObjectBase(); // no default constructor.
  ~vtkFrameBufferObjectBase() override;

private:
  vtkFrameBufferObjectBase(const vtkFrameBufferObjectBase&) = delete;
  void operator=(const vtkFrameBufferObjectBase&) = delete;
};

#endif
