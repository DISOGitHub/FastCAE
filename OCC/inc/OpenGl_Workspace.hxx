// Created on: 2011-09-20
// Created by: Sergey ZERCHANINOV
// Copyright (c) 2011-2013 OPEN CASCADE SAS
//
// This file is part of Open CASCADE Technology software library.
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published
// by the Free Software Foundation, with special exception defined in the file
// OCCT_LGPL_EXCEPTION.txt. Consult the file LICENSE_LGPL_21.txt included in OCCT
// distribution for complete text of the license and disclaimer of any warranty.
//
// Alternatively, this file may be used under the terms of Open CASCADE
// commercial license or contractual agreement.

#ifndef _OpenGl_Workspace_Header
#define _OpenGl_Workspace_Header

#include <Graphic3d_BufferType.hxx>

#include <OpenGl_Aspects.hxx>
#include <OpenGl_CappingAlgo.hxx>
#include <OpenGl_FrameBuffer.hxx>
#include <OpenGl_Material.hxx>
#include <OpenGl_Matrix.hxx>
#include <OpenGl_ShaderObject.hxx>
#include <OpenGl_ShaderProgram.hxx>
#include <OpenGl_TextureBufferArb.hxx>
#include <OpenGl_RenderFilter.hxx>
#include <OpenGl_Vec.hxx>
#include <OpenGl_Window.hxx>

class OpenGl_View;
class Image_PixMap;

class OpenGl_Workspace;
DEFINE_STANDARD_HANDLE(OpenGl_Workspace,Standard_Transient)

//! Rendering workspace.
//! Provides methods to render primitives and maintain GL state.
class OpenGl_Workspace : public Standard_Transient
{
public:

  //! Constructor of rendering workspace.
  Standard_EXPORT OpenGl_Workspace (OpenGl_View* theView, const Handle(OpenGl_Window)& theWindow);

  //! Destructor
  virtual ~OpenGl_Workspace() {}

  //! Activate rendering context.
  Standard_EXPORT Standard_Boolean Activate();

  OpenGl_View* View() const { return myView; }

  const Handle(OpenGl_Context)& GetGlContext() { return myGlContext; }

  Standard_EXPORT Handle(OpenGl_FrameBuffer) FBOCreate (const Standard_Integer theWidth, const Standard_Integer theHeight);

  Standard_EXPORT void FBORelease (Handle(OpenGl_FrameBuffer)& theFbo);

  Standard_Boolean BufferDump (const Handle(OpenGl_FrameBuffer)& theFbo,
                               Image_PixMap&                     theImage,
                               const Graphic3d_BufferType&       theBufferType);

  Standard_EXPORT Standard_Integer Width()  const;

  Standard_EXPORT Standard_Integer Height() const;

  //! Setup Z-buffer usage flag (without affecting GL state!).
  //! Returns previously set flag.
  Standard_Boolean SetUseZBuffer (const Standard_Boolean theToUse)
  {
    const Standard_Boolean wasUsed = myUseZBuffer;
    myUseZBuffer = theToUse;
    return wasUsed;
  }

  //! @return true if usage of Z buffer is enabled.
  Standard_Boolean& UseZBuffer() { return myUseZBuffer; }

  //! @return true if depth writing is enabled.
  Standard_Boolean& UseDepthWrite() { return myUseDepthWrite; }

  //! Configure default polygon offset parameters.
  //! Return previous settings.
  Standard_EXPORT Graphic3d_PolygonOffset SetDefaultPolygonOffset (const Graphic3d_PolygonOffset& theOffset);

  //// RELATED TO STATUS ////

  //! Return true if active group might activate face culling (e.g. primitives are closed).
  bool ToAllowFaceCulling() const { return myToAllowFaceCulling; }

  //! Allow or disallow face culling.
  //! This call does NOT affect current state of back face culling;
  //! ApplyAspectFace() should be called to update state.
  bool SetAllowFaceCulling (bool theToAllow)
  {
    const bool wasAllowed = myToAllowFaceCulling;
    myToAllowFaceCulling = theToAllow;
    return wasAllowed;
  }

  //! Return true if following structures should apply highlight color.
  bool ToHighlight() const { return !myHighlightStyle.IsNull(); }

  //! Return highlight style.
  const Handle(Graphic3d_PresentationAttributes)& HighlightStyle() const { return myHighlightStyle; }

  //! Set highlight style.
  void SetHighlightStyle (const Handle(Graphic3d_PresentationAttributes)& theStyle) {  myHighlightStyle = theStyle; }

  //! Return edge color taking into account highlight flag.
  const OpenGl_Vec4& EdgeColor() const
  {
    return !myHighlightStyle.IsNull()
         ?  myHighlightStyle->ColorRGBA()
         :  myAspectsSet->Aspect()->EdgeColorRGBA();
  }

