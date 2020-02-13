#ifndef ACTORINTERACTORSTYLE_H
#define ACTORINTERACTORSTYLE_H

#include "graphicsanalyse_global.h"
#include <vtkCommand.h>
#include <global.h>
#include <vtkSmartPointer.h>
#include "vtkRenderWindowInteractor.h"
#include "vtkCallbackCommand.h"
#include "vtkPropPicker.h"
#include "vtkPointPicker.h"
#include "vtkTransform.h"
#include "vtkActor.h"
#include "vtkInteractorStyleTrackballActor.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkDataSet.h"
#include "vtkCellPicker.h"
#include "qobject.h"

class GRAPHICSANALYSESHARED_EXPORT actorInteractorStyle :public QObject, public vtkInteractorStyleTrackballActor
{
	Q_OBJECT
public:
	static actorInteractorStyle *New();
	vtkTypeMacro(actorInteractorStyle, vtkInteractorStyleTrackballActor);
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnMouseMove();
	virtual void Rotate();
	void set_rotateType(ActorOperateType tep_rotateType) { m_rotateType = tep_rotateType;};
	ActorOperateType get_rotateType() { return m_rotateType; };
private:
	ActorOperateType m_rotateType;
signals:
	void sig_restoreInteractor();
};
#endif