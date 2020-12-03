/*=========================================================================

  Program:   Visualization Toolkit

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef vtkOpenGLIndexBufferObject_h
#define vtkOpenGLIndexBufferObject_h

#include "vtkOpenGLBufferObject.h"
#include "vtkRenderingOpenGL2Module.h" // for export macro

/**
 * @brief OpenGL vertex buffer object
 *
 * OpenGL buffer object to store geometry and/or attribute data on the
 * GPU.
 */

class VTKRENDERINGOPENGL2_EXPORT vtkOpenGLIndexBufferObject : public vtkOpenGLBufferObject
{
public:
  static vtkOpenGLIndexBufferObject* New();
  vtkTypeMacro(vtkOpenGLIndexBufferObject, vtkOpenGLBufferObject);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  // Sizes/offsets are all in bytes as OpenGL API expects them.
  size_t IndexCount; // Number of indices in the VBO

  // Description:
  // used to create an IBO for triangle primitives
  size_t CreateTriangleIndexBuffer(vtkCellArray* cells, vtkPoints* points);

  // Description:
  // used to create an IBO for triangle primitives
  static void AppendTriangleIndexBuffer(std::vector<unsigned int>& indexArray, vtkCellArray* cells,
    vtkPoints* points, vtkIdType vertexOffset);

  // Description:
  // create a IBO for wireframe polys/tris
  size_t CreateTriangleLineIndexBuffer(vtkCellArray* cells);

  // Description:
  // used to create an IBO for line primitives
  static void AppendLineIndexBuffer(
    std::vector<unsigned int>& indexArray, vtkCellArray* cells, vtkIdType vertexOffset);

  // Description:
  // create a IBO for wireframe polys/tris
  size_t CreateLineIndexBuffer(vtkCellArray* cells);

  // Description:
  // create a IBO for wireframe polys/tris
  static void AppendTriangleLineIndexBuffer(
    std::vector<unsigned int>& indexArray, vtkCellArray* cells, vtkIdType vertexOffset);

  // Description:
  // used to create an IBO for primitives as points
  size_t CreatePointIndexBuffer(vtkCellArray* cells);

  // Description:
  // used to create an IBO for primitives as points
  static void AppendPointIndexBuffer(
    std::vector<unsigned int>& indexArray, vtkCellArray* cells, vtkIdType vertexOffset);

  // Description:
  // used to create an IBO for line strips and triangle strips
  size_t CreateStripIndexBuffer(vtkCellArray* cells, bool wireframeTriStrips);

  static void AppendStripIndexBuffer(std::vector<unsigned int>& indexArray, vtkCellArray* cells,
    vtkIdType vertexOffset, bool wireframeTriStrips);

  // Description:
  // special index buffer for polys wireframe with edge visibilityflags
  static void AppendEdgeFlagIndexBuffer(std::vector<unsigned int>& indexArray, vtkCellArray* cells,
    vtkIdType vertexOffset, vtkDataArray* edgeflags);

  size_t CreateEdgeFlagIndexBuffer(vtkCellArray* cells, vtkDataArray* edgeflags);

  // Description:
  // used to create an IBO for cell Vertices as points
  size_t CreateVertexIndexBuffer(vtkCellArray** cells);

  // Description:
  // used to create an IBO for primitives as points
  static void AppendVertexIndexBuffer(
    std::vector<unsigned int>& indexArray, vtkCellArray** cells, vtkIdType vertexOffset);

protected:
  vtkOpenGLIndexBufferObject();
  ~vtkOpenGLIndexBufferObject() override;

private:
  vtkOpenGLIndexBufferObject(const vtkOpenGLIndexBufferObject&) = delete;
  void operator=(const vtkOpenGLIndexBufferObject&) = delete;
};

#endif