  //! Return Interior color taking into account highlight flag.
  const OpenGl_Vec4& InteriorColor() const
  {
    return !myHighlightStyle.IsNull()
         ?  myHighlightStyle->ColorRGBA()
         :  myAspectsSet->Aspect()->InteriorColorRGBA();
  }

  //! Return text color taking into account highlight flag.
  const OpenGl_Vec4& TextColor() const
  {
    return !myHighlightStyle.IsNull()
         ?  myHighlightStyle->ColorRGBA()
         :  myAspectsSet->Aspect()->ColorRGBA();
  }

  //! Return text Subtitle color taking into account highlight flag.
  const OpenGl_Vec4& TextSubtitleColor() const
  {
    return !myHighlightStyle.IsNull()
         ?  myHighlightStyle->ColorRGBA()
         :  myAspectsSet->Aspect()->ColorSubTitleRGBA();
  }

  //! Currently set aspects (can differ from applied).
  const OpenGl_Aspects* Aspects() const { return myAspectsSet; }

  //! Assign new aspects (will be applied within ApplyAspects()).
  Standard_EXPORT const OpenGl_Aspects* SetAspects (const OpenGl_Aspects* theAspect);

  //! Apply aspects.
  //! @return aspect set by SetAspects()
  Standard_EXPORT const OpenGl_Aspects* ApplyAspects();

  //! Clear the applied aspect state to default values.
  void ResetAppliedAspect();

  //! Get rendering filter.
  //! @sa ShouldRender()
  Standard_Integer RenderFilter() const { return myRenderFilter; }

  //! Set filter for restricting rendering of particular elements.
  //! @sa ShouldRender()
  void SetRenderFilter (Standard_Integer theFilter) { myRenderFilter = theFilter; }

  //! Checks whether the element can be rendered or not.
  //! @param theElement [in] the element to check
  //! @return True if element can be rendered
  bool ShouldRender (const OpenGl_Element* theElement);

  //! Return the number of skipped transparent elements within active OpenGl_RenderFilter_OpaqueOnly filter.
  //! @sa OpenGl_LayerList::Render()
  Standard_Integer NbSkippedTransparentElements() { return myNbSkippedTranspElems; }

  //! Reset skipped transparent elements counter.
  //! @sa OpenGl_LayerList::Render()
  void ResetSkippedCounter() { myNbSkippedTranspElems = 0; }

  //! @return applied view matrix.
  inline const OpenGl_Matrix* ViewMatrix() const { return ViewMatrix_applied; }

  //! @return applied model structure matrix.
  inline const OpenGl_Matrix* ModelMatrix() const { return StructureMatrix_applied; }

  //! Returns face aspect for none culling mode.
  const OpenGl_Aspects& NoneCulling() const { return myNoneCulling; }

  //! Returns face aspect for front face culling mode.
  const OpenGl_Aspects& FrontCulling() const { return myFrontCulling; }

  //! Sets a new environment texture.
  void SetEnvironmentTexture (const Handle(OpenGl_TextureSet)& theTexture) { myEnvironmentTexture = theTexture; }

  //! Returns environment texture.
  const Handle(OpenGl_TextureSet)& EnvironmentTexture() const { return myEnvironmentTexture; }

protected: //! @name protected fields

  OpenGl_View*           myView;
  Handle(OpenGl_Window)  myWindow;
  Handle(OpenGl_Context) myGlContext;
  Standard_Boolean       myUseZBuffer;
  Standard_Boolean       myUseDepthWrite;
  OpenGl_Aspects         myNoneCulling;
  OpenGl_Aspects         myFrontCulling;

protected: //! @name fields related to status

  Standard_Integer myNbSkippedTranspElems; //!< counter of skipped transparent elements for OpenGl_LayerList two rendering passes method
  Standard_Integer myRenderFilter;         //!< active filter for skipping rendering of elements by some criteria (multiple render passes)

  OpenGl_Aspects   myDefaultAspects;
  const OpenGl_Aspects*      myAspectsSet;
  Handle(Graphic3d_Aspects)  myAspectsApplied;

  Handle(Graphic3d_PresentationAttributes) myAspectFaceAppliedWithHL;

  const OpenGl_Matrix* ViewMatrix_applied;
  const OpenGl_Matrix* StructureMatrix_applied;

  bool            myToAllowFaceCulling; //!< allow back face culling
  Handle(Graphic3d_PresentationAttributes) myHighlightStyle; //!< active highlight style

  OpenGl_Matrix myModelViewMatrix; //!< Model matrix with applied structure transformations

  OpenGl_Aspects myAspectFaceHl; //!< Hiddenline aspect

  Handle(OpenGl_TextureSet) myEnvironmentTexture;

public: //! @name type definition

  DEFINE_STANDARD_RTTIEXT(OpenGl_Workspace,Standard_Transient)
  DEFINE_STANDARD_ALLOC

};

#endif // _OpenGl_Workspace_Header
