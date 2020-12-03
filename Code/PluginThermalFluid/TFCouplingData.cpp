#include <QDomDocument>
#include <qDebug>
#include <QFile.h>
#include "ThermalData.h"
#include "TFCouplingData.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshSet.h"

namespace ThermalFluid
{
	TFCouplingModelData::TFCouplingModelData(ProjectTreeType t)
		:ModelDataBaseExtend(t)
	{
		_thermalData = new ThermalData{};
	}

	TFCouplingModelData::~TFCouplingModelData()
	{
		if (_thermalData != nullptr)
			delete _thermalData;
		_thermalData = nullptr;
	}

	MeshData::MeshSet* TFCouplingModelData::getMeshSetByShortName(const QString& name)
	{
		MeshData::MeshData* md = MeshData::MeshData::getInstance();
		const int n = md->getMeshSetCount();
		for (int i = 0; i < n; ++i)
		{
			MeshData::MeshSet* set = md->getMeshSetAt(i);
			QString setName = set->getName();
			if (name == setName.split("(").at(0))
				return set;
		}
		return nullptr; 
	}

	void TFCouplingModelData::setThermalData(ThermalData* td)
	{
		_thermalData = td;
	}

	ThermalFluid::ThermalData* TFCouplingModelData::getThermalData()
	{
		return _thermalData;
	}

	QDomElement& TFCouplingModelData::writeToProjectFile(QDomDocument* doc, QDomElement* e)
	{
		QDomElement element = ModelDataBase::writeToProjectFile(doc, e);
		QDomElement tf      = doc->createElement("Thermal");
		QDomElement ff      = doc->createElement("Flow");		
		element.appendChild(tf);
		element.appendChild(ff);
		_thermalData -> writeThermalData(doc, &tf);
        //_flowData  -> writeFluidData(doc,&ff);			
		return *e;
	}

	void TFCouplingModelData::readDataFromProjectFile(QDomElement* e)
	{
 		ModelDataBase::readDataFromProjectFile(e);
		_thermalData -> readThermalData(e);
		//_flowData  -> readFluidData(e);
	}
}

