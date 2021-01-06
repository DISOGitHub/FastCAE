#include "GeoCommandSketchBase.h"
#include "MainWidgets/sketchViewProvider.h"

namespace Command
{

	GeocommandSketchBase::GeocommandSketchBase(MainWidget::SketchViewProvider* p, MainWidget::PreWindow* pre, GUI::MainWindow* m)
		: _sketchView(p), GeoCommandBase(m, pre)
	{
		_type = Sketch;

		connect(this, SIGNAL(showSketch(TopoDS_Shape*)), p, SIGNAL(showSketch(TopoDS_Shape*)));
		connect(this, SIGNAL(removeSketch(TopoDS_Shape*)), p, SIGNAL(hideSketch(TopoDS_Shape*)));
	}

	GeocommandSketchBase::~GeocommandSketchBase()
	{

	}

	void GeocommandSketchBase::setPoints(QList<gp_Pnt> ptlist)
	{
		_pointList = ptlist;
	}

	void GeocommandSketchBase::releaseResult()
	{
		if (_resShape != nullptr) delete _resShape;
		_resShape = nullptr;
	}



}