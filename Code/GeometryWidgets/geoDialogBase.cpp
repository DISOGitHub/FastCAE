#include "geoDialogBase.h"
#include "geometry/geometryData.h"
#include "mainWindow/mainWindow.h"
#include "MainWidgets/preWindow.h"
#include "python/PyAgent.h"
#include <QPushButton>
#include <QDialogButtonBox>

namespace GeometryWidget
{
	GeoDialogBase::GeoDialogBase(GUI::MainWindow* m, MainWidget::PreWindow* p, bool keySig , bool  res) :
		QFDialog(m, keySig), _preWindow(p), _restoreSelectMode(res)
	{
		_geoData = Geometry::GeometryData::getInstance();
		Qt::WindowFlags flags = this->windowFlags();
		flags |= Qt::WindowStaysOnTopHint;
		this->setWindowFlags(flags);

//		connect(this, SIGNAL(updateGraphOptions()), m, SIGNAL(updateGraphOptionsSig()));
// 		connect(this, SIGNAL(highLightGeometrySet(Geometry::GeometrySet*, bool)), m, SIGNAL(highLightGeometrySetSig(Geometry::GeometrySet*, bool)));
// 		connect(this, SIGNAL(highLightGeometryPoint(Geometry::GeometrySet*, int, QList<vtkActor*>*)), m, SIGNAL(highLightGeometryPointSig(Geometry::GeometrySet*, int, QList<vtkActor*>*)));
// 		connect(this, SIGNAL(highLightGeometryEdge(Geometry::GeometrySet*, int, QList<vtkActor*>*)), m, SIGNAL(highLightGeometryEdgeSig(Geometry::GeometrySet*, int, QList<vtkActor*>*)));
// 		connect(this, SIGNAL(highLightGeometryFace(Geometry::GeometrySet*, int, QList<vtkActor*>*)), m, SIGNAL(highLightGeometryFaceSig(Geometry::GeometrySet*, int, QList<vtkActor*>*)));
// 		connect(p, SIGNAL(selectGeoActorShape(vtkActor*, int, Geometry::GeometrySet*)), this, SLOT(selectActorShape(vtkActor*, int, Geometry::GeometrySet*)));
	
		connect(this, SIGNAL(setSelectMode(int)), m, SIGNAL(selectModelChangedSig(int)));
		connect(this, SIGNAL(updateGeoTree()), _mainWindow, SIGNAL(updateGeometryTreeSig()));
		connect(this, SIGNAL(showGeometry(Geometry::GeometrySet*)), _preWindow, SIGNAL(showGeoSet(Geometry::GeometrySet*)));
		connect(this, SIGNAL(hideGeometry(Geometry::GeometrySet*)), _preWindow, SIGNAL(removeGemoActors(Geometry::GeometrySet*)));

		connect(_preWindow, SIGNAL(geoShapeSelected(Geometry::GeometrySet*, int)), this, SLOT(shapeSlected(Geometry::GeometrySet*, int)));
		connect(this, SIGNAL(highLightGeometrySetSig(Geometry::GeometrySet*, bool)), _preWindow, SIGNAL(highLightGeometrySet(Geometry::GeometrySet*, bool)));
		connect(this, SIGNAL(highLightGeometryPointSig(Geometry::GeometrySet*, int, bool)), _preWindow, SIGNAL(highLightGeometryPoint(Geometry::GeometrySet*, int, bool)));
		connect(this, SIGNAL(highLightGeometryEdgeSig(Geometry::GeometrySet*, int, bool)), _preWindow, SIGNAL(highLightGeometryEdge(Geometry::GeometrySet*, int, bool)));
		connect(this, SIGNAL(highLightGeometryFaceSig(Geometry::GeometrySet*, int, bool)), _preWindow, SIGNAL(highLightGeometryFace(Geometry::GeometrySet*, int, bool)));
		connect(this, SIGNAL(highLightGeometrySolidSig(Geometry::GeometrySet*, int, bool)), _preWindow, SIGNAL(highLightGeometrySolid(Geometry::GeometrySet*, int, bool)));
		connect(this, SIGNAL(clearGeometryHighLightSig()), _preWindow, SIGNAL(clearGeometryHighLight()));
		_pyAgent = Py::PythonAagent::getInstance();
	}

	GeoDialogBase::~GeoDialogBase()
	{
		if (_restoreSelectMode)
			emit setSelectMode(-1);
		emit clearGeometryHighLightSig();
	}

	void GeoDialogBase::selectActorShape(vtkActor*, int, Geometry::GeometrySet*)
	{

	}

	void GeoDialogBase::shapeSlected(Geometry::GeometrySet* set, int index)
	{

	}

	bool GeoDialogBase::isEdit()
	{
		return _isEdit;
	}

	void GeoDialogBase::translateButtonBox(QDialogButtonBox *b)
	{
		QPushButton* po = b->button(QDialogButtonBox::Ok);
		if (po != nullptr)
			po->setText(tr("OK"));

		QPushButton* pc = b->button(QDialogButtonBox::Cancel);
		if (pc != nullptr)
			pc->setText(tr("Cancel"));
	}


}