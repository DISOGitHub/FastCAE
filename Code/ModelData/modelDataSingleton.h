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
		//��ȡ����
		static ModelDataSingleton* getinstance();
		//���ģ��
		void appendModel(ModelDataBase* model);
		//��ȡ��index��ģ��
		ModelDataBase* getModelAt(const int index);
		//��ȡģ�͸���
		int getModelCount();
		//ͨ��ID��ȡģ��
		ModelDataBase* getModelByID(const int id);
		//ͨ�����ƻ�ȡģ��
		ModelDataBase* getModelByName(QString name);
		QString getMD5();
		//�������
		void clear();
		//�Ƴ�IDΪid��ģ��
		void removeModelByID(const int id);
		//��ȡ��index��ģ�͵�ID
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
