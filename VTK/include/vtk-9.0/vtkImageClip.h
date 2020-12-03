/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkImageClip.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkImageClip
 * @brief   Reduces the image extent of the input.
 *
 * vtkImageClip  will make an image smaller.  The output must have
 * an image extent which is the subset of the input.  The filter has two
 * modes of operation:
 * 1: By default, the data is not copied in this filter.
 * Only the whole extent is modified.
 * 2: If ClipDataOn is set, then you will get no more that the clipped
 * extent.
 */

#ifndef vtkImageClip_h
#define vtkImageClip_h

// I did not make this a subclass of in place filter because
// the references on the data do not matter. I make no modifications
// to the data.
#include "vtkImageAlgorithm.h"
#include "vtkImagingCoreModule.h" // For export macro

class VTKIMAGINGCORE_EXPORT vtkImageClip : public vtkImageAlgorithm
{
public:
  static vtkImageClip* New();
  vtkTypeMacro(vtkImageClip, vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * The whole extent of the output has to be set explicitly.
   */
  void SetOutputWholeExtent(int extent[6], vtkInformation* outInfo = nullptr);
  void SetOutputWholeExtent(int minX, int maxX, int minY, int maxY, int minZ, int maxZ);
  void GetOutputWholeExtent(int extent[6]);
  int* GetOutputWholeExtent() VTK_SIZEHINT(6) { return this->OutputWholeExtent; }
  //@}

  void ResetOutputWholeExtent();

  //@{
  /**
   * By default, ClipData is off, and only the WholeExtent is modified.
   * the data's extent may actually be larger.  When this flag is on,
   * the data extent will be no more than the OutputWholeExtent.
   */
  vtkSetMacro(ClipData, vtkTypeBool);
  vtkGetMacro(ClipData, vtkTypeBool);
  vtkBooleanMacro(ClipData, vtkTypeBool);
  //@}

protected:
  vtkImageClip();
  ~vtkImageClip() override {}

  // Time when OutputImageExtent was computed.
  vtkTimeStamp CTime;
  int Initialized; // Set the OutputImageExtent for the first time.
  int OutputWholeExtent[6];

  vtkTypeBool ClipData;

  int RequestInformation(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  void CopyData(vtkImageData* inData, vtkImageData* outData, int* ext);

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

private:
  vtkImageClip(const vtkImageClip&) = delete;
  void operator=(const vtkImageClip&) = delete;
};

#endif
