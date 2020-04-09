#include "GeoCommandBase.h"
#include "geometry/geometryData.h"
#include "mainWindow/mainWindow.h"
#include "MainWidgets/preWindow.h"
#include "GeometryCommand/GeoCommandList.h"

namespace Command
{


	GeoCommandBase::GeoCommandBase(GUI::MainWindow* m, MainWidget::PreWindow* p):
		_mainWindow(m), _preWindow(p)
	{
		_geoData = Geometry::GeometryData::getInstance();

		if (_mainWindow != nullptr)
		{
			connect(this, SIGNAL(updateGeoTree()), _mainWindow, SIGNAL(updateGeometryTreeSig()));
			connect(this, SIGNAL(updateActions()), _mainWindow, SIGNAL(updateActionStatesSig()));
		}
		if (_preWindow != nullptr)
		{
			connect(this, SIGNAL(showSet(Geometry::GeometrySet*,bool)), _preWindow, SIGNAL(showGeoSet(Geometry::GeometrySet*,bool)));
			connect(this, SIGNAL(showDatum(Geometry::GeometryDatum*)), _preWindow, SIGNAL(showDatum(Geometry::GeometryDatum*)));
			connect(this, SIGNAL(removeDisplayActor(Geometry::GeometrySet*)), _preWindow, SIGNAL(removeGemoActors(Geometry::GeometrySet*)));
			connect(this, SIGNAL(removeDisplayDatumActor(Geometry::GeometryDatum*)), _preWindow, SIGNAL(removeGeoDatumActors(Geometry::GeometryDatum*)));
		}
		
		_commandList = GeoComandList::getInstance();
	
	}

	void GeoCommandBase::undo()
	{

	}

	void GeoCommandBase::redo()
	{

	}

	bool GeoCommandBase::execute()
	{
		_commandList->clearRedoCommands();
		_commandList->clearSketchRedoCommands();
		emit updateActions();
		return true;
	}

	void GeoCommandBase::releaseResult()
	{

	}

	Command::CommandType GeoCommandBase::getComamndType()
	{
		return _type;
	}

	TopoDS_Shape* GeoCommandBase::getResultShape()
	{
		return _resShape;
	}

	void GeoCommandBase::setEditData(Geometry::GeometrySet* set)
	{
		_isEdit = set != nullptr;
		_editSet = set;
	}

}