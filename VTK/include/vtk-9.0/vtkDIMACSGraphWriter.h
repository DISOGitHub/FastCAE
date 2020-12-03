/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkDIMACSGraphWriter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/*----------------------------------------------------------------------------
 Copyright (c) Sandia Corporation
 See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.
----------------------------------------------------------------------------*/

/**
 * @class   vtkDIMACSGraphWriter
 * @brief   write vtkGraph data to a DIMACS
 * formatted file
 *
 *
 * vtkDIMACSGraphWriter is a sink object that writes
 * vtkGraph data files into a generic DIMACS (.gr) format.
 *
 * Output files contain a problem statement line:
 *
 * p graph <num_verts> <num_edges>
 *
 * Followed by |E| edge descriptor lines that are formatted as:
 *
 * e <source> <target> <weight>
 *
 * Vertices are numbered from 1..n in DIMACS formatted files.
 *
 * See webpage for format details.
 * http://prolland.free.fr/works/research/dsat/dimacs.html
 *
 * @sa
 * vtkDIMACSGraphReader
 *
 */

#ifndef vtkDIMACSGraphWriter_h
#define vtkDIMACSGraphWriter_h

#include "vtkDataWriter.h"
#include "vtkIOInfovisModule.h" // For export macro

class vtkGraph;

class VTKIOINFOVIS_EXPORT vtkDIMACSGraphWriter : public vtkDataWriter
{
public:
  static vtkDIMACSGraphWriter* New();
  vtkTypeMacro(vtkDIMACSGraphWriter, vtkDataWriter);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Get the input to this writer.
   */
  vtkGraph* GetInput();
  vtkGraph* GetInput(int port);
  //@}

protected:
  vtkDIMACSGraphWriter() {}
  ~vtkDIMACSGraphWriter() override {}

  void WriteData() override;

  int FillInputPortInformation(int port, vtkInformation* info) override;

private:
  vtkDIMACSGraphWriter(const vtkDIMACSGraphWriter&) = delete;
  void operator=(const vtkDIMACSGraphWriter&) = delete;
};

#endif
