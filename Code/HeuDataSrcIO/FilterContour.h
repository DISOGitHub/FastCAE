#ifndef FILTERCONTOUR_H
#define FILTERCONTOUR_H

#include "PipelineObject.h"
#include "heudatasrcio_global.h"
#include "vtkActor.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkContourFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkRectilinearSynchronizedTemplates.h"
#include "vtkCellDataToPointData.h"

class HEUDATASRCIOSHARED_EXPORT FilterContour :
	public PipelineObject
{
public:
	FilterContour(const QString& dataFile, PipelineObject * parent);
	~FilterContour();
	virtual QString          GetName() const;
	virtual Def_PipelineObjType GetObjectType() const { return dContour_DataSource; }
	void generate_actors();
private:
	vtkSmartPointer<vtkContourFilter> contourFilter;
	vtkSmartPointer<vtkDataSetMapper> con_map;
	QString mDataFile;
	struct contourFilter_pipelineObj_propData mFilterContourProp;
	void func_generate_actors();
	
};
#endif
