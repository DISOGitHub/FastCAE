/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkXMLPHierarchicalBoxDataWriter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkXMLPHierarchicalBoxDataWriter
 * @brief   parallel writer for
 * vtkHierarchicalBoxDataSet for backwards compatibility.
 *
 * vtkXMLPHierarchicalBoxDataWriter is an empty subclass of
 * vtkXMLPUniformGridAMRWriter for backwards compatibility.
 */

#ifndef vtkXMLPHierarchicalBoxDataWriter_h
#define vtkXMLPHierarchicalBoxDataWriter_h

#include "vtkIOParallelXMLModule.h" // For export macro
#include "vtkXMLPUniformGridAMRWriter.h"

class VTKIOPARALLELXML_EXPORT vtkXMLPHierarchicalBoxDataWriter : public vtkXMLPUniformGridAMRWriter
{
public:
  static vtkXMLPHierarchicalBoxDataWriter* New();
  vtkTypeMacro(vtkXMLPHierarchicalBoxDataWriter, vtkXMLPUniformGridAMRWriter);
  void PrintSelf(ostream& os, vtkIndent indent) override;

protected:
  vtkXMLPHierarchicalBoxDataWriter();
  ~vtkXMLPHierarchicalBoxDataWriter() override;

private:
  vtkXMLPHierarchicalBoxDataWriter(const vtkXMLPHierarchicalBoxDataWriter&) = delete;
  void operator=(const vtkXMLPHierarchicalBoxDataWriter&) = delete;
};

#endif
