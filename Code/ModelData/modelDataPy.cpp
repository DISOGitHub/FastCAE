#include "modelDataPy.h"
#include "modelDataSingleton.h"
#include "modelDataBase.h"
#include "modelDataBaseExtend.h"
#include "BCBase/BCBase.h"
#include "ParaClassFactory/BCFactory.h"
#include "python/PyAgent.h"
#include <QDebug>
#include "ProjectTree/DialogAddBC.h"


namespace ModelData
{	
	void ModelDataPy::importComponents(int caseId, const char* addcomponentsId)
	{
		QString scomponentIds(addcomponentsId);
		QStringList scomponentIdsList = scomponentIds.simplified().split(" ");
		QList<int> ids;
		for(QString id : scomponentIdsList)
			ids.append(id.toInt());
		ModelDataBase* model = ModelDataSingleton::getinstance()->getModelByID(caseId);
		if (model == nullptr) return;
		model->setComponentIDList(ids);
		Py::PythonAagent::getInstance()->unLock();
	}

	void ModelDataPy::importGeometry(int caseId, char* addcomponentsId)
	{
		QString scomponentIds(addcomponentsId);
		QStringList scomponentIdsList = scomponentIds.simplified().split(" ");
		QList<int> ids;
		for (int i = 0; i < scomponentIdsList.size(); ++i)
		{
//			qDebug() << scomponentIdsList[i];
			ids.append(scomponentIdsList[i].toInt());
		}
		ModelDataBase* model = ModelDataSingleton::getinstance()->getModelByID(caseId);
		if (model == nullptr) return;

		model->setGeometryList(ids);
		Py::PythonAagent::getInstance()->unLock();
	}


	void ModelDataPy::addBC(int caseId, int id, char* bctypetostring)
	{
		
		QString bctyst(bctypetostring);
		BCBase::BCType bctype = BCBase::StringToBCType(bctyst);
//		bctyst = BCBase::BCTypeToString(bctype);
		if (bctype == BCBase::None) bctype = BCBase::UserDef;
		ModelDataBase* model = ModelDataSingleton::getinstance()->getModelByID(caseId);
		if (model == nullptr) return;
		ModelDataBaseExtend *_data = dynamic_cast<ModelDataBaseExtend*>(model);
		BCBase::BCBase* bc = ParaClassFactory::BCFactory::createBCByType(bctype, bctyst, _data->getTreeType());
		if (bc == nullptr) return;
//		bc->setBCType(bctype);
		bc->bingdingComponentID(id);
		_data->appeendBC(bc);
		bc->generateParaInfo();
		Py::PythonAagent::getInstance()->unLock();
	}

	void ModelDataPy::setValue(int caseID, char* variable, char* type, char* value)
	{
		QString svariable(variable);
		QString sType(type);
		QString sValue(value);
		ModelDataBase* model = ModelDataSingleton::getinstance()->getModelByID(caseID);
		if (model == nullptr) return;
		DataProperty::ParameterBase* p = model->getParameterByName(svariable);
		if (p == nullptr) return;
		DataProperty::ParaType t = DataProperty::ParameterBase::StringToParaType(sType);
		if (t != p->getParaType()) return;
		p->setValueFromString(sValue);
		Py::PythonAagent::getInstance()->unLock();
	}

	void ModelDataPy::setBCValue(int caseID, int index, char* variable, char* type, char* value)
	{
		QString svariable(variable);
		QString sType(type);
		QString sValue(value);
		ModelDataBase* model = ModelDataSingleton::getinstance()->getModelByID(caseID);
		if (model == nullptr) return;
		BCBase::BCBase* bc = model->getBCAt(index);
		DataProperty::ParameterBase* p = bc->getParameterByName(svariable);
		if (p == nullptr) return;
		DataProperty::ParaType t = DataProperty::ParameterBase::StringToParaType(sType);
		if (t != p->getParaType()) return;
		p->setValueFromString(sValue);
		Py::PythonAagent::getInstance()->unLock();
	}

	void ModelDataPy::setMaterial(int caseID, char* componentIDs, int materialID)
	{
		ModelData::ModelDataSingleton *modelData = ModelData::ModelDataSingleton::getinstance();
		ModelData::ModelDataBaseExtend* model = dynamic_cast<ModelData::ModelDataBaseExtend*>(modelData->getModelByID(caseID));
		if (!model)    return;
		QString cpIDs(componentIDs);
		QStringList cpIDList = cpIDs.split(';');
		for (QString cpID : cpIDList)
			model->setMaterial(cpID.toInt(), materialID);
		Py::PythonAagent::getInstance()->unLock();
	}
}

void MODELDATAAPI importComponents(int caseId, const char* addcomponentsId)
{
	ModelData::ModelDataPy::importComponents(caseId, addcomponentsId);
}

void MODELDATAAPI addBC(int caseId, int id, char* bctypetostring)
{
	ModelData::ModelDataPy::addBC(caseId, id, bctypetostring);
}

void MODELDATAAPI importGeometry(int caseId, char* addcomponentsId)
{
	ModelData::ModelDataPy::importGeometry(caseId, addcomponentsId);
}
void MODELDATAAPI setValue(int caseID, char* variable, char* stype, char* svalue)
{
	ModelData::ModelDataPy::setValue(caseID, variable, stype, svalue);
}
void MODELDATAAPI setBCValue(int caseID, int index ,char* variable, char* stype, char* svalue)
{
	ModelData::ModelDataPy::setBCValue(caseID, index,variable, stype, svalue);
}

void MODELDATAAPI setMaterial(int caseID, char* componentIDs, int materialID)
{
	ModelData::ModelDataPy::setMaterial(caseID, componentIDs, materialID);
}
