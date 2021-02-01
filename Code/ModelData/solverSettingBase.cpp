#include "solverSettingBase.h"
#include "modelDataBase.h"
#include <QDomElement>
#include <QDomDocument>

namespace ModelData
{
	SolverSettingBase::SolverSettingBase(ModelDataBase* m)
		: _modelData(m)
	{
		this->setModuleType(DataProperty::Module_Model);
		this->setID(m->getID());
	}
	QDomElement& SolverSettingBase::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		DataProperty::DataBase::writeToProjectFile(doc, parent);
		return *parent;
	}

}