#include "rubberZoomInteractorStyle.h"

void rubberZoomInteractorStyle::OnLeftButtonUp()
{
	vtkInteractorStyleRubberBandZoom::OnLeftButtonUp();
	emit sig_restoreInteractor();
}