/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkUnstructuredGridVolumeMapper.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkUnstructuredGridVolumeMapper
 * @brief   Abstract class for an unstructured grid volume mapper
 *
 *
 * vtkUnstructuredGridVolumeMapper is the abstract definition of a volume mapper for
 * unstructured data (vtkUnstructuredGrid). Several basic types of volume mappers
 * are supported as subclasses.
 *
 * @sa
 * vtkUnstructuredGridVolumeRayCastMapper
 */

#ifndef vtkUnstructuredGridVolumeMapper_h
#define vtkUnstructuredGridVolumeMapper_h

#include "vtkAbstractVolumeMapper.h"
#include "vtkRenderingVolumeModule.h" // For export macro

class vtkRenderer;
class vtkVolume;
class vtkUnstructuredGridBase;
class vtkWindow;

class VTKRENDERINGVOLUME_EXPORT vtkUnstructuredGridVolumeMapper : public vtkAbstractVolumeMapper
{
public:
  vtkTypeMacro(vtkUnstructuredGridVolumeMapper, vtkAbstractVolumeMapper);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Set/Get the input data
   */
  virtual void SetInputData(vtkUnstructuredGridBase*);
  virtual void SetInputData(vtkDataSet*);
  vtkUnstructuredGridBase* GetInput();
  //@}

  vtkSetMacro(BlendMode, int);
  void SetBlendModeToComposite()
  {
    this->SetBlendMode(vtkUnstructuredGridVolumeMapper::COMPOSITE_BLEND);
  }
  void SetBlendModeToMaximumIntensity()
  {
    this->SetBlendMode(vtkUnstructuredGridVolumeMapper::MAXIMUM_INTENSITY_BLEND);
  }
  vtkGetMacro(BlendMode, int);

  /**
   * WARNING: INTERNAL METHOD - NOT INTENDED FOR GENERAL USE
   * DO NOT USE THIS METHOD OUTSIDE OF THE RENDERING PROCESS
   * Render the volume
   */
  void Render(vtkRenderer* ren, vtkVolume* vol) override = 0;

  /**
   * WARNING: INTERNAL METHOD - NOT INTENDED FOR GENERAL USE
   * Release any graphics resources that are being consumed by this mapper.
   * The parameter window could be used to determine which graphic
   * resources to release.
   */
  void ReleaseGraphicsResources(vtkWindow*) override {}

  enum
  {
    COMPOSITE_BLEND,
    MAXIMUM_INTENSITY_BLEND
  };

protected:
  vtkUnstructuredGridVolumeMapper();
  ~vtkUnstructuredGridVolumeMapper() override;

  int BlendMode;

  int FillInputPortInformation(int, vtkInformation*) override;

private:
  vtkUnstructuredGridVolumeMapper(const vtkUnstructuredGridVolumeMapper&) = delete;
  void operator=(const vtkUnstructuredGridVolumeMapper&) = delete;
};

#endif
