/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkInformationDataObjectKey.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkInformationDataObjectKey
 * @brief   Key for vtkDataObject values.
 *
 * vtkInformationDataObjectKey is used to represent keys in
 * vtkInformation for values that are vtkDataObject instances.
 */

#ifndef vtkInformationDataObjectKey_h
#define vtkInformationDataObjectKey_h

#include "vtkCommonCoreModule.h" // For export macro
#include "vtkInformationKey.h"

#include "vtkCommonInformationKeyManager.h" // Manage instances of this type.

class vtkDataObject;

class VTKCOMMONCORE_EXPORT vtkInformationDataObjectKey : public vtkInformationKey
{
public:
  vtkTypeMacro(vtkInformationDataObjectKey, vtkInformationKey);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  vtkInformationDataObjectKey(const char* name, const char* location);
  ~vtkInformationDataObjectKey() override;

  /**
   * This method simply returns a new vtkInformationDataObjectKey, given a
   * name and a location. This method is provided for wrappers. Use the
   * constructor directly from C++ instead.
   */
  static vtkInformationDataObjectKey* MakeKey(const char* name, const char* location)
  {
    return new vtkInformationDataObjectKey(name, location);
  }

  //@{
  /**
   * Get/Set the value associated with this key in the given
   * information object.
   */
  void Set(vtkInformation* info, vtkDataObject*);
  vtkDataObject* Get(vtkInformation* info);
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

private:
  vtkInformationDataObjectKey(const vtkInformationDataObjectKey&) = delete;
  void operator=(const vtkInformationDataObjectKey&) = delete;
};

#endif
