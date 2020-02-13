#include "myInteractorStyle.h"
#include "vtkCamera.h"

void MyInteractorStyle::StopState()
{
	vtkInteractorStyleTrackballCamera::StopState();
	Camera_Para tep_data;
	vtkRenderer* tep_renderer = this->GetCurrentRenderer();
	if (tep_renderer == NULL)
		return;
	vtkCamera* tep_camera=tep_renderer->GetActiveCamera();
	if (tep_camera == NULL)
		return;
	tep_data.position[0] = tep_camera->GetPosition()[0];
	tep_data.position[1] = tep_camera->GetPosition()[1];
	tep_data.position[2] = tep_camera->GetPosition()[2];
	tep_data.focalPoint[0] = tep_camera->GetFocalPoint()[0];
	tep_data.focalPoint[1] = tep_camera->GetFocalPoint()[1];
	tep_data.focalPoint[2] = tep_camera->GetFocalPoint()[2];
	tep_data.clippingRange[0] = tep_camera->GetClippingRange()[0];
	tep_data.clippingRange[1] = tep_camera->GetClippingRange()[1];
	tep_data.viewUp[0] = tep_camera->GetViewUp()[0];
	tep_data.viewUp[1] = tep_camera->GetViewUp()[1];
	tep_data.viewUp[2] = tep_camera->GetViewUp()[2];
	tep_data.viewAngle = tep_camera->GetViewAngle();
	if (tep_camera->GetParallelProjection())
	{
		tep_data.flag_parallel = true;
		tep_data.parallel_scale = tep_camera->GetParallelScale();
	}
	else
		tep_data.flag_parallel = false;
	cameraPara_list.insert(0,tep_data );
	if (cameraPara_list.count() > 10)
	{
		cameraPara_list.removeAt(10);
	}
	set_cur_pos(0);
	emit sig_update_UndoReDoButton(cur_pos);
}

