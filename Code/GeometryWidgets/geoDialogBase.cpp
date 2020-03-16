#include "geoDialogBase.h"
#include "geometry/geometryData.h"
#include "mainWindow/mainWindow.h"
#include "MainWidgets/preWindow.h"
#include "python/PyAgent.h"
#include <QPushButton>
#include <QDialogButtonBox>

namespace GeometryWidget
{
	GeoDialogBase::GeoDialogBase(GUI::MainWindow* m, MainWidget::PreWindow* p, bool keySig) :
		QFDialog(m,keySig), _preWindow(p)
	{
		_geoData = Geometry::GeometryData::getInstance();
		Qt::WindowFlags flags = this->windowFlags();
		flags |= Qt::WindowStaysOnTopHint;
		this->setWindowFlags(flags);

		connect(this, SIGNAL(updateGraphOptions()), m, SIGNAL(updateGraphOptionsSig()));
 		connect(this, SIGNAL(setSelectMode(int)), m, SIGNAL(selectModelChangedSig(int)));
// 		connect(this, SIGNAL(setSelectMode(int)), m, SIGNAL(selectGeoCloseSig(int)));
		connect(this, SIGNAL(highLightGeometrySet(Geometry::GeometrySet*, bool)), m, SIGNAL(highLightGeometrySetSig(Geometry::GeometrySet*, bool)));
		connect(this, SIGNAL(highLightGeometryPoint(Geometry::GeometrySet*, int, QList<vtkActor*>*)), m, SIGNAL(highLightGeometryPointSig(Geometry::GeometrySet*, int, QList<vtkActor*>*)));
		connect(this, SIGNAL(highLightGeometryEdge(Geometry::GeometrySet*, int, QList<vtkActor*>*)), m, SIGNAL(highLightGeometryEdgeSig(Geometry::GeometrySet*, int, QList<vtkActor*>*)));
		connect(this, SIGNAL(highLightGeometryFace(Geometry::GeometrySet*, int, QList<vtkActor*>*)), m, SIGNAL(highLightGeometryFaceSig(Geometry::GeometrySet*, int, QList<vtkActor*>*)));
		connect(p, SIGNAL(selectGeoActorShape(vtkActor*, int, Geometry::GeometrySet*)), this, SLOT(selectActorShape(vtkActor*, int, Geometry::GeometrySet*)));
	
		connect(this, SIGNAL(updateGeoTree()), _mainWindow, SIGNAL(updateGeometryTreeSig()));
		connect(this, SIGNAL(showGeometry(Geometry::GeometrySet*)), _preWindow, SIGNAL(showGeoSet(Geometry::GeometrySet*)));
		connect(this, SIGNAL(hideGeometry(Geometry::GeometrySet*)), _preWindow, SIGNAL(removeGemoActors(Geometry::GeometrySet*)));
		_pyAgent = Py::PythonAagent::getInstance();
	}

	void GeoDialogBase::selectActorShape(vtkActor*, int, Geometry::GeometrySet*)
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