#include "GeoCommandRemoveDatum.h"
#include "geometry/geometryData.h"
#include "geometry/geometryDatum.h"
#include <QDebug>

namespace Command
{
	GeoCommandRemoveDatum::GeoCommandRemoveDatum(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoCommandBase(m,p)
	{
	}

	GeoCommandRemoveDatum::~GeoCommandRemoveDatum()
	{
		if (!needRelease && _removeOne != nullptr)
			delete _removeOne;
	}

	bool GeoCommandRemoveDatum::execute()
	{
		QList<Geometry::GeometryDatum*> datumList = _geoData->getGeometryDatum();
		Geometry::GeometryDatum* removeOne = datumList[_index];
		_geoData->removeGeometryDatum(removeOne);
		_removeOne = removeOne;
		emit removeDisplayDatumActor(removeOne);
		emit updateGeoTree();
		return true;
	}

	void GeoCommandRemoveDatum::undo()
	{
		_geoData->appendGeometryDatum(_removeOne);
		showDatum(_removeOne);
		emit updateGeoTree();
		needRelease = false;
	}

	void GeoCommandRemoveDatum::redo()
	{
		_geoData->removeGeometryDatum(_removeOne);
		emit removeDisplayDatumActor(_removeOne);
		emit updateGeoTree();
		needRelease = true;
	}

	void GeoCommandRemoveDatum::releaseResult()
	{
		if (needRelease && _removeOne != nullptr) delete _removeOne;
		_removeOne = nullptr;
	}

	void GeoCommandRemoveDatum::setIndex(int index)
	{
		_index = index;
	}


}