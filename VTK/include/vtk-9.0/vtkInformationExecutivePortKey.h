/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkInformationExecutivePortKey.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkInformationExecutivePortKey
 * @brief   Key for vtkExecutive/Port value pairs.
 *
 * vtkInformationExecutivePortKey is used to represent keys in
 * vtkInformation for values that are vtkExecutive instances paired
 * with port numbers.
 */

#ifndef vtkInformationExecutivePortKey_h
#define vtkInformationExecutivePortKey_h

#include "vtkCommonExecutionModelModule.h" // For export macro
#include "vtkInformationKey.h"

#include "vtkFilteringInformationKeyManager.h" // Manage instances of this type.

class vtkExecutive;

class VTKCOMMONEXECUTIONMODEL_EXPORT vtkInformationExecutivePortKey : public vtkInformationKey
{
public:
  vtkTypeMacro(vtkInformationExecutivePortKey, vtkInformationKey);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  vtkInformationExecutivePortKey(const char* name, const char* location);
  ~vtkInformationExecutivePortKey() override;

  /**
   * This method simply returns a new vtkInformationExecutivePortKey, given a
   * name and a location. This method is provided for wrappers. Use the
   * constructor directly from C++ instead.
   */
  static vtkInformationExecutivePortKey* MakeKey(const char* name, const char* location)
  {
    return new vtkInformationExecutivePortKey(name, location);
  }

  //@{
  /**
   * Get/Set the value associated with this key in the given
   * information object.
   */
  void Set(vtkInformation* info, vtkExecutive*, int);
  vtkExecutive* GetExecutive(vtkInformation* info);
  int GetPort(vtkInformation* info);
  void Get(vtkInformation* info, vtkExecutive*& executive, int& port);
  //@}

  /**
   * Copy the entry associated with this key from one information
   * object to another.  If there is no entry in the first information
   * object for this key, the value is removed from the second.
   */
  void ShallowCopy(vtkInformation* from, vtkInformation* to) override;

  /**
   * Report a reference this key has in the given information object.
   */
  void Report(vtkInformation* info, vtkGarbageCollector* collector) override;

  /**
   * Print the key's value in an information object to a stream.
   */
  void Print(ostream& os, vtkInformation* info) override;

private:
  vtkInformationExecutivePortKey(const vtkInformationExecutivePortKey&) = delete;
  void operator=(const vtkInformationExecutivePortKey&) = delete;
};

#endif
