#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "MaterialAPI.h"
#include "DataProperty/DataBase.h"
#include <QString>

class QDomElement;
class QDomDocument;

namespace Material
{
	class MATERIALAPI Material : public DataProperty::DataBase
	{
	public:
		Material(bool IDplus = true);
		~Material() = default;
		//获取最大ID
		static int getMaxID();
		void enableID(bool on);
		//获取类型
		QString getType();
		//设置类型
		void setType(QString type);
		//拷贝数据
		void copy(DataBase* data) override;
		//从工程文件读入
		void readDataFromProjectFile(QDomElement* e) override;
		//写出到工程文件
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		
	private:
		QString _type{};
		bool _needID{true};

	private:
		static int maxID;
	};
}



#endif
