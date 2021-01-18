#include "modelDataFactory.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/ProjectTreeConfig.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataBaseExtend.h"

namespace ModelData
{
	QHash<int, QString> ModelDataFactory::_typeNameHash = QHash<int, QString>();
	QHash<int, CREATEMODEL> ModelDataFactory::_typeFunHash = QHash<int, CREATEMODEL>();

	void ModelDataFactory::registerType(int type, QString name, CREATEMODEL fun)
    {
		_typeNameHash[type] = name;
		_typeFunHash[type] = fun;
	}


	void ModelDataFactory::removeType(int type)
	{
		_typeNameHash.remove(type);
		_typeFunHash.remove(type);
	}


	QHash<int, QString> ModelDataFactory::getRegedType()
	{
		return _typeNameHash;
	}

	ModelDataBase* ModelDataFactory::createModel(QString type)
	{
		ConfigOption::ProjectTreeConfig* treeconfig = ConfigOption::ConfigOption::getInstance()->getProjectTreeConfig();
	//	ModelData::ModelDataSingleton* s = ModelData::ModelDataSingleton::getinstance();
		ProjectTreeType t = treeconfig->getTypeByName(type);
		if (t != UnDefined)
		{
			ModelData::ModelDataBase* modelBase = new ModelData::ModelDataBaseExtend(t);
			modelBase->copyFormConfig();
//			modelBase->generateParaInfo()
			return modelBase;
		}
		else
		{
			int ti = _typeNameHash.key(type);
			auto fun = _typeFunHash.value(ti);
			if (fun == nullptr) return nullptr;
			QPair<int, ModelDataBase*> p{};
			bool ok = fun(ti,&p);
			if (!ok) return nullptr;
			p.second->copyFormConfig();
			return p.second;
		}
		
	}

	ModelDataBase* ModelDataFactory::createModel(ProjectTreeType type)
	{
		if (type == UnDefined) return nullptr;

		auto fun = _typeFunHash.value((int)type);
		if (fun != nullptr)
		{
			QPair<int, ModelDataBase*> p{};
			bool ok = fun(int(type), &p);
			if (!ok) return nullptr;
			return p.second;
		}
		else
		{
			auto m = new ModelDataBaseExtend(type);
			return m;
		}
	}


}
