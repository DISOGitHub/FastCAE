#ifndef FILTERSLICE_H
#define FILTERSLICE_H

#include <QObject>
#include "PipelineObject.h"
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
#include "vtktipw2callback.h"

class HEUDATASRCIOSHARED_EXPORT FilterSlice : public PipelineObject
{
	Q_OBJECT
public:
	vtkSmartPointer<vtkTIPW2Callback> get_callback() { return myCallback; };
	void set_plane_interactor(vtkRenderWindowInteractor* get_plane_interactor){ plane_interactor = get_plane_interactor; }; 
	
	void get_slice_pos_data(double tep_val[6]);
	void plane_visible(bool flag);
	FilterSlice(const QString& dataFile, PipelineObject * parent);
	~FilterSlice();
	virtual QString          GetName() const;
	virtual Def_PipelineObjType GetObjectType() const { return dSlice_DataSource; }
	void generate_actors();
	void init_plane();
	void SetCenter(double x, double y, double z); 
	void SetNormal(double x, double y, double z); 
	void slot_changeNormals();
	void slot_changeOrigins();
	void slot_changeCameraNormals();
	void close_planeWidget();
private:
	vtkSmartPointer<vtkTIPW2Callback> myCallback;
	vtkRenderWindowInteractor* plane_interactor;
	vtkSmartPointer<vtkImplicitPlaneWidget2> planeWidget;
	vtkPlane* plane; 
	vtkSmartPointer< vtkCylinder> cylinderpanel;
	vtkSmartPointer<vtkCutter> cutter;
	vtkSmartPointer <vtkOutlineFilter> outline;
	vtkSmartPointer <vtkDataSetMapper> slinceoutlineMapper;
	vtkSmartPointer <vtkDataSetMapper> outlineMapper;
	vtkSmartPointer <vtkImplicitPlaneRepresentation> sliceRep;
	vtkSmartPointer<vtkActor> outlineActor;
	QString mDataFile;
	struct sliceFilter_pipelineObj_propData mFilterSliceProp;
	double origin_center[3];
	double normal[3];
	void getActorOrigin(vtkSmartPointer<vtkDataSet> tep_dataSet);
	void func_init_plane();
	void func_generate_actors();
};
#endif

