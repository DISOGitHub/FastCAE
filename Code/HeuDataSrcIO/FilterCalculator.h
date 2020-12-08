#ifndef FILTERCALCULATOR_H
#define FILTERCALCULATOR_H

#include "PipelineObject.h"
#include "heudatasrcio_global.h"
#include "vtkActor.h"
#include "vtkPolyDataMapper.h"
#include <vtkReflectionFilter.h>
#include <vtkPolyData.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include "vtkFunctionParser.h"
#include "vtkArrayCalculator.h"
class HEUDATASRCIOSHARED_EXPORT FilterCalculator :
	public PipelineObject
{
public:
	FilterCalculator(const QString& dataFile, PipelineObject * parent);
	~FilterCalculator();
	virtual QString  GetName() const;
	virtual Def_PipelineObjType GetObjectType() const { return dCalculator_DataSource; }
	void generate_actors();
	void setCalculator();
private:
	vtkSmartPointer<vtkDataSetMapper> calMap;
	QString mDataFile;
	void func_generate_actors();
	vtkSmartPointer<vtkArrayCalculator> arrCalculator;
	vtkSmartPointer<vtkFloatArray> resultArr;
};
#endif
