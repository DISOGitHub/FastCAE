/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkThreadedCompositeDataPipeline.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notice for more information.

  =========================================================================*/
/**
 * @class   vtkThreadedCompositeDataPipeline
 * @brief   Executive that works in parallel
 *
 * vtkThreadedCompositeDataPipeline processes a composite data object in
 * parallel using the SMP framework. It does this by creating a vector of
 * data objects (the pieces of the composite data) and processing them
 * using vtkSMPTools::For. Note that this requires that the
 * algorithm implement all pipeline passes in a re-entrant way. It should
 * store/retrieve all state changes using input and output information
 * objects, which are unique to each thread.
 */

#ifndef vtkThreadedCompositeDataPipeline_h
#define vtkThreadedCompositeDataPipeline_h

#include "vtkCommonExecutionModelModule.h" // For export macro
#include "vtkCompositeDataPipeline.h"

class vtkInformationVector;
class vtkInformation;

class VTKCOMMONEXECUTIONMODEL_EXPORT vtkThreadedCompositeDataPipeline
  : public vtkCompositeDataPipeline
{
public:
  static vtkThreadedCompositeDataPipeline* New();
  vtkTypeMacro(vtkThreadedCompositeDataPipeline, vtkCompositeDataPipeline);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * An API to CallAlgorithm that allows you to pass in the info objects to
   * be used
   */
  int CallAlgorithm(vtkInformation* request, int direction, vtkInformationVector** inInfo,
    vtkInformationVector* outInfo) override;

protected:
  vtkThreadedCompositeDataPipeline();
  ~vtkThreadedCompositeDataPipeline() override;
  void ExecuteEach(vtkCompositeDataIterator* iter, vtkInformationVector** inInfoVec,
    vtkInformationVector* outInfoVec, int compositePort, int connection, vtkInformation* request,
    std::vector<vtkSmartPointer<vtkCompositeDataSet> >& compositeOutput) override;

private:
  vtkThreadedCompositeDataPipeline(const vtkThreadedCompositeDataPipeline&) = delete;
  void operator=(const vtkThreadedCompositeDataPipeline&) = delete;
  friend class ProcessBlock;
};

#endif
