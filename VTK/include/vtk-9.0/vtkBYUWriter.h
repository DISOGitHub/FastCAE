/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkBYUWriter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkBYUWriter
 * @brief   write MOVIE.BYU files
 *
 * vtkBYUWriter writes MOVIE.BYU polygonal files. These files consist
 * of a geometry file (.g), a scalar file (.s), a displacement or
 * vector file (.d), and a 2D texture coordinate file (.t). These files
 * must be specified to the object, the appropriate boolean
 * variables must be true, and data must be available from the input
 * for the files to be written.
 * WARNING: this writer does not currently write triangle strips. Use
 * vtkTriangleFilter to convert strips to triangles.
 */

#ifndef vtkBYUWriter_h
#define vtkBYUWriter_h

#include "vtkIOGeometryModule.h" // For export macro
#include "vtkWriter.h"

class vtkPolyData;

class VTKIOGEOMETRY_EXPORT vtkBYUWriter : public vtkWriter
{
public:
  static vtkBYUWriter* New();

  vtkTypeMacro(vtkBYUWriter, vtkWriter);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Specify the name of the geometry file to write.
   */
  vtkSetStringMacro(GeometryFileName);
  vtkGetStringMacro(GeometryFileName);
  //@}

  //@{
  /**
   * Specify the name of the displacement file to write.
   */
  vtkSetStringMacro(DisplacementFileName);
  vtkGetStringMacro(DisplacementFileName);
  //@}

  //@{
  /**
   * Specify the name of the scalar file to write.
   */
  vtkSetStringMacro(ScalarFileName);
  vtkGetStringMacro(ScalarFileName);
  //@}

  //@{
  /**
   * Specify the name of the texture file to write.
   */
  vtkSetStringMacro(TextureFileName);
  vtkGetStringMacro(TextureFileName);
  //@}

  //@{
  /**
   * Turn on/off writing the displacement file.
   */
  vtkSetMacro(WriteDisplacement, vtkTypeBool);
  vtkGetMacro(WriteDisplacement, vtkTypeBool);
  vtkBooleanMacro(WriteDisplacement, vtkTypeBool);
  //@}

  //@{
  /**
   * Turn on/off writing the scalar file.
   */
  vtkSetMacro(WriteScalar, vtkTypeBool);
  vtkGetMacro(WriteScalar, vtkTypeBool);
  vtkBooleanMacro(WriteScalar, vtkTypeBool);
  //@}

  //@{
  /**
   * Turn on/off writing the texture file.
   */
  vtkSetMacro(WriteTexture, vtkTypeBool);
  vtkGetMacro(WriteTexture, vtkTypeBool);
  vtkBooleanMacro(WriteTexture, vtkTypeBool);
  //@}

  //@{
  /**
   * Get the input to this writer.
   */
  vtkPolyData* GetInput();
  vtkPolyData* GetInput(int port);
  //@}

protected:
  vtkBYUWriter();
  ~vtkBYUWriter() override;

  void WriteData() override;

  char* GeometryFileName;
  char* DisplacementFileName;
  char* ScalarFileName;
  char* TextureFileName;
  vtkTypeBool WriteDisplacement;
  vtkTypeBool WriteScalar;
  vtkTypeBool WriteTexture;

  void WriteGeometryFile(FILE* fp, int numPts);
  void WriteDisplacementFile(int numPts);
  void WriteScalarFile(int numPts);
  void WriteTextureFile(int numPts);

  int FillInputPortInformation(int port, vtkInformation* info) override;

private:
  vtkBYUWriter(const vtkBYUWriter&) = delete;
  void operator=(const vtkBYUWriter&) = delete;
};

#endif
