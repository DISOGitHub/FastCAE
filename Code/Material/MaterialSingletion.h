#ifndef _MATERIALSINGLETON_H_
#define _MATERIALSINGLETON_H_

#include "MaterialAPI.h"
#include <QList>
#include "DataProperty/DataBase.h"
#include <QHash>

class QDomDocument;
class QDomElement;
class QFile;

namespace GUI
{
	class MainWindow;
}

namespace Material
{
	class Material;

	class MATERIALAPI MaterialSingleton :public DataProperty::DataBase
	{
	public:
		//��ȡ����ָ��
		static MaterialSingleton* getInstance();
		//�������
		void clear();
		//��ȡ��������
		int getMaterialCount();
		//��ȡ��i������
		Material* getMaterialAt(const int i);
		//����ID��ȡ����
		Material* getMaterialByID(const int id);
		//��Ӳ���
		void appendMaterial(Material* m);
		//����ID�Ƴ�����
		void removeMaterialByID(const int id);
		//��IDΪid�Ĳ�����������Ͽ�
		void appendToMaterialLib(const int id);
		//�Ӳ��Ͽ��м���
		void loadFromMaterialLib(GUI::MainWindow* m);
		//�Ӳ��Ͽ�ɾ��
		void removeFromMAterialLib(GUI::MainWindow* m);
	
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* e) override;
		void readDataFromProjectFile(QDomElement* ele) override;

	private:
		MaterialSingleton() = default;
		~MaterialSingleton() = default;
		QHash<QString, Material*> loadMaterilLib();
		void writeToMaterialLib(QHash<QString, Material*> ms);

	private:
		static MaterialSingleton* _instance;
		QList<Material*> _materialList{};
		
	};
}


#endif