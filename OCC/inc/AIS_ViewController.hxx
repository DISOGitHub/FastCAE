// Copyright (c) 2016-2019 OPEN CASCADE SAS
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

#ifndef _AIS_ViewController_HeaderFile
#define _AIS_ViewController_HeaderFile

#include <Aspect_VKeySet.hxx>
#include <Aspect_TouchMap.hxx>
#include <AIS_DragAction.hxx>
#include <AIS_MouseGesture.hxx>
#include <AIS_NavigationMode.hxx>
#include <AIS_ViewInputBuffer.hxx>
#include <AIS_RotationMode.hxx>
#include <AIS_WalkDelta.hxx>

#include <gp_Pnt.hxx>
#include <Graphic3d_Vec3.hxx>
#include <NCollection_Array1.hxx>
#include <OSD_Timer.hxx>
#include <Precision.hxx>
#include <Standard_Mutex.hxx>

class AIS_AnimationCamera;
class AIS_InteractiveObject;
class AIS_InteractiveContext;
class AIS_Point;
class AIS_RubberBand;
class V3d_View;

//! Auxiliary structure for handling viewer events between GUI and Rendering threads.
//!
//! Class implements the following features:
//! - Buffers storing the state of user input (mouse, touches and keyboard).
//! - Mapping mouse/multi-touch input to View camera manipulations (panning/rotating/zooming).
//! - Input events are not applied immediately but queued for separate processing from two working threads
//!   UI thread receiving user input and Rendering thread for OCCT 3D Viewer drawing.
class AIS_ViewController
{
public:

  //! Empty constructor.
  Standard_EXPORT AIS_ViewController();

  //! Return input buffer.
  const AIS_ViewInputBuffer& InputBuffer (AIS_ViewInputBufferType theType) const { return theType == AIS_ViewInputBufferType_UI ? myUI : myGL; }

  //! Return input buffer.
  AIS_ViewInputBuffer& ChangeInputBuffer (AIS_ViewInputBufferType theType)       { return theType == AIS_ViewInputBufferType_UI ? myUI : myGL; }

  //! Return view animation; empty (but not NULL) animation by default.
  const Handle(AIS_AnimationCamera)& ViewAnimation() const { return myViewAnimation; }

  //! Set view animation to be handled within handleViewRedraw().
  void SetViewAnimation (const Handle(AIS_AnimationCamera)& theAnimation) { myViewAnimation = theAnimation; }

  //! Interrupt active view animation.
  Standard_EXPORT void AbortViewAnimation();

public: //! @name global parameters

  //! Return camera rotation mode, AIS_RotationMode_BndBoxActive by default.
  AIS_RotationMode RotationMode() const { return myRotationMode; }

  //! Set camera rotation mode.
  void SetRotationMode (AIS_RotationMode theMode) { myRotationMode = theMode; }

  //! Return camera navigation mode; AIS_NavigationMode_Orbit by default.
  AIS_NavigationMode NavigationMode() const { return myNavigationMode; }

  //! Set camera navigation mode.
  Standard_EXPORT void SetNavigationMode (AIS_NavigationMode theMode);

  //! Return mouse input acceleration ratio in First Person mode; 1.0 by default.
  float MouseAcceleration() const { return myMouseAccel; }

  //! Set mouse input acceleration ratio.
  void SetMouseAcceleration (float theRatio) { myMouseAccel = theRatio; }

  //! Return orbit rotation acceleration ratio; 1.0 by default.
  float OrbitAcceleration() const { return myOrbitAccel; }

  //! Set orbit rotation acceleration ratio.
  void SetOrbitAcceleration (float theRatio) { myOrbitAccel = theRatio; }

  //! Return TRUE if panning anchor point within perspective projection should be displayed in 3D Viewer; TRUE by default.
  bool ToShowPanAnchorPoint() const { return myToShowPanAnchorPoint; }

  //! Set if panning anchor point within perspective projection should be displayed in 3D Viewer.
  void SetShowPanAnchorPoint (bool theToShow) { myToShowPanAnchorPoint = theToShow; }

  //! Return TRUE if rotation point should be displayed in 3D Viewer; TRUE by default.
  bool ToShowRotateCenter() const { return myToShowRotateCenter; }

  //! Set if rotation point should be displayed in 3D Viewer.
  void SetShowRotateCenter (bool theToShow) { myToShowRotateCenter = theToShow; }

