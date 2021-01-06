#ifndef FILTERSMOOTH_H
#define FILTERSMOOTH_H

#include "PipelineObject.h"
#include "heudatasrcio_global.h"
#include <vtkCellDataToPointData.h>
#include "vtkActor.h"
#include "vtkPolyDataMapper.h"
#include <vtkSmoothPolyDataFilter.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include "Smooth_VtkSetData.h"
#include <vtkDataSetReader.h>
class HEUDATASRCIOSHARED_EXPORT FilterSmooth :
	public PipelineObject
{
public:
	FilterSmooth(const QString& dataFile, PipelineObject * parent);
	~FilterSmooth();
	virtual QString  GetName() const;
	virtual Def_PipelineObjType GetObjectType() const { return dSmooth_DataSource; }
	void generate_actors();
	void  ChangeColumnColor();
private:
	vtkSmartPointer<vtkPolyData> polyData;
	Smooth_VtkSetData *smoothFilter;
	QString mDataFile;
	struct reflectionFilter_pipelineObj_propData mFilterSmoothProp;
	void func_generate_actors();
};
#endif
