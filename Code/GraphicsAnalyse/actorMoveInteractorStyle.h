#ifndef ACTORMOVEINTERACTORSTYLE_H
#define ACTORMOVEINTERACTORSTYLE_H
#include "qobject.h"
#include "graphicsanalyse_global.h"
#include <vtkCommand.h>
#include <global.h>
#include <vtkSmartPointer.h>
#include "vtkRenderWindowInteractor.h"
#include "vtkCallbackCommand.h"
#include "vtkInteractorStyleTrackballActor.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkCellPicker.h"

class GRAPHICSANALYSESHARED_EXPORT actorMoveInteractorStyle :public QObject, public vtkInteractorStyleTrackballActor
{
	Q_OBJECT
public:
	static actorMoveInteractorStyle *New();
	vtkTypeMacro(actorMoveInteractorStyle, vtkInteractorStyleTrackballActor);
	
	virtual void OnLeftButtonDown();
	virtual void OnMouseMove();
	virtual void OnLeftButtonUp();

private:
	double pos1[3];
	double pos2[3];
signals:
	void sig_restoreInteractor();
	void sig_movePosition(double*);

};
#endif