  //! Return TRUE if camera up orientation within AIS_NavigationMode_Orbit rotation mode should be forced Z up; FALSE by default.
  bool ToLockOrbitZUp() const { return myToLockOrbitZUp; }

  //! Set if camera up orientation within AIS_NavigationMode_Orbit rotation mode should be forced Z up.
  void SetLockOrbitZUp (bool theToForceUp) { myToLockOrbitZUp = theToForceUp; }

  //! Return TRUE if z-rotation via two-touches gesture is enabled; FALSE by default.
  bool ToAllowTouchZRotation() const { return myToAllowTouchZRotation; }

  //! Set if z-rotation via two-touches gesture is enabled.
  void SetAllowTouchZRotation (bool theToEnable) { myToAllowTouchZRotation = theToEnable; }

  //! Return TRUE if camera rotation is allowed; TRUE by default.
  bool ToAllowRotation() const { return myToAllowRotation; }

  //! Set if camera rotation is allowed.
  void SetAllowRotation (bool theToEnable) { myToAllowRotation = theToEnable; }

  //! Return TRUE if panning is allowed; TRUE by default.
  bool ToAllowPanning() const { return myToAllowPanning; }

  //! Set if panning is allowed.
  void SetAllowPanning (bool theToEnable) { myToAllowPanning = theToEnable; }

  //! Return TRUE if zooming is allowed; TRUE by default.
  bool ToAllowZooming() const { return myToAllowZooming; }

  //! Set if zooming is allowed.
  void SetAllowZooming (bool theToEnable) { myToAllowZooming = theToEnable; }

  //! Return TRUE if ZFocus change is allowed; TRUE by default.
  bool ToAllowZFocus() const { return myToAllowZFocus; }

  //! Set if ZFocus change is allowed.
  void SetAllowZFocus (bool theToEnable) { myToAllowZFocus = theToEnable; }

  //! Return TRUE if dynamic highlight on mouse move is allowed; TRUE by default.
  bool ToAllowHighlight() const { return myToAllowHighlight; }

  //! Set if dragging object is allowed.
  void SetAllowHighlight (bool theToEnable) { myToAllowHighlight = theToEnable; }

  //! Return TRUE if dragging object is allowed; TRUE by default.
  bool ToAllowDragging() const { return myToAllowDragging; }

  //! Set if dynamic highlight on mouse move is allowed.
  void SetAllowDragging (bool theToEnable) { myToAllowDragging = theToEnable; }

  //! Return TRUE if picked point should be projected to picking ray on zooming at point; TRUE by default.
  bool ToStickToRayOnZoom() const { return myToStickToRayOnZoom; }

  //! Set if picked point should be projected to picking ray on zooming at point.
  void SetStickToRayOnZoom (bool theToEnable) { myToStickToRayOnZoom = theToEnable; }

  //! Return TRUE if picked point should be projected to picking ray on rotating around point; TRUE by default.
  bool ToStickToRayOnRotation() const { return myToStickToRayOnRotation; }

  //! Set if picked point should be projected to picking ray on rotating around point.
  void SetStickToRayOnRotation (bool theToEnable) { myToStickToRayOnRotation = theToEnable; }

  //! Return TRUE if pitch direction should be inverted while processing Aspect_VKey_NavLookUp/Aspect_VKey_NavLookDown; FALSE by default.
  bool ToInvertPitch() const { return myToInvertPitch; }

  //! Set flag inverting pitch direction.
  void SetInvertPitch (bool theToInvert) { myToInvertPitch = theToInvert; }

  //! Return normal walking speed, in m/s; 1.5 by default.
  float WalkSpeedAbsolute() const { return myWalkSpeedAbsolute; }

  //! Set normal walking speed, in m/s; 1.5 by default.
  void SetWalkSpeedAbsolute (float theSpeed) { myWalkSpeedAbsolute = theSpeed; }

  //! Return walking speed relative to scene bounding box; 0.1 by default.
  float WalkSpeedRelative() const { return myWalkSpeedRelative; }

  //! Set walking speed relative to scene bounding box.
  void SetWalkSpeedRelative (float theFactor) { myWalkSpeedRelative = theFactor; }

  //! Return active thrust value; 0.0f by default.
  float ThrustSpeed() const { return myThrustSpeed; }

  //! Set active thrust value.
  void SetThrustSpeed (float theSpeed) { myThrustSpeed = theSpeed; }

  //! Return TRUE if previous position of MoveTo has been defined.
  bool HasPreviousMoveTo() const { return myPrevMoveTo != Graphic3d_Vec2i (-1); }

