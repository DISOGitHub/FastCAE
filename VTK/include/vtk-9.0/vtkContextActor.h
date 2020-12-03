/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkContextActor.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkContextActor
 * @brief   provides a vtkProp derived object.
 *
 * This object provides the entry point for the vtkContextScene to be rendered
 * in a vtkRenderer. Uses the RenderOverlay pass to render the 2D
 * vtkContextScene.
 */

#ifndef vtkContextActor_h
#define vtkContextActor_h

#include "vtkNew.h" // For ivars
#include "vtkProp.h"
#include "vtkRenderingContext2DModule.h" // For export macro
#include "vtkSmartPointer.h"             // For ivars

class vtkContext2D;
class vtkContext3D;
class vtkContextDevice2D;
class vtkContextScene;

class VTKRENDERINGCONTEXT2D_EXPORT vtkContextActor : public vtkProp
{
public:
  void PrintSelf(ostream& os, vtkIndent indent) override;
  vtkTypeMacro(vtkContextActor, vtkProp);

  static vtkContextActor* New();

  /**
   * We only render in the overlay for the context scene.
   */
  int RenderOverlay(vtkViewport* viewport) override;

  //@{
  /**
   * Get the vtkContext2D for the actor.
   */
  vtkGetNewMacro(Context, vtkContext2D);
  //@}

  /**
   * Get the chart object for the actor.
   */
  vtkContextScene* GetScene();

  /**
   * Set the scene for the actor.
   */
  void SetScene(vtkContextScene* scene);

  /**
   * Force rendering to a specific device. If left NULL, a default
   * device will be created.
   * @{
   */
  void SetForceDevice(vtkContextDevice2D* dev);
  vtkGetObjectMacro(ForceDevice, vtkContextDevice2D);
  /**@}*/

  /**
   * Release any graphics resources that are being consumed by this actor.
   * The parameter window could be used to determine which graphic
   * resources to release.
   */
  void ReleaseGraphicsResources(vtkWindow* window) override;

protected:
  vtkContextActor();
  ~vtkContextActor() override;

  /**
   * Initialize the actor - right now we just decide which device to initialize.
   */
  virtual void Initialize(vtkViewport* viewport);

  vtkSmartPointer<vtkContextScene> Scene;
  vtkNew<vtkContext2D> Context;
  vtkNew<vtkContext3D> Context3D;
  vtkContextDevice2D* ForceDevice;
  bool Initialized;

private:
  vtkContextActor(const vtkContextActor&) = delete;
  void operator=(const vtkContextActor&) = delete;
};

#endif
