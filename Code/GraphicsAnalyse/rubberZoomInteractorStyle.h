#ifndef RUBBERZOOMINTERACTORSTYLE_H
#define RUBBERZOOMINTERACTORSTYLE_H

#include <QObject>
#include "graphicsanalyse_global.h"
#include "vtkDataSetMapper.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkWidgetEvent.h"
#include "vtkCallbackCommand.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include <vtkInteractorStyleRubberBandZoom.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkUnstructuredGrid.h>
#include "PipelineObject.h"
#include "string.h"
#include "global.h"
#include "qobject.h"

class rubberZoomInteractorStyle : public QObject,public vtkInteractorStyleRubberBandZoom
{
	Q_OBJECT
public:
	static rubberZoomInteractorStyle *New();
	vtkTypeMacro(rubberZoomInteractorStyle, vtkInteractorStyleRubberBandZoom);
	
	
	virtual void OnLeftButtonUp();
private:
	
signals:
	void sig_restoreInteractor();
};
#endif // RUBBERZOOMINTERACTORSTYLE_H


