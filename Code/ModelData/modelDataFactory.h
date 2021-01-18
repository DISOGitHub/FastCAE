#ifndef _MODELDATAFACTORY_H_
#define _MODELDATAFACTORY_H_

#include "modelDataAPI.h"
#include "DataProperty/modelTreeItemType.h"
#include <QHash>
#include <QPair>
#include <QString>

namespace ModelData
{
	class ModelDataBase;
}

typedef bool(*CREATEMODEL)(int,QPair<int,ModelData::ModelDataBase*>*);

namespace ModelData
{
	class MODELDATAAPI ModelDataFactory
	{
	public:
		ModelDataFactory() = default;
		~ModelDataFactory() = default;

		static void registerType(int type, QString name, CREATEMODEL);
		static void removeType(int type);
		static QHash<int, QString> getRegedType();

		//操作创建
		static ModelDataBase* createModel(QString chartype);
		//文件读取
		static ModelDataBase* createModel(ProjectTreeType type);
	
	private:
		static QHash<int, QString> _typeNameHash;
		static QHash<int, CREATEMODEL> _typeFunHash;

	};
}



#endif