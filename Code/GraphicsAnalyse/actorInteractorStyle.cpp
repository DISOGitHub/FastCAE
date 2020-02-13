#include "actorInteractorStyle.h"
#include "vtkRenderWindow.h"
#include "qdebug.h"
#include "vtkMatrix4x4.h"
#include "vtkRenderer.h"
#include "vtkUnstructuredGrid.h"

void actorInteractorStyle::OnLeftButtonDown()
{
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];

	this->FindPokedRenderer(x, y);
	this->FindPickedActor(x, y);
	if (this->CurrentRenderer == NULL || this->InteractionProp == NULL)
	{
		return;
	}

	this->GrabFocus(this->EventCallbackCommand);
	this->StartRotate();
	vtkInteractorStyleTrackballActor::OnLeftButtonDown();
}

void actorInteractorStyle::OnMouseMove()
{
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];
	this->FindPokedRenderer(x, y);
	this->Rotate();
	this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
}

void actorInteractorStyle::OnLeftButtonUp()
{
	this->EndRotate();
	if (this->Interactor)
	{
		this->ReleaseFocus();
	}
	vtkInteractorStyleTrackballActor::OnLeftButtonUp();	
	emit sig_restoreInteractor();
}

void actorInteractorStyle::Rotate()
{
	if (this->CurrentRenderer == NULL || this->InteractionProp == NULL)
	{
		return;
	}

	vtkRenderWindowInteractor *rwi = this->Interactor;
	vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();

	// First get the origin of the assembly
	double *obj_center = this->InteractionProp->GetCenter();
	// GetLength gets the length of the diagonal of the bounding box
	double boundRadius = this->InteractionProp->GetLength() * 0.5;
	// Get the view up and view right vectors
	double view_up[3], view_look[3], view_right[3];

	cam->OrthogonalizeViewUp();
	cam->ComputeViewPlaneNormal();
	cam->GetViewUp(view_up);
	vtkMath::Normalize(view_up);
	cam->GetViewPlaneNormal(view_look);
	vtkMath::Cross(view_up, view_look, view_right);
	vtkMath::Normalize(view_right);

	// Get the furtherest point from object position+origin
	double outsidept[3];

	outsidept[0] = obj_center[0] + view_right[0] * boundRadius;
	outsidept[1] = obj_center[1] + view_right[1] * boundRadius;
	outsidept[2] = obj_center[2] + view_right[2] * boundRadius;

	// Convert them to display coord
	double disp_obj_center[3];

	this->ComputeWorldToDisplay(obj_center[0], obj_center[1], obj_center[2],
		disp_obj_center);

	this->ComputeWorldToDisplay(outsidept[0], outsidept[1], outsidept[2],
		outsidept);

	double radius = sqrt(vtkMath::Distance2BetweenPoints(disp_obj_center,
		outsidept));
	double nxf = (rwi->GetEventPosition()[0] - disp_obj_center[0]) / radius;

	double nyf = (rwi->GetEventPosition()[1] - disp_obj_center[1]) / radius;

	double oxf = (rwi->GetLastEventPosition()[0] - disp_obj_center[0]) / radius;

	double oyf = (rwi->GetLastEventPosition()[1] - disp_obj_center[1]) / radius;

	if (((nxf * nxf + nyf * nyf) <= 1.0) &&
		((oxf * oxf + oyf * oyf) <= 1.0))
	{
		double newXAngle = vtkMath::DegreesFromRadians(asin(nxf));
		double newYAngle = vtkMath::DegreesFromRadians(asin(nyf));
		double oldXAngle = vtkMath::DegreesFromRadians(asin(oxf));
		double oldYAngle = vtkMath::DegreesFromRadians(asin(oyf));

		double scale[3];
		scale[0] = scale[1] = scale[2] = 1.0;

		double **rotate = new double*[2];

		rotate[0] = new double[4];
		rotate[1] = new double[4];

		if (get_rotateType() == kActor_Rotate)
		{
			rotate[0][0] = newXAngle - oldXAngle;
			rotate[0][1] = view_up[0];
			rotate[0][2] = view_up[1];
			rotate[0][3] = view_up[2];

			rotate[1][0] = oldYAngle - newYAngle;
			rotate[1][1] = view_right[0];
			rotate[1][2] = view_right[1];
			rotate[1][3] = view_right[2];
		}
		else if (get_rotateType() == kActor_RotateX)
		{
			rotate[0][0] = newXAngle - oldXAngle;
			rotate[0][1] = view_up[0];
			rotate[0][2] = 0;
			rotate[0][3] = 0;

			rotate[1][0] = oldYAngle - newYAngle;
			rotate[1][1] = view_right[0];
			rotate[1][2] = 0;
			rotate[1][3] = 0;
		}
		else if (get_rotateType() == kActor_RotateY)
		{
			rotate[0][0] = newXAngle - oldXAngle;
			rotate[0][1] = 0;
			rotate[0][2] = view_up[1];
			rotate[0][3] = 0;

			rotate[1][0] = oldYAngle - newYAngle;
			rotate[1][1] = 0;
			rotate[1][2] = view_right[1];
			rotate[1][3] = 0;
		}
		else if (get_rotateType() == kActor_RotateZ)
		{
			rotate[0][0] = newXAngle - oldXAngle;
			rotate[0][1] = 0;
			rotate[0][2] = 0;
			rotate[0][3] = view_up[2];

			rotate[1][0] = oldYAngle - newYAngle;
			rotate[1][1] = 0;
			rotate[1][2] = 0;
			rotate[1][3] = view_right[2];
		}

		this->Prop3DTransform(this->InteractionProp,
			obj_center,
			2,
			rotate,
			scale);

		delete[] rotate[0];
		delete[] rotate[1];
		delete[] rotate;

		if (this->AutoAdjustCameraClippingRange)
		{
			this->CurrentRenderer->ResetCameraClippingRange();
		}

		rwi->Render();
	}
}

