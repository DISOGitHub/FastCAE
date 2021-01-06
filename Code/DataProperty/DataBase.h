/****************************
数据的基类
所有的数据都应该继承自该类
****************************/

#ifndef PROPDATABASE_H
#define PROPDATABASE_H

#include "PropertyList.h"
#include "ParameterList.h"
#include "DataPropertyAPI.h"
#include <QString>
#include <QList> 
#include <QStringList>
#include <QObject>

class QDomDocument;
class QDomElement;

struct  ButtonInfo
{
	QString text{};
	QString chinese{};
	QString command{};

};

namespace DataProperty 
{
	class ParameterGroup;

	class DATAPROPERTYAPI DataBase : public QObject, public PropertyList, public ParameterList
	{
		Q_OBJECT

	public:
		DataBase() = default; 
		~DataBase() = default;
		//设置数据所属的模块
		void setModuleType(ModuleType t);
		//获取数据所属的模块
		ModuleType getModuleType();
		//从data中拷贝内容
		 virtual void copy(DataBase* data);
		 //设置数据ID
		virtual void setID(int id);
		//获取数据ID
		int getID();
		//设置数据索引
		void setIndex(int index);
		//获取数据索引
		int getIndex();
		//设置名称
		virtual void setName(const QString& name);
		//获取名称
		QString getName();
		//添加参数组
		void appendParameterGroup(ParameterGroup* g);
		//获取第index个参数组
		ParameterGroup* getParameterGroupAt(const int index);
		//获取参数组个数
		int getParameterGroupCount();
		//获取可见参数组个数
		int getVisiableParameterGroupCount();
		//移除参数组
		virtual void removeParameterGroup(ParameterGroup* g);
		//移除第i个参数组
		void removeParameterGroupAt(int i);

		void appendParameter(ParameterBase* p) override;
		ParameterBase* appendParameter(ParaType type) override;

		virtual void removeParameter(ParameterBase* p) override;

		/*用于产生MD5 */
		virtual void dataToStream(QDataStream* datas) override;
		///数据写入工程文件
		virtual QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent);
		///从工程文件读入数据
		virtual void readDataFromProjectFile(QDomElement* e);
		//读取参数
		virtual void readParameters(QDomElement* ele);
		//写出参数
		virtual void writeParameters(QDomDocument* doc, QDomElement* parent);
		///根据名称获取参数
		ParameterBase* getParameterByName(QString name) override;
		///根据名称获取参数组
		virtual ParameterGroup* getParameterGroupByName(QString name);
		///是否需要button
		bool isContainsButton();
		///获取button英文
		QStringList getButtonText();
		///获取button中文
		QStringList getButtonChinese();
		///获取button链表
		QList<ButtonInfo> getButtonList();
		//填充所有参数信息，包括模块，ID Index等
		virtual void generateParaInfo();

	private slots:
		virtual void onButtonClicked(QString b);

	protected:
		int _id{ -1 };
		int _index{ -1 };
		QString _name{ "FFFFFF" };
		QList<ParameterGroup*> _paraGroupList{};
		QList<ButtonInfo> _buttons{};

		ModuleType _moduleType{ Module_None };
	};

}

#endif
