#include "GeoCommandCreateComponent.h"
#include "geometry/geometryData.h"
#include "mainWindow/mainWindow.h"

namespace Command
{
	CommandCreateComponent::CommandCreateComponent(GUI::MainWindow* m, MainWidget::PreWindow* p):
		GeoCommandBase(m, p)
	{
		connect(this, SIGNAL(clearAllHighLight()), m, SIGNAL(clearAllHighLight()));
	}

	bool Command::CommandCreateComponent::execute()
	{
		_gc = new Geometry::GeoComponent(_name, _type);
		_geoData = Geometry::GeometryData::getInstance();
		/*int index =*/ _geoData->appendGeoComponent(_gc);
		Geometry::GeometrySet* gSet{};

		QMultiHash<Geometry::GeometrySet*, int> items;
		for (int i = 0; i < _geoIds.size(); i++)
		{
			QString gID = _geoIds.at(i);
			gSet = _geoData->getGeometrySetByID(gID.toInt());
			if (!gSet)	continue;
			items.insert(gSet, _itemIds.at(i).toInt());
		}
		if (items.size() == 0)    return false;
		_gc->setSelectedItems(items);
		return true;
	}

	void CommandCreateComponent::undo()
	{
		_geoData->removeGeoComponent(_gc);
		emit updateGeoTree();
		emit clearAllHighLight();
		GeoCommandBase::undo();
	}

	void CommandCreateComponent::redo()
	{
		_geoData->appendGeoComponent(_gc);
		emit updateGeoTree();
	}

	void CommandCreateComponent::releaseResult()
	{
		delete _gc;
		_gc = nullptr;
	}

	void CommandCreateComponent::setName(const QString& name)
	{
		_name = name;
	}

	void CommandCreateComponent::setType(const QString& type)
	{
		_type = Geometry::GeoComponent::stringTogcType(type);
	}

	void CommandCreateComponent::setMembers(const QStringList& geoIds, const QStringList& itemIds)
	{
		_geoIds = geoIds;
		_itemIds = itemIds;
	}
}