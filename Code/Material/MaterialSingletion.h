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
		//获取单例指针
		static MaterialSingleton* getInstance();
		//清空数据
		void clear();
		//获取材料数量
		int getMaterialCount();
		//获取第i个材料
		Material* getMaterialAt(const int i);
		//根据ID获取材料
		Material* getMaterialByID(const int id);
		//添加材料
		void appendMaterial(Material* m);
		//根据ID移除材料
		void removeMaterialByID(const int id);
		//将ID为id的材料添加至材料库
		void appendToMaterialLib(const int id);
		//从材料库中加载
		void loadFromMaterialLib(GUI::MainWindow* m);
		//从材料库删除
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