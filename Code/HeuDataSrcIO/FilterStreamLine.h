#ifndef FILTERSTREAMLINE_H
#define FILTERSTREAMLINE_H

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
#include "vtkArrowSource.h"
#include "vtkConeSource.h"
#include "vtkMaskPoints.h"
#include "vtkGlyph3D.h"
#include "vtkDataSetSurfaceFilter.h"
#include "vtkPolyData.h"
#include "vtkPolyDataNormals.h"
#include "vtkCellCenters.h"
#include "vtkStreamTracer.h"
#include "vtkPointSource.h"
#include "vtkDataArray.h"
class HEUDATASRCIOSHARED_EXPORT FilterStreamLine :
	public PipelineObject
{
public:
	FilterStreamLine(const QString& dataFile, PipelineObject * parent);
	~FilterStreamLine();
	virtual QString          GetName() const;
	virtual Def_PipelineObjType GetObjectType() const { return dStreamLine_DataSource; }
	void generate_actors();
	void  ChangeColumnColor();
	void initReflectionPara();
private:
	vtkSmartPointer<vtkPolyData> pointData;
	vtkSmartPointer<vtkStreamTracer> streamTracer;
	vtkSmartPointer<vtkDataSetMapper> glyph_map;
	vtkSmartPointer<vtkPointSource> seeds;
	double center_point[3];
	QString mDataFile;
	bool MakeGlyphs1();
	QString active_colName;
	vtkDataSet* stream_grid;
};
#endif
