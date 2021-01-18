#include "simulationSettingBase.h"
#include "modelDataBase.h"
#include <QDomElement>
#include <QDomDocument>

namespace ModelData
{
	SimlutationSettingBase::SimlutationSettingBase(ModelDataBase* m)
		:_modelData(m)
	{
		this->setModuleType(DataProperty::Module_Model);
		this->setID(m->getID());
	}

	QDomElement& SimlutationSettingBase::writeToProjectFile(QDomDocument* doc, QDomElement* e)
	{
		DataProperty::DataBase::writeToProjectFile(doc, e);

		return *e;
	}
	void SimlutationSettingBase::readDataFromProjectFile(QDomElement* e)
	{
		DataProperty::DataBase::readDataFromProjectFile(e);
	}

	void SimlutationSettingBase::dataToStream(QDataStream* datas)
	{
		DataProperty::DataBase::dataToStream(datas);
	}


}