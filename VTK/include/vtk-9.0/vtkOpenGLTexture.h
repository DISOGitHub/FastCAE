/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkOpenGLTexture.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkOpenGLTexture
 * @brief   OpenGL texture map
 *
 * vtkOpenGLTexture is a concrete implementation of the abstract class
 * vtkTexture. vtkOpenGLTexture interfaces to the OpenGL rendering library.
 */

#ifndef vtkOpenGLTexture_h
#define vtkOpenGLTexture_h

#include "vtkRenderingOpenGL2Module.h" // For export macro
#include "vtkTexture.h"
#include "vtkWeakPointer.h" // needed for vtkWeakPointer.

class vtkRenderWindow;
class vtkTextureObject;

class VTKRENDERINGOPENGL2_EXPORT vtkOpenGLTexture : public vtkTexture
{
public:
  static vtkOpenGLTexture* New();
  vtkTypeMacro(vtkOpenGLTexture, vtkTexture);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Renders a texture map. It first checks the object's modified time
   * to make sure the texture maps Input is valid, then it invokes the
   * Load() method.
   */
  void Render(vtkRenderer* ren) override;

  /**
   * Implement base class method.
   */
  void Load(vtkRenderer*) override;

  // Descsription:
  // Clean up after the rendering is complete.
  void PostRender(vtkRenderer*) override;

  /**
   * Release any graphics resources that are being consumed by this texture.
   * The parameter window could be used to determine which graphic
   * resources to release. Using the same texture object in multiple
   * render windows is NOT currently supported.
   */
  void ReleaseGraphicsResources(vtkWindow*) override;

  /**
   * copy the renderers read buffer into this texture
   */
  void CopyTexImage(int x, int y, int width, int height);

  //@{
  /**
   * Provide for specifying a format for the texture
   */
  vtkGetMacro(IsDepthTexture, int);
  vtkSetMacro(IsDepthTexture, int);
  //@}

  //@{
  /**
   * What type of texture map GL_TEXTURE_2D versus GL_TEXTURE_RECTANGLE
   */
  vtkGetMacro(TextureType, int);
  vtkSetMacro(TextureType, int);
  //@}

  vtkGetObjectMacro(TextureObject, vtkTextureObject);
  void SetTextureObject(vtkTextureObject*);

  /**
   * Return the texture unit used for this texture
   */
  int GetTextureUnit() override;

  /**
   * Is this Texture Translucent?
   * returns false (0) if the texture is either fully opaque or has
   * only fully transparent pixels and fully opaque pixels and the
   * Interpolate flag is turn off.
   */
  int IsTranslucent() override;

protected:
  vtkOpenGLTexture();
  ~vtkOpenGLTexture() override;

  vtkTimeStamp LoadTime;
  vtkWeakPointer<vtkRenderWindow> RenderWindow; // RenderWindow used for previous render

  bool ExternalTextureObject;
  vtkTextureObject* TextureObject;

  int IsDepthTexture;
  int TextureType;
  int PrevBlendParams[4];

  // used when the texture exceeds the GL limit
  unsigned char* ResampleToPowerOfTwo(
    int& xsize, int& ysize, unsigned char* dptr, int bpp, int maxTexSize);

private:
  vtkOpenGLTexture(const vtkOpenGLTexture&) = delete;
  void operator=(const vtkOpenGLTexture&) = delete;
};

#endif