  //! Return previous position of MoveTo event in 3D viewer.
  const Graphic3d_Vec2i& PreviousMoveTo() const { return myPrevMoveTo; }

  //! Reset previous position of MoveTo.
  void ResetPreviousMoveTo() { myPrevMoveTo = Graphic3d_Vec2i (-1); }

public: //! @name keyboard input

  //! Return keyboard state.
  const Aspect_VKeySet& Keys() const { return myKeys; }

  //! Return keyboard state.
  Aspect_VKeySet& ChangeKeys() { return myKeys; }

  //! Press key.
  //! @param theKey key pressed
  //! @param theTime event timestamp
  Standard_EXPORT virtual void KeyDown (Aspect_VKey theKey,
                                        double theTime,
                                        double thePressure = 1.0);

  //! Release key.
  //! @param theKey key pressed
  //! @param theTime event timestamp
  Standard_EXPORT virtual void KeyUp (Aspect_VKey theKey,
                                      double theTime);

  //! Simulate key up/down events from axis value.
  Standard_EXPORT virtual void KeyFromAxis (Aspect_VKey theNegative,
                                            Aspect_VKey thePositive,
                                            double theTime,
                                            double thePressure);

  //! Fetch active navigation actions.
  Standard_EXPORT AIS_WalkDelta FetchNavigationKeys (Standard_Real theCrouchRatio,
                                                     Standard_Real theRunRatio);

public: //! @name mouse input

  //! Return map defining mouse gestures.
  const AIS_MouseGestureMap& MouseGestureMap() const { return myMouseGestureMap; }

  //! Return map defining mouse gestures.
  AIS_MouseGestureMap& ChangeMouseGestureMap() { return myMouseGestureMap; }

  //! Return double click interval in seconds; 0.4 by default.
  double MouseDoubleClickInterval() const { return myMouseDoubleClickInt; }

  //! Set double click interval in seconds.
  void SetMouseDoubleClickInterval (double theSeconds) { myMouseDoubleClickInt = theSeconds; }

  //! Perform selection in 3D viewer.
  //! This method is expected to be called from UI thread.
  //! @param thePnt picking point
  //! @param theIsXOR XOR selection flag
  Standard_EXPORT virtual void SelectInViewer (const Graphic3d_Vec2i& thePnt,
                                               const bool theIsXOR = false);

  //! Perform selection in 3D viewer.
  //! This method is expected to be called from UI thread.
  //! @param thePnts picking point
  //! @param theIsXOR XOR selection flag
  Standard_EXPORT virtual void SelectInViewer (const NCollection_Sequence<Graphic3d_Vec2i>& thePnts,
                                               const bool theIsXOR = false);

  //! Update rectangle selection tool.
  //! This method is expected to be called from UI thread.
  //! @param thePntFrom rectangle first   corner
  //! @param thePntTo   rectangle another corner
  //! @param theIsXOR XOR selection flag
  Standard_EXPORT virtual void UpdateRubberBand (const Graphic3d_Vec2i& thePntFrom,
                                                 const Graphic3d_Vec2i& thePntTo,
                                                 const bool theIsXOR = false);

  //! Update polygonal selection tool.
  //! This method is expected to be called from UI thread.
  //! @param thePnt new point to add to polygon
  //! @param theToAppend append new point or update the last point
  Standard_EXPORT virtual void UpdatePolySelection (const Graphic3d_Vec2i& thePnt,
                                                    bool theToAppend);

  //! Update zoom event (e.g. from mouse scroll).
  //! This method is expected to be called from UI thread.
  //! @param theDelta mouse cursor position to zoom at and zoom delta
  //! @return TRUE if new zoom event has been created or FALSE if existing one has been updated
  Standard_EXPORT virtual bool UpdateZoom (const Aspect_ScrollDelta& theDelta);

  //! Update Z rotation event.
  //! @param theAngle rotation angle, in radians.
  //! @return TRUE if new zoom event has been created or FALSE if existing one has been updated
  Standard_EXPORT virtual bool UpdateZRotation (double theAngle);

  //! Update mouse scroll event; redirects to UpdateZoom by default.
  //! This method is expected to be called from UI thread.
  //! @param theDelta mouse cursor position and delta
  //! @return TRUE if new event has been created or FALSE if existing one has been updated
  Standard_EXPORT virtual bool UpdateMouseScroll (const Aspect_ScrollDelta& theDelta);

