#ifndef MODELDATASINGLETON_H
#define MODELDATASINGLETON_H

#include "modelDataAPI.h"
#include <QList>
#include "DataProperty/DataBase.h"

class QDomDocument;
class QDomElement;

namespace ModelData
{
	class ModelDataBase;

	class MODELDATAAPI ModelDataSingleton : public DataProperty::DataBase
	{
	public:
		//获取单例
		static ModelDataSingleton* getinstance();
		//添加模型
		void appendModel(ModelDataBase* model);
		//获取第index个模型
		ModelDataBase* getModelAt(const int index);
		//获取模型个数
		int getModelCount();
		//通过ID获取模型
		ModelDataBase* getModelByID(const int id);
		//通过名称获取模型
		ModelDataBase* getModelByName(QString name);
		QString getMD5();
		//清空数据
		void clear();
		//移除ID为id的模型
		void removeModelByID(const int id);
		//获取第index个模型的ID
		int getModelIDByIndex(const int index);
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
	
	
	private:
		ModelDataSingleton();
		~ModelDataSingleton();

	private:
		static ModelDataSingleton* _instance;
		QList<ModelDataBase*> _modelList{};

	};
}


#endif
