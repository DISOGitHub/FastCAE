#ifndef MYINTERACTORSTYLE_H
#define MYINTERACTORSTYLE_H

#include <QObject>
#include "graphicsanalyse_global.h"
#include "vtkRenderer.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkWidgetEvent.h"
#include "vtkCallbackCommand.h"
#include "vtkRenderWindowInteractor.h"
#include <vtkSmartPointer.h>
#include "string.h"
#include "global.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "qlist.h"
class GRAPHICSANALYSESHARED_EXPORT MyInteractorStyle : public QObject, public vtkInteractorStyleTrackballCamera
{
	Q_OBJECT
public:
	static MyInteractorStyle *New();
	vtkTypeMacro(MyInteractorStyle, vtkInteractorStyleTrackballCamera);
	MyInteractorStyle() : vtkInteractorStyleTrackballCamera()
	{
		cameraPara_list.clear();
	}
	virtual void StopState();
	QList<Camera_Para> get_cameraPara_list() { return cameraPara_list; };
	void set_cur_pos(int tep_pos) { cur_pos = tep_pos; };
	int get_cur_pos() { return cur_pos; };
private:
	QList<Camera_Para> cameraPara_list;
	int cur_pos;
signals:
	void sig_update_UndoReDoButton(int);
};

#endif 