  //! Handle mouse button press/release event.
  //! This method is expected to be called from UI thread.
  //! @param thePoint      mouse cursor position
  //! @param theButtons    pressed buttons
  //! @param theModifiers  key modifiers
  //! @param theIsEmulated if TRUE then mouse event comes NOT from real mouse
  //!                      but emulated from non-precise input like touch on screen
  //! @return TRUE if View should be redrawn
  Standard_EXPORT virtual bool UpdateMouseButtons (const Graphic3d_Vec2i& thePoint,
                                                   Aspect_VKeyMouse theButtons,
                                                   Aspect_VKeyFlags theModifiers,
                                                   bool theIsEmulated);

  //! Handle mouse cursor movement event.
  //! This method is expected to be called from UI thread.
  //! @param thePoint      mouse cursor position
  //! @param theButtons    pressed buttons
  //! @param theModifiers  key modifiers
  //! @param theIsEmulated if TRUE then mouse event comes NOT from real mouse
  //!                      but emulated from non-precise input like touch on screen
  //! @return TRUE if View should be redrawn
  Standard_EXPORT virtual bool UpdateMousePosition (const Graphic3d_Vec2i& thePoint,
                                                    Aspect_VKeyMouse theButtons,
                                                    Aspect_VKeyFlags theModifiers,
                                                    bool theIsEmulated);

  //! Handle mouse button press event.
  //! This method is expected to be called from UI thread.
  //! @param thePoint      mouse cursor position
  //! @param theButton     pressed button
  //! @param theModifiers  key modifiers
  //! @param theIsEmulated if TRUE then mouse event comes NOT from real mouse
  //!                      but emulated from non-precise input like touch on screen
  //! @return TRUE if View should be redrawn
  bool PressMouseButton (const Graphic3d_Vec2i& thePoint,
                         Aspect_VKeyMouse theButton,
                         Aspect_VKeyFlags theModifiers,
                         bool theIsEmulated)
  {
    return UpdateMouseButtons (thePoint, myMousePressed | theButton, theModifiers, theIsEmulated);
  }

  //! Handle mouse button release event.
  //! This method is expected to be called from UI thread.
  //! @param thePoint      mouse cursor position
  //! @param theButton     released button
  //! @param theModifiers  key modifiers
  //! @param theIsEmulated if TRUE then mouse event comes NOT from real mouse
  //!                      but emulated from non-precise input like touch on screen
  //! @return TRUE if View should be redrawn
  bool ReleaseMouseButton (const Graphic3d_Vec2i& thePoint,
                           Aspect_VKeyMouse theButton,
                           Aspect_VKeyFlags theModifiers,
                           bool theIsEmulated)
  {
    Aspect_VKeyMouse aButtons = myMousePressed & (~theButton);
    return UpdateMouseButtons (thePoint, aButtons, theModifiers, theIsEmulated);
  }

  //! Handle mouse button click event (emulated by UpdateMouseButtons() while releasing single button).
  //! Note that as this method is called by UpdateMouseButtons(), it should be executed from UI thread.
  //! Default implementation redirects to SelectInViewer().
  //! This method is expected to be called from UI thread.
  //! @param thePoint      mouse cursor position
  //! @param theButton     clicked button
  //! @param theModifiers  key modifiers
  //! @param theIsDoubleClick flag indicating double mouse click
  //! @return TRUE if View should be redrawn
  Standard_EXPORT virtual bool UpdateMouseClick (const Graphic3d_Vec2i& thePoint,
                                                 Aspect_VKeyMouse theButton,
                                                 Aspect_VKeyFlags theModifiers,
                                                 bool theIsDoubleClick);

  //! Return currently pressed mouse buttons.
  Aspect_VKeyMouse PressedMouseButtons() const { return myMousePressed; }

  //! Return active key modifiers passed with last mouse event.
  Aspect_VKeyFlags LastMouseFlags() const { return myMouseModifiers; }

  //! Return last mouse position.
  const Graphic3d_Vec2i& LastMousePosition() const { return myMousePositionLast; }

public: //! @name multi-touch input

  //! Return scale factor for adjusting tolerances for starting multi-touch gestures; 1.0 by default
  //! This scale factor is expected to be computed from touch screen resolution.
  float TouchToleranceScale() const { return myTouchToleranceScale; }

  //! Set scale factor for adjusting tolerances for starting multi-touch gestures.
  void SetTouchToleranceScale (float theTolerance) { myTouchToleranceScale = theTolerance; }

