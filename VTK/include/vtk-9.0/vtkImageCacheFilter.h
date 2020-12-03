/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkImageCacheFilter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkImageCacheFilter
 * @brief   Caches multiple vtkImageData objects.
 *
 *
 * vtkImageCacheFilter keep a number of vtkImageDataObjects from previous
 * updates to satisfy future updates without needing to update the input.  It
 * does not change the data at all.  It just makes the pipeline more
 * efficient at the expense of using extra memory.
 */

#ifndef vtkImageCacheFilter_h
#define vtkImageCacheFilter_h

#include "vtkImageAlgorithm.h"
#include "vtkImagingCoreModule.h" // For export macro

class vtkExecutive;

class VTKIMAGINGCORE_EXPORT vtkImageCacheFilter : public vtkImageAlgorithm
{
public:
  static vtkImageCacheFilter* New();
  vtkTypeMacro(vtkImageCacheFilter, vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * This is the maximum number of images that can be retained in memory.
   * it defaults to 10.
   */
  void SetCacheSize(int size);
  int GetCacheSize();
  //@}

protected:
  vtkImageCacheFilter();
  ~vtkImageCacheFilter() override;

  // Create a default executive.
  vtkExecutive* CreateDefaultExecutive() override;
  void ExecuteData(vtkDataObject*) override;

private:
  vtkImageCacheFilter(const vtkImageCacheFilter&) = delete;
  void operator=(const vtkImageCacheFilter&) = delete;
};

#endif
