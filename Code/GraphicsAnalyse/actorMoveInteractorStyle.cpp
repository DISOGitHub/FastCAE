#include "actorMoveInteractorStyle.h"
#include "vtkRenderWindow.h"
#include "qdebug.h"
#include "vtkMatrix4x4.h"
#include "vtkRenderer.h"
#include "vtkUnstructuredGrid.h"
#include "vtkTransformFilter.h"
#include "vtkTransform.h"
void actorMoveInteractorStyle::OnMouseMove()
{
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];
	this->FindPokedRenderer(x, y);
	this->Pan();
	this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
	vtkInteractorStyleTrackballActor::OnMouseMove();
}

void actorMoveInteractorStyle::OnLeftButtonDown()
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
	this->StartPan();
	this->InteractionProp->GetPosition(pos1);
}

void actorMoveInteractorStyle::OnLeftButtonUp()
{
	this->EndPan();
	if (this->Interactor)
	{
		this->ReleaseFocus();
	}
	////////////////////////////////////////////
	/*this->InteractionProp->GetPosition(pos2);
	vtkPropCollection* tep_coll;
	vtkTransform* tep_transForm = vtkTransform::New();
	tep_transForm->Identity();
	tep_transForm->PostMultiply();
	tep_transForm->GetOrientation();
	tep_transForm->Translate(pos2);
	vtkDataSet* tep_dateSet = ((vtkActor*)this->InteractionProp)->GetMapper()->GetInput();
	vtkTransformFilter* tep_filter = vtkTransformFilter::New();
	tep_filter->SetInputData(tep_dateSet);
	tep_filter->SetTransform(tep_transForm);
	tep_filter->Update();
	vtkPointSet* tep_pointSet = tep_filter->GetOutput();
	((vtkUnstructuredGrid*)tep_dateSet)->SetPoints(tep_pointSet->GetPoints());*/
	//////////////////////////////////////////////
	//pos2[0]=this->InteractionProp->GetCenter()[0];
	//pos2[1] = this->InteractionProp->GetCenter()[1];
	//pos2[2] = this->InteractionProp->GetCenter()[2];
	this->InteractionProp->GetPosition(pos2);
	vtkInteractorStyleTrackballActor::OnLeftButtonUp();
	emit sig_movePosition(pos2);
	emit sig_restoreInteractor();
}