  //! Return TRUE if touches map is not empty.
  bool HasTouchPoints() const { return !myTouchPoints.IsEmpty(); }

  //! Add touch point with the given ID.
  //! This method is expected to be called from UI thread.
  //! @param theId touch unique identifier
  //! @param thePnt touch coordinates
  //! @param theClearBefore if TRUE previously registered touches will be removed
  Standard_EXPORT virtual void AddTouchPoint (Standard_Size theId,
                                              const Graphic3d_Vec2d& thePnt,
                                              Standard_Boolean theClearBefore = false);

  //! Remove touch point with the given ID.
  //! This method is expected to be called from UI thread.
  //! @param theId touch unique identifier
  //! @param theClearSelectPnts if TRUE will initiate clearing of selection points
  //! @return TRUE if point has been removed
  Standard_EXPORT virtual bool RemoveTouchPoint (Standard_Size theId,
                                                 Standard_Boolean theClearSelectPnts = false);

  //! Update touch point with the given ID.
  //! If point with specified ID was not registered before, it will be added.
  //! This method is expected to be called from UI thread.
  //! @param theId touch unique identifier
  //! @param thePnt touch coordinates
  Standard_EXPORT virtual void UpdateTouchPoint (Standard_Size theId,
                                                 const Graphic3d_Vec2d& thePnt);

public:

  //! Return event time (e.g. current time).
  double EventTime() const { return myEventTimer.ElapsedTime(); }

  //! Reset input state (pressed keys, mouse buttons, etc.) e.g. on window focus loss.
  //! This method is expected to be called from UI thread.
  Standard_EXPORT virtual void ResetViewInput();

  //! Reset view orientation.
  //! This method is expected to be called from UI thread.
  Standard_EXPORT virtual void UpdateViewOrientation (V3d_TypeOfOrientation theOrientation,
                                                      bool theToFitAll);

  //! Update buffer for rendering thread.
  //! This method is expected to be called within synchronization barrier between GUI
  //! and Rendering threads (e.g. GUI thread should be locked beforehand to avoid data races).
  //! @param theCtx interactive context
  //! @param theView active view
  //! @param theToHandle if TRUE, the HandleViewEvents() will be called
  Standard_EXPORT virtual void FlushViewEvents (const Handle(AIS_InteractiveContext)& theCtx,
                                                const Handle(V3d_View)& theView,
                                                Standard_Boolean theToHandle = Standard_False);

  //! Process events within rendering thread.
  Standard_EXPORT virtual void HandleViewEvents (const Handle(AIS_InteractiveContext)& theCtx,
                                                 const Handle(V3d_View)& theView);

public:

  //! Callback called by handleMoveTo() on Selection in 3D Viewer.
  //! This method is expected to be called from rendering thread.
  Standard_EXPORT virtual void OnSelectionChanged (const Handle(AIS_InteractiveContext)& theCtx,
                                                   const Handle(V3d_View)& theView);

  //! Callback called by handleMoveTo() on dragging object in 3D Viewer.
  //! This method is expected to be called from rendering thread.
  Standard_EXPORT virtual void OnObjectDragged (const Handle(AIS_InteractiveContext)& theCtx,
                                                const Handle(V3d_View)& theView,
                                                AIS_DragAction theAction);

  //! Pick closest point under mouse cursor.
  //! This method is expected to be called from rendering thread.
  //! @param thePnt   [out] result point
  //! @param theCtx    [in] interactive context
  //! @param theView   [in] active view
  //! @param theCursor [in] mouse cursor
  //! @param theToStickToPickRay [in] when TRUE, the result point will lie on picking ray
  //! @return TRUE if result has been found
  Standard_EXPORT virtual bool PickPoint (gp_Pnt& thePnt,
                                          const Handle(AIS_InteractiveContext)& theCtx,
                                          const Handle(V3d_View)& theView,
                                          const Graphic3d_Vec2i& theCursor,
                                          bool theToStickToPickRay);

  //! Compute rotation gravity center point depending on rotation mode.
  //! This method is expected to be called from rendering thread.
  Standard_EXPORT virtual gp_Pnt GravityPoint (const Handle(AIS_InteractiveContext)& theCtx,
                                               const Handle(V3d_View)& theView);

public:

