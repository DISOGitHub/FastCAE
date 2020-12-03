/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkOpenGLPolyDataMapper2D.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkOpenGLPolyDataMapper2D
 * @brief   2D PolyData support for OpenGL
 *
 * vtkOpenGLPolyDataMapper2D provides 2D PolyData annotation support for
 * vtk under OpenGL.  Normally the user should use vtkPolyDataMapper2D
 * which in turn will use this class.
 *
 * @sa
 * vtkPolyDataMapper2D
 */

#ifndef vtkOpenGLPolyDataMapper2D_h
#define vtkOpenGLPolyDataMapper2D_h

#include "vtkNew.h"          // used for ivars
#include "vtkOpenGLHelper.h" // used for ivars
#include "vtkPolyDataMapper2D.h"
#include "vtkRenderingOpenGL2Module.h" // For export macro
#include <map>                         //for used data arrays & vbos
#include <string>                      // For API.
#include <vector>                      //for ivars

class vtkActor2D;
class vtkGenericOpenGLResourceFreeCallback;
class vtkMatrix4x4;
class vtkOpenGLBufferObject;
class vtkOpenGLCellToVTKCellMap;
class vtkOpenGLHelper;
class vtkOpenGLVertexBufferObjectGroup;
class vtkPoints;
class vtkRenderer;
class vtkTextureObject;
class vtkTransform;

class VTKRENDERINGOPENGL2_EXPORT vtkOpenGLPolyDataMapper2D : public vtkPolyDataMapper2D
{
public:
  vtkTypeMacro(vtkOpenGLPolyDataMapper2D, vtkPolyDataMapper2D);
  static vtkOpenGLPolyDataMapper2D* New();
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Actually draw the poly data.
   */
  void RenderOverlay(vtkViewport* viewport, vtkActor2D* actor) override;

  /**
   * Release any graphics resources that are being consumed by this mapper.
   * The parameter window could be used to determine which graphic
   * resources to release.
   */
  void ReleaseGraphicsResources(vtkWindow*) override;

protected:
  vtkOpenGLPolyDataMapper2D();
  ~vtkOpenGLPolyDataMapper2D() override;

  vtkGenericOpenGLResourceFreeCallback* ResourceCallback;

  /**
   * Does the shader source need to be recomputed
   */
  virtual bool GetNeedToRebuildShaders(vtkOpenGLHelper& cellBO, vtkViewport* ren, vtkActor2D* act);

  /**
   * Build the shader source code
   */
  virtual void BuildShaders(std::string& VertexCode, std::string& fragmentCode,
    std::string& geometryCode, vtkViewport* ren, vtkActor2D* act);

  /**
   * Determine what shader to use and compile/link it
   */
  virtual void UpdateShaders(vtkOpenGLHelper& cellBO, vtkViewport* viewport, vtkActor2D* act);

  /**
   * Set the shader parameteres related to the mapper/input data, called by UpdateShader
   */
  virtual void SetMapperShaderParameters(
    vtkOpenGLHelper& cellBO, vtkViewport* ren, vtkActor2D* act);

  /**
   * Set the shader parameteres related to the Camera
   */
  void SetCameraShaderParameters(vtkOpenGLHelper& cellBO, vtkViewport* viewport, vtkActor2D* act);

  /**
   * Set the shader parameteres related to the property
   */
  void SetPropertyShaderParameters(vtkOpenGLHelper& cellBO, vtkViewport* viewport, vtkActor2D* act);

  /**
   * Perform string replacements on the shader templates, called from
   * ReplaceShaderValues
   */
  virtual void ReplaceShaderPicking(std::string& fssource, vtkRenderer* ren, vtkActor2D* act);

  /**
   * Update the scene when necessary.
   */
  void UpdateVBO(vtkActor2D* act, vtkViewport* viewport);

  // The VBO and its layout.
  vtkOpenGLVertexBufferObjectGroup* VBOs;

  // Structures for the various cell types we render.
  vtkOpenGLHelper Points;
  vtkOpenGLHelper Lines;
  vtkOpenGLHelper Tris;
  vtkOpenGLHelper TriStrips;
  vtkOpenGLHelper* LastBoundBO;

  vtkTextureObject* CellScalarTexture;
  vtkOpenGLBufferObject* CellScalarBuffer;
  bool HaveCellScalars;
  int PrimitiveIDOffset;

  vtkTimeStamp VBOUpdateTime; // When was the VBO updated?
  vtkPoints* TransformedPoints;
  vtkNew<vtkTransform> VBOTransformInverse;
  vtkNew<vtkMatrix4x4> VBOShiftScale;

  int LastPickState;
  vtkTimeStamp PickStateChanged;

  // do we have wide lines that require special handling
  virtual bool HaveWideLines(vtkViewport*, vtkActor2D*);

  // stores the mapping from vtk cells to gl_PrimitiveId
  vtkNew<vtkOpenGLCellToVTKCellMap> CellCellMap;

private:
  vtkOpenGLPolyDataMapper2D(const vtkOpenGLPolyDataMapper2D&) = delete;
  void operator=(const vtkOpenGLPolyDataMapper2D&) = delete;
};

#endif
