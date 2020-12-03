#include "GeoComponent.h"
#include "geometrySet.h"
#include "geometryData.h"
#include <qdom.h>
#include <QDebug>

namespace Geometry
{
	GeoComponent::GeoComponent(QString name, GeoComponentType type) : ComponentBase(DataProperty::ComponentType::GEOMETRY)
	{
		setName(name);
		setType(type);
	}

	GeoComponent::~GeoComponent()
	{

	}

	void GeoComponent::setType(GeoComponentType type)
	{
		QString qType{};
		switch (type)
		{
			case Node: qType = tr("Point"); break;
			case Line: qType = tr("Line"); break;
			case Surface: qType = tr("Surface"); break;
			case Body: qType = tr("Body"); break;
			default: break;
		}
		_type = type;
		appendProperty("Type", qType);
	}

	void GeoComponent::appendSelectedItem(GeometrySet* geoSet, int id)
	{
		if (!geoSet)	return;
		if (!_selectedItems.contains(geoSet, id))    return;
		_selectedItems.insert(geoSet, id);
		appendProperty(QObject::tr("Count"), _selectedItems.count());
	}

	void GeoComponent::appendSelectedItem(QMultiHash<GeometrySet*, int>& items)
	{
		QMutableHashIterator<GeometrySet*, int> iterator(items);
		while (iterator.hasNext())
		{
			iterator.next();
			GeometrySet* geoSet = iterator.key();
			int id = iterator.value();
			if (_selectedItems.contains(geoSet, id))	continue;
			_selectedItems.insert(geoSet, id);
		}
		appendProperty(QObject::tr("Count"), _selectedItems.count());
	}

	void GeoComponent::appendSelectedItem(GeometrySet* geoSet, QList<int>& itemIDs)
	{
		for (int itemID : itemIDs)
		{
			if (_selectedItems.contains(geoSet, itemID))	continue;
			_selectedItems.insert(geoSet, itemID);
		}
		appendProperty(QObject::tr("Count"), _selectedItems.count());
	}

	void GeoComponent::setSelectedItems(QMultiHash<GeometrySet*, int>& items)
	{
		_selectedItems = items;
		appendProperty(QObject::tr("Count"), items.count());
	}

	QMultiHash<GeometrySet*, int>& GeoComponent::getSelectedItems()
	{
		return _selectedItems;
	}

	GeoComponentType GeoComponent::getGCType()
	{
		return _type;
	}

	GeoComponentType GeoComponent::stringTogcType(QString qtype)
	{
		GeoComponentType type;
		if (qtype == "Node")
			type = Node;
		else if (qtype == "Line")
			type = Line;
		else if (qtype == "Surface")
			type = Surface;
		if (qtype == "Body")
			type = Body;
		return type;
	}

	QString GeoComponent::gcTypeToString(GeoComponentType type)
	{
		QString qtype{};
		switch (type)
		{
		case Node: qtype = "Node"; break;
		case Line: qtype = "Line"; break;
		case Surface: qtype = "Surface"; break;
		case Body: qtype = "Body"; break;
		default: break;
		}
		return qtype;
	}

	QDomElement& GeoComponent::writeToProjectFile(QDomDocument* doc, QDomElement* root)
	{
		QDomElement geoComponent = doc->createElement("GeoComponent");
		geoComponent.setAttribute("gcID", getID());
		geoComponent.setAttribute("gcName", getName());
		geoComponent.setAttribute("gcType", gcTypeToString(_type));
		QList<GeometrySet*> geoSets = _selectedItems.uniqueKeys();
		for (GeometrySet* geoSet : geoSets)
		{
			QList<int> itemIDs = _selectedItems.values(geoSet);
			QString qItemIDs{};
			for (int itemID : itemIDs)
				qItemIDs.append(QString::number(itemID)).append(',');
			qItemIDs = qItemIDs.left(qItemIDs.size() - 1);

			QDomElement geometrySet = doc->createElement("GeometrySet");
			geometrySet.setAttribute("GeometrySetID", geoSet->getID());
			geometrySet.setAttribute("GeometrySetName", geoSet->getName());
			geometrySet.setAttribute("GeometrySetType", geoSet->getType());
			geometrySet.setAttribute("GeometrySetItemIDs", qItemIDs);
			geoComponent.appendChild(geometrySet);
		}
		root->appendChild(geoComponent);
		return geoComponent;
	}

	void GeoComponent::readDataFromProjectFile(QDomElement* root)
	{
		setID(root->attribute("gcID").toInt());
		GeometryData* geoData = GeometryData::getInstance();
		QDomNodeList geoSetsEle = root->elementsByTagName("GeometrySet");
		for (int i = 0; i < geoSetsEle.size(); ++i)
		{
			QDomElement geoSetEle = geoSetsEle.at(i).toElement();
			int geoSetID = geoSetEle.attribute("GeometrySetID").toInt();
			GeometrySet* geoSet = geoData->getGeometrySetByID(geoSetID);
			if (!geoSet)	continue;
			QStringList qItemIDs = geoSetEle.attribute("GeometrySetItemIDs").split(',');
			QList<int> itemIDs;
			for (QString itemID : qItemIDs)
				itemIDs.append(itemID.toInt());
			appendSelectedItem(geoSet, itemIDs);
		}
		appendProperty("Count", geoSetsEle.size());
	}
}