  //! Perform camera actions.
  //! This method is expected to be called from rendering thread.
  Standard_EXPORT virtual void handleCameraActions (const Handle(AIS_InteractiveContext)& theCtx,
                                                    const Handle(V3d_View)& theView,
                                                    const AIS_WalkDelta& theWalk);

  //! Perform moveto/selection/dragging.
  //! This method is expected to be called from rendering thread.
  Standard_EXPORT virtual void handleMoveTo (const Handle(AIS_InteractiveContext)& theCtx,
                                             const Handle(V3d_View)& theView);

  //! Return TRUE if another frame should be drawn right after this one.
  bool toAskNextFrame() const { return myToAskNextFrame; }

  //! Set if another frame should be drawn right after this one.
  void setAskNextFrame (bool theToDraw = true) { myToAskNextFrame = theToDraw; }

  //! Return if panning anchor point has been defined.
  bool hasPanningAnchorPoint() const { return !Precision::IsInfinite (myPanPnt3d.X()); }

  //! Return active panning anchor point.
  const gp_Pnt& panningAnchorPoint() const { return myPanPnt3d; }

  //! Set active panning anchor point.
  void setPanningAnchorPoint (const gp_Pnt& thePnt) { myPanPnt3d = thePnt; }

  //! Handle panning event myGL.Panning.
  Standard_EXPORT virtual void handlePanning (const Handle(V3d_View)& theView);

  //! Handle Z rotation event myGL.ZRotate.
  Standard_EXPORT virtual void handleZRotate (const Handle(V3d_View)& theView);

  //! Return minimal camera distance for zoom operation.
  double MinZoomDistance() const { return myMinCamDistance; }

  //! Set minimal camera distance for zoom operation.
  void SetMinZoomDistance (double theDist) { myMinCamDistance = theDist; }

  //! Handle zoom event myGL.ZoomActions.
  //! This method is expected to be called from rendering thread.
  Standard_EXPORT virtual void handleZoom (const Handle(V3d_View)& theView,
                                           const Aspect_ScrollDelta& theParams,
                                           const gp_Pnt* thePnt);

  //! Handle ZScroll event myGL.ZoomActions.
  //! This method is expected to be called from rendering thread.
  Standard_EXPORT virtual void handleZFocusScroll (const Handle(V3d_View)& theView,
                                                   const Aspect_ScrollDelta& theParams);

  //! Handle orbital rotation events myGL.OrbitRotation.
  //! @param theView view to modify
  //! @param thePnt 3D point to rotate around
  //! @param theToLockZUp amend camera to exclude roll angle (put camera Up vector to plane containing global Z and view direction)
  Standard_EXPORT virtual void handleOrbitRotation (const Handle(V3d_View)& theView,
                                                    const gp_Pnt& thePnt,
                                                    bool theToLockZUp);

  //! Handle view direction rotation events myGL.ViewRotation.
  //! This method is expected to be called from rendering thread.
  //! @param theView       camera to modify
  //! @param theYawExtra   extra yaw increment
  //! @param thePitchExtra extra pitch increment
  //! @param theRoll       roll value
  //! @param theToRestartOnIncrement flag indicating flight mode
  Standard_EXPORT virtual void handleViewRotation (const Handle(V3d_View)& theView,
                                                   double theYawExtra,
                                                   double thePitchExtra,
                                                   double theRoll,
                                                   bool theToRestartOnIncrement);

  //! Handle view redraw.
  //! This method is expected to be called from rendering thread.
  Standard_EXPORT virtual void handleViewRedraw (const Handle(AIS_InteractiveContext)& theCtx,
                                                 const Handle(V3d_View)& theView);

protected:

  //! Flush buffers.
  Standard_EXPORT virtual void flushBuffers (const Handle(AIS_InteractiveContext)& theCtx,
                                             const Handle(V3d_View)& theView);

  //! Flush touch gestures.
  Standard_EXPORT virtual void flushGestures (const Handle(AIS_InteractiveContext)& theCtx,
                                              const Handle(V3d_View)& theView);

  //! Return current and previously fetched event times.
  //! This callback is intended to compute delta between sequentially processed events.
  //! @param thePrevTime [out] events time fetched previous time by this method
  //! @param theCurrTime [out] actual events time
  void updateEventsTime (double& thePrevTime,
                         double& theCurrTime)
  {
    thePrevTime = myLastEventsTime;
    myLastEventsTime = EventTime();
    theCurrTime = myLastEventsTime;
  }

