#ifndef FILTERVECTOR_H
#define FILTERVECTOR_H

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
class HEUDATASRCIOSHARED_EXPORT FilterVector :
	public PipelineObject
{
public:
	FilterVector(const QString& dataFile, PipelineObject * parent);
	~FilterVector();
	virtual QString          GetName() const;
	virtual Def_PipelineObjType GetObjectType() const { return dVector_DataSource; }
	void generate_actors();
	void  ChangeColumnColor();
	
private:
	vtkSmartPointer<vtkPolyData> pointData;
	vtkSmartPointer<vtkArrowSource> mArrowSource;
	vtkSmartPointer<vtkConeSource> mConeSource;
	vtkSmartPointer<vtkMaskPoints> mMask;
	vtkSmartPointer<vtkGlyph3D> glyphFilter;
	vtkSmartPointer<vtkPolyDataMapper> glyph_map;
	vtkSmartPointer<vtkDataSetSurfaceFilter> surfacefliter;
	vtkSmartPointer<vtkPolyDataNormals> normalGenerator;
	QString mDataFile;
	bool MakeGlyphs1();
};
#endif
