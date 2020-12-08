#ifndef PICKAREAINTERACTORSTYLE_H
#define PICKAREAINTERACTORSTYLE_H

#include <QObject>
#include "graphicsanalyse_global.h"
#include "vtkDataSetMapper.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkWidgetEvent.h"
#include "vtkCallbackCommand.h"
#include "vtkRenderWindowInteractor.h"
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkAreaPicker.h>
#include <vtkExtractGeometry.h>
#include <vtkPlanes.h>
#include <vtkAbstractPicker.h>
#include <vtkExtractPolyDataGeometry.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkUnstructuredGrid.h>
#include "PipelineObject.h"
#include "string.h"
#include "consolecmddockwidget.h"
#include "global.h"
#include "vtkPlanes.h"

//struct pick_interactor_propData
//{
//	int flag_pickMode;//-1无拾取，0点，1单元，2区域点，3区域单元
//	vtkActor *pick_actor;
//	vtkIdType pick_id;
//	//vtkPlanes *frustum;
//	PipelineObject* pick_pipelineObj;
//	//vtkRenderer *renderer;
//	vtkIdList *pick_areaPoint;
//	vtkIdList *pick_areaCell;
//	int *objId;
//};
class GRAPHICSANALYSESHARED_EXPORT PickAreaInteractorStyle : public QObject,public vtkInteractorStyleRubberBandPick
{
	Q_OBJECT
public:
	static PickAreaInteractorStyle *New();
	vtkTypeMacro(PickAreaInteractorStyle, vtkInteractorStyleRubberBandPick);
	PickAreaInteractorStyle() : vtkInteractorStyleRubberBandPick()
	{
		dataSet = NULL;
		consoleForm = NULL;
		//pick_Data.frustum = NULL;
		pick_Data.pick_actor = NULL;
		pick_Data.pick_pipelineObj = NULL;
	}
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();

	void SetAreaPicker(vtkSmartPointer<vtkAreaPicker> tep_areaPicker) { areaPicker = tep_areaPicker; };
	void SetDefaultMouseStyle(vtkInteractorStyleTrackballCamera* tep_style) { default_mouse_style = tep_style; };
	void SetPipelineObjectList(QList<PipelineObject*> mPipelineObjs);
	void SetAreaPickFlag(int tep_flag) { pick_Data.flag_pickMode = tep_flag; };
	void SetConsoleForm(consoleCmdDockWidget* tep_form) { consoleForm = tep_form; };
	void SetPickData(struct pick_pipelineObj_propData tep_pick_Data);
	void restoreInteractor();
private:
	vtkDataSet* dataSet;
	vtkSmartPointer<vtkAreaPicker> areaPicker;
	vtkInteractorStyleTrackballCamera* default_mouse_style;
	QList<PipelineObject*> cur_mPipelineObjs;
	//int flag_pick;//2areapoint 3areacell
	consoleCmdDockWidget* consoleForm;
	QString update_selectedCell(vtkIdList* pointId_list, QMap<vtkIdType, QString> cell_output_map);
	bool findIdInIdList(vtkIdType id, vtkIdList* pointId_list);
	struct pick_pipelineObj_propData pick_Data;//struct pick_interactor_propData pick_Data;
	
	void generate_actor();
signals:
	//void sig_restoreInteractor();
	void sig_updatePickData(struct pick_pipelineObj_propData tep_pick_Data);
};

#endif // VTKTIPW2CALLBACK_H