  //! Perform selection via mouse click.
  //! This method is expected to be called from rendering thread.
  Standard_EXPORT virtual void handleSelectionPick (const Handle(AIS_InteractiveContext)& theCtx,
                                                    const Handle(V3d_View)& theView);

  //! Perform dynamic highlight on mouse move.
  //! This method is expected to be called from rendering thread.
  Standard_EXPORT virtual void handleDynamicHighlight (const Handle(AIS_InteractiveContext)& theCtx,
                                                       const Handle(V3d_View)& theView);

  //! Perform rubber-band selection.
  //! This method is expected to be called from rendering thread.
  Standard_EXPORT virtual void handleSelectionPoly (const Handle(AIS_InteractiveContext)& theCtx,
                                                    const Handle(V3d_View)& theView);

  //! Lazy AIS_InteractiveContext::MoveTo() with myPrevMoveTo check.
  Standard_EXPORT virtual void contextLazyMoveTo (const Handle(AIS_InteractiveContext)& theCtx,
                                                  const Handle(V3d_View)& theView,
                                                  const Graphic3d_Vec2i& thePnt);

protected:

  AIS_ViewInputBuffer myUI;                       //!< buffer for UI thread
  AIS_ViewInputBuffer myGL;                       //!< buffer for rendering thread

  OSD_Timer           myEventTimer;               //!< timer for timestamping events
  Standard_Real       myLastEventsTime;           //!< last fetched events timer value for computing delta/progress
  Standard_Boolean    myToAskNextFrame;           //!< flag indicating that another frame should be drawn right after this one

  Standard_Real       myMinCamDistance;           //!< minimal camera distance for zoom operation
  AIS_RotationMode    myRotationMode;             //!< rotation mode
  AIS_NavigationMode  myNavigationMode;           //!< navigation mode (orbit rotation / first person)
  Standard_ShortReal  myMouseAccel;               //!< mouse input acceleration ratio in First Person mode
  Standard_ShortReal  myOrbitAccel;               //!< Orbit rotation acceleration ratio
  Standard_Boolean    myToShowPanAnchorPoint;     //!< option displaying panning  anchor point
  Standard_Boolean    myToShowRotateCenter;       //!< option displaying rotation center point
  Standard_Boolean    myToLockOrbitZUp;           //!< force camera up orientation within AIS_NavigationMode_Orbit rotation mode
  Standard_Boolean    myToInvertPitch;            //!< flag inverting pitch direction while processing Aspect_VKey_NavLookUp/Aspect_VKey_NavLookDown
  Standard_Boolean    myToAllowTouchZRotation;    //!< enable z-rotation two-touches gesture; FALSE by default
  Standard_Boolean    myToAllowRotation;          //!< enable rotation; TRUE by default
  Standard_Boolean    myToAllowPanning;           //!< enable panning; TRUE by default
  Standard_Boolean    myToAllowZooming;           //!< enable zooming; TRUE by default
  Standard_Boolean    myToAllowZFocus;            //!< enable ZFocus change; TRUE by default
  Standard_Boolean    myToAllowHighlight;         //!< enable dynamic highlight on mouse move; TRUE by default
  Standard_Boolean    myToAllowDragging;          //!< enable dragging object; TRUE by default
  Standard_Boolean    myToStickToRayOnZoom;       //!< project picked point to ray while zooming at point, TRUE by default
  Standard_Boolean    myToStickToRayOnRotation;   //!< project picked point to ray while rotating around point; TRUE by default

  Standard_ShortReal  myWalkSpeedAbsolute;        //!< normal walking speed, in m/s; 1.5 by default
  Standard_ShortReal  myWalkSpeedRelative;        //!< walking speed relative to scene bounding box; 0.1 by default
  Standard_ShortReal  myThrustSpeed;              //!< active thrust value
  Standard_Boolean    myHasThrust;                //!< flag indicating active thrust

  Handle(AIS_AnimationCamera) myViewAnimation;    //!< view animation
  Handle(AIS_RubberBand) myRubberBand;            //!< Rubber-band presentation
  Handle(AIS_InteractiveObject) myDragObject;     //!< currently dragged object
  Graphic3d_Vec2i     myPrevMoveTo;               //!< previous position of MoveTo event in 3D viewer
  Standard_Boolean    myHasHlrOnBeforeRotation;   //!< flag for restoring Computed mode after rotation

protected: //! @name keyboard input variables

  Aspect_VKeySet      myKeys;                     //!< keyboard state

protected: //! @name mouse input variables

