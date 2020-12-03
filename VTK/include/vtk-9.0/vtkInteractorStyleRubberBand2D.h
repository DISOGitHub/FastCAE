/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkInteractorStyleRubberBand2D.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/*-------------------------------------------------------------------------
  Copyright 2008 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
  the U.S. Government retains certain rights in this software.
-------------------------------------------------------------------------*/
/**
 * @class   vtkInteractorStyleRubberBand2D
 * @brief   A rubber band interactor for a 2D view
 *
 *
 * vtkInteractorStyleRubberBand2D manages interaction in a 2D view.
 * Camera rotation is not allowed with this interactor style.
 * Zooming affects the camera's parallel scale only, and assumes
 * that the camera is in parallel projection mode.
 * The style also allows draws a rubber band using the left button.
 * All camera changes invoke StartInteractionEvent when the button
 * is pressed, InteractionEvent when the mouse (or wheel) is moved,
 * and EndInteractionEvent when the button is released.  The bindings
 * are as follows:
 * Left mouse - Select (invokes a SelectionChangedEvent).
 * Right mouse - Zoom.
 * Middle mouse - Pan.
 * Scroll wheel - Zoom.
 */

#ifndef vtkInteractorStyleRubberBand2D_h
#define vtkInteractorStyleRubberBand2D_h

#include "vtkInteractionStyleModule.h" // For export macro
#include "vtkInteractorStyle.h"

class vtkUnsignedCharArray;

class VTKINTERACTIONSTYLE_EXPORT vtkInteractorStyleRubberBand2D : public vtkInteractorStyle
{
public:
  static vtkInteractorStyleRubberBand2D* New();
  vtkTypeMacro(vtkInteractorStyleRubberBand2D, vtkInteractorStyle);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  void OnLeftButtonDown() override;
  void OnLeftButtonUp() override;
  void OnMiddleButtonDown() override;
  void OnMiddleButtonUp() override;
  void OnRightButtonDown() override;
  void OnRightButtonUp() override;
  void OnMouseMove() override;
  void OnMouseWheelForward() override;
  void OnMouseWheelBackward() override;

  //@{
  /**
   * Whether to invoke a render when the mouse moves.
   */
  vtkSetMacro(RenderOnMouseMove, bool);
  vtkGetMacro(RenderOnMouseMove, bool);
  vtkBooleanMacro(RenderOnMouseMove, bool);
  //@}

  /**
   * Selection types
   */
  enum
  {
    SELECT_NORMAL = 0,
    SELECT_UNION = 1
  };

  //@{
  /**
   * Current interaction state
   */
  vtkGetMacro(Interaction, int);
  //@}

  enum
  {
    NONE,
    PANNING,
    ZOOMING,
    SELECTING
  };

  //@{
  /**
   * Access to the start and end positions (display coordinates) of the rubber
   * band pick area. This is a convenience method for the wrapped languages
   * since the event callData is lost when using those wrappings.
   */
  vtkGetVector2Macro(StartPosition, int);
  vtkGetVector2Macro(EndPosition, int);
  //@}

protected:
  vtkInteractorStyleRubberBand2D();
  ~vtkInteractorStyleRubberBand2D() override;

  // The interaction mode
  int Interaction;

  // Draws the selection rubber band
  void RedrawRubberBand();

  // The start position of the selection
  int StartPosition[2];

  // The end position of the selection
  int EndPosition[2];

  // The pixel array for the rubber band
  vtkUnsignedCharArray* PixelArray;

  // Whether to render when the mouse moves
  bool RenderOnMouseMove;

private:
  vtkInteractorStyleRubberBand2D(const vtkInteractorStyleRubberBand2D&) = delete;
  void operator=(const vtkInteractorStyleRubberBand2D&) = delete;
};

#endif
