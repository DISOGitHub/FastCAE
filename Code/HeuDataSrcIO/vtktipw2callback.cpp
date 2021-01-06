#include "vtktipw2callback.h"

void vtkTIPW2Callback::Execute(vtkObject *caller, unsigned long, void*)
{
    vtkImplicitPlaneWidget2 *planeWidget =
        reinterpret_cast<vtkImplicitPlaneWidget2*>(caller);
    vtkImplicitPlaneRepresentation *rep =
        reinterpret_cast<vtkImplicitPlaneRepresentation*>(planeWidget->GetRepresentation());
    rep->GetPlane(this->Plane);
    this->Plane->GetOrigin(origin);
    this->Plane->GetNormal(normal);
    emit sig_update_planeProp_widget(origin, normal);
    
}