  Standard_Real       myMouseClickThreshold;      //!< mouse click threshold in pixels; 3 by default
  Standard_Real       myMouseDoubleClickInt;      //!< double click interval in seconds; 0.4 by default
  Standard_ShortReal  myScrollZoomRatio;          //!< distance ratio for mapping mouse scroll event to zoom; 15.0 by default

  AIS_MouseGestureMap myMouseGestureMap;          //!< map defining mouse gestures
  AIS_MouseGesture    myMouseActiveGesture;       //!< initiated mouse gesture (by pressing mouse button)
  Standard_Boolean    myMouseActiveIdleRotation;  //!< flag indicating view idle rotation state
  Graphic3d_Vec2i     myMousePositionLast;        //!< last mouse position
  Graphic3d_Vec2i     myMousePressPoint;          //!< mouse position where active gesture was been initiated
  Graphic3d_Vec2i     myMouseProgressPoint;       //!< gesture progress
  OSD_Timer           myMouseClickTimer;          //!< timer for handling double-click event
  Standard_Integer    myMouseClickCounter;        //!< counter for handling double-click event
  Aspect_VKeyMouse    myMousePressed;             //!< active mouse buttons
  Aspect_VKeyFlags    myMouseModifiers;           //!< active key modifiers passed with last mouse event
  Standard_Integer    myMouseSingleButton;        //!< index of mouse button pressed alone (>0)

protected: //! @name multi-touch input variables

  Standard_ShortReal  myTouchToleranceScale;      //!< tolerance scale factor; 1.0 by default
  Standard_ShortReal  myTouchRotationThresholdPx; //!< threshold for starting one-touch rotation     gesture in pixels;  6 by default
  Standard_ShortReal  myTouchZRotationThreshold;  //!< threshold for starting two-touch Z-rotation   gesture in radians; 2 degrees by default
  Standard_ShortReal  myTouchPanThresholdPx;      //!< threshold for starting two-touch panning      gesture in pixels;  4 by default
  Standard_ShortReal  myTouchZoomThresholdPx;     //!< threshold for starting two-touch zoom (pitch) gesture in pixels;  6 by default
  Standard_ShortReal  myTouchZoomRatio;           //!< distance ratio for mapping two-touch zoom (pitch) gesture from pixels to zoom; 0.13 by default

  Aspect_TouchMap     myTouchPoints;              //!< map of active touches
  Graphic3d_Vec2d     myStartPanCoord;            //!< touch coordinates at the moment of starting panning  gesture
  Graphic3d_Vec2d     myStartRotCoord;            //!< touch coordinates at the moment of starting rotating gesture
  Standard_Integer    myNbTouchesLast;            //!< number of touches within previous gesture flush to track gesture changes
  Standard_Boolean    myUpdateStartPointPan;      //!< flag indicating that new anchor  point should be picked for starting panning    gesture
  Standard_Boolean    myUpdateStartPointRot;      //!< flag indicating that new gravity point should be picked for starting rotation   gesture
  Standard_Boolean    myUpdateStartPointZRot;     //!< flag indicating that new gravity point should be picked for starting Z-rotation gesture

protected: //! @name rotation/panning transient state variables

  Handle(AIS_Point)   myAnchorPointPrs1;          //!< anchor point presentation (Graphic3d_ZLayerId_Top)
  Handle(AIS_Point)   myAnchorPointPrs2;          //!< anchor point presentation (Graphic3d_ZLayerId_Topmost)
  gp_Pnt              myPanPnt3d;                 //!< active panning anchor point
  gp_Pnt              myRotatePnt3d;              //!< active rotation center of gravity
  gp_Dir              myCamStartOpUp;             //!< camera Up    direction at the beginning of rotation
  gp_Dir              myCamStartOpDir;            //!< camera View  direction at the beginning of rotation
  gp_Pnt              myCamStartOpEye;            //!< camera Eye    position at the beginning of rotation
  gp_Pnt              myCamStartOpCenter;         //!< camera Center position at the beginning of rotation
  gp_Vec              myCamStartOpToCenter;       //!< vector from rotation gravity point to camera Center at the beginning of rotation
  gp_Vec              myCamStartOpToEye;          //!< vector from rotation gravity point to camera Eye    at the beginning of rotation
  Graphic3d_Vec3d     myRotateStartYawPitchRoll;  //!< camera yaw pitch roll at the beginning of rotation

};

#endif // _AIS_ViewController_HeaderFile
