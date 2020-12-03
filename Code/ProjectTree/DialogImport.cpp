#include "DialogImport.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshSet.h"
#include "ModelData/modelDataBaseExtend.h"
#include <QMap>
#include <QDebug>
#include <QMessageBox>
#include "python/PyAgent.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBase.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include "geometry/GeoComponent.h"

namespace ProjectTree
{
	ImportDialog::ImportDialog(GUI::MainWindow* mainwindow, ModelData::ModelDataBaseExtend* data, ImportType t)
		: ModuleBase::ComponentSelectDialogBase(mainwindow, nullptr, nullptr), _type(t)
	{
		_data = data;
		init();
	}
	void ImportDialog::init()
	{
		/*switch (_type)
		{
		case ProjectTree::None:
			break;
		case ProjectTree::Geometry:
			initGeo();
			break;
		case ProjectTree::MeshSet:
			initMeshSet();
			break;
		default:
			break;
		}*/
		//initGeo();
		initMeshSet();
		initGeoComponent();
	}

	void ImportDialog::accept()
	{
		/*switch (_type)
		{
		case ProjectTree::None:
			break;
		case ProjectTree::Geometry:
			acceptGeo();
			break;
		case ProjectTree::MeshSet:
			acceptMeshSet();
			break;
		default:
			break;
		}*/
		//acceptGeo();
		QMap<int, QString> idname = this->getSelectedItemIDNames();
		QList<int> ids = idname.keys();
		QStringList addIdValues{};
		for(int id : ids)
			addIdValues << QString::number(id);
		int caseID = _data->getID();
		QString code = QString("Case.importComponents(%1,\"%2\")").arg(caseID).arg(addIdValues.join(" "));
		Py::PythonAagent::getInstance()->submit(code);
		ComponentSelectDialogBase::accept();
	}

	void ImportDialog::initGeo()
	{
		Geometry::GeometryData* geoData = Geometry::GeometryData::getInstance();
		const int n = geoData->getGeometrySetCount();
		QList<int> geoIDs = _data->getGeometryList();
		for (int i = 0; i < n; ++i)
		{
			Geometry::GeometrySet* set = geoData->getGeometrySetAt(i);
			QString name = set->getName();
			int id = set->getID();
			QString icon = ":/QUI/icon/geometry.png";
			if (geoIDs.contains(id))
				appendItemToSelectedList(name, id, icon);
			else
				appendItemToAvailableList(name, id, icon);
		}
	}

	void ImportDialog::initMeshSet()
	{
		MeshData::MeshData* meshData = MeshData::MeshData::getInstance();
		const int n = meshData->getMeshSetCount();
		QList<int> compIDs = _data->getMeshSetList();
		for (int i = 0; i < n; ++i)
		{
			MeshData::MeshSet* set = meshData->getMeshSetAt(i);
			QString name = set->getName();
			int id = set->getID();
			QString icon = ":/QUI/icon/node.png";
			if (set->getSetType() == MeshData::Element)
				icon = ":/QUI/icon/mesh.png";
			if (compIDs.contains(id))
				appendItemToSelectedList(name, id, icon);
			else
				appendItemToAvailableList(name, id, icon);
		}
	}

	void ImportDialog::initGeoComponent()
	{
		auto geoData = Geometry::GeometryData::getInstance();
		QList<int> geoIDs = _data->getGeoComponentIDList();
		foreach(auto aGc, geoData->getGeoComponentList())
		{
			QString name = aGc->getName();
			int id = aGc->getID();
			QString icon = ":/QUI/icon/geometry.png";
			if (geoIDs.contains(id))
				appendItemToSelectedList(name, id, icon);
			else
				appendItemToAvailableList(name, id, icon);
		}
	}

	void ImportDialog::acceptGeo()
	{
		QMap<int, QString> idname = this->getSelectedItemIDNames();
		QStringList values = idname.values();
		QList<int> ids = idname.keys();
		QStringList addIdValues;
		for (int i = 0; i < ids.size(); ++i)
		{
			addIdValues << QString::number(ids.at(i));
		}
		int  id = _data->getID();
		QString code = QString("Case.importGeometry(%1,\"%2\")").arg(id).arg(addIdValues.join(" "));//
		qDebug() << code;
		if (ids.size() > 0)
			Py::PythonAagent::getInstance()->submit(code);
		ComponentSelectDialogBase::accept();
	}

// 	void ImportDialog::acceptComponents()
// 	{
// 		QMap<int, QString> idname = this->getSelectedItemIDNames();
// 		QList<int> ids = idname.keys();
// 		QStringList addIdValues{};
// 		foreach(int id, ids)
// 			addIdValues << QString::number(id);
// 		int caseID = _data->getID();
// 		QString code = QString("Case.importGeoComponents(%1,\"%2\")").arg(caseID).arg(addIdValues.join(" "));
// 		Py::PythonAagent::getInstance()->submit(code);
// 	}

// 	void ImportDialog::acceptGeoComponent()
// 	{
// 		QMap<int, QString> idname = this->getSelectedItemIDNames();
// 		QList<int> ids = idname.keys();
// 		QStringList addIdValues{};
// 		foreach(int id, ids)
// 			addIdValues << QString::number(id);
// 		int caseID = _data->getID();
// 		QString code = QString("Case.importGeoComponents(%1,\"%2\")").arg(caseID).arg(addIdValues.join(" "));
// 		Py::PythonAagent::getInstance()->submit(code);
// 	}
} 