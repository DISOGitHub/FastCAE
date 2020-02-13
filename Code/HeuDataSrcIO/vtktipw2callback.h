#ifndef VTKTIPW2CALLBACK_H
#define VTKTIPW2CALLBACK_H

#include <QObject>
#include "heudatasrcio_global.h"
#include "vtkCylinder.h"
#include "vtkImplicitPlaneWidget2.h"
#include "vtkCutter.h"
#include "vtkOutlineFilter.h"
#include "vtkLODActor.h"
#include "vtkPlane.h"
#include "vtkImplicitCylinderRepresentation.h"
#include "vtkImplicitPlaneWidget2.h"
#include "vtkImplicitPlaneRepresentation.h"
#include "vtkBoundedPlanePointPlacer.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkWidgetEvent.h"
#include "vtkCallbackCommand.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkClipDataSet.h"
class HEUDATASRCIOSHARED_EXPORT vtkTIPW2Callback : public QObject,public vtkCommand
{
    Q_OBJECT
public:
	explicit vtkTIPW2Callback() : Actor(0) {Plane = NULL; }
    static vtkTIPW2Callback *New()
        {
            return new vtkTIPW2Callback;
        }
    virtual void Execute(vtkObject *caller, unsigned long, void*);

	~vtkTIPW2Callback() { if (Plane != NULL) Plane->Delete(); }
    
	vtkPlane* Plane;
    vtkSmartPointer<vtkActor> Actor;
    double origin[3];
    double normal[3];
signals:
    void sig_update_planeProp_widget(double* origins_val, double* normals_val);

public slots:
};

#endif // VTKTIPW2CALLBACK_H


