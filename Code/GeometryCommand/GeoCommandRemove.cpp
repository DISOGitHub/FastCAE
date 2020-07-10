#include "GeoCommandRemove.h"
#include "geometry/geometryData.h"

namespace Command
{
	GeoCommandRemove::GeoCommandRemove(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoCommandBase(m,p)
	{
	}

	bool GeoCommandRemove::execute()
	{
		auto set = _geoData->getGeometrySetAt(_index);
		if (set == nullptr) return false;
		_res = set;
		_geoData->removeTopGeometrySet(set);

		emit removeDisplayActor(set);
		emit updateGeoTree();
		return true;
	}

	void GeoCommandRemove::undo()
	{
		_geoData->appendGeometrySet(_res);
		emit showSet(_res);
		emit updateGeoTree();
		needRelease = false;
	}

	void GeoCommandRemove::redo()
	{
		_geoData->removeTopGeometrySet(_res);

		emit removeDisplayActor(_res);
		emit updateGeoTree();
		needRelease = true;
	}

	void GeoCommandRemove::releaseResult()
	{
		if (needRelease && _res != nullptr)
			delete _res;
	}

	void GeoCommandRemove::setIndex(int index)
	{
		_index = index;
	}

	GeoCommandRemove::~GeoCommandRemove()
	{
		if (needRelease && _res != nullptr)
			delete _res;
	}

}