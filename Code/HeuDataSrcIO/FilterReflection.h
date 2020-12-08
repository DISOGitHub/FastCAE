#ifndef FILTERREFLECTION_H
#define FILTERREFLECTION_H

#include "PipelineObject.h"
#include "heudatasrcio_global.h"
#include "vtkActor.h"
#include "vtkPolyDataMapper.h"
#include <vtkReflectionFilter.h>
#include <vtkPolyData.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
class HEUDATASRCIOSHARED_EXPORT FilterReflection :
	public PipelineObject
{
public:
	FilterReflection(const QString& dataFile, PipelineObject * parent);
	~FilterReflection();
	virtual QString  GetName() const;
	virtual Def_PipelineObjType GetObjectType() const { return dReflection_DataSource; }
	void generate_actors();
private:
	vtkSmartPointer<vtkReflectionFilter> reflectionFilter;
	vtkSmartPointer<vtkDataSetMapper> refMap;
	QString mDataFile;
	void func_generate_actors();
};
#endif
