#ifndef _EDITABLEPROPERTYBASE_H_
#define _EDITABLEPROPERTYBASE_H_

#include "DataProperty/DataPropertyAPI.h"
#include <QString>
#include <QObject>
#include <QList>

class QDomDocument;
class QDomElement;

namespace ConfigOption
{
	class ParameterObserver;
}

namespace DataProperty
{
	enum  ParaType
	{
		Para_Blank = 0,
		Para_Int,
		Para_Double,
		Para_Color,
		Para_String,
		Para_Bool,
		Para_Selectable,
		Para_Path,
		Para_Table,
	};

	enum ModuleType
	{
		Module_None = 0,
		Module_Model,
		Module_BC,
		Module_Material,
		Module_Mesher,
	};
	
	class DATAPROPERTYAPI ParameterBase : public QObject
	{
		Q_OBJECT

	public:
		ParameterBase(ParaType t);
		ParameterBase() = default;
		~ParameterBase() = default;
		//从ori中拷贝内容
		virtual void copy(ParameterBase* ori, bool valueOnly = false);
		//获取类型
		ParaType getParaType();
		//设置类型
		void setParaType(ParaType type);
		//获取名称
		QString getDescribe();
		//设置名称
		void setDescribe(QString s);
		//设置是否可见
		void setVisible(bool v);
		//获取可见状态
		bool isVisible();
		//设置是否可编辑
		void setEditable(bool e);
		//获取可编辑状态
		bool isEditable();
		//设置中文名称，与setDescribe()对应
		void setChinese(QString chinese);
		//获取中文名称
		QString getChinese();
		//将值转化为字符串
		virtual QString valueToString();
		//从字符串读取设置数据
		virtual void setValueFromString(QString v);
		//设置模块类型
		void setModuleType(ModuleType t);
		//获取模块类型
		ModuleType getModuleType();
		//获取数据ID(DataBase)
		int getDataID();
		//设置数据ID
		void setDataID(int id);
		//获取数据索引
		int getDataIndex();
		//设置数据索引
		void setDataIndex(int index);
		//获取所在的组名称
		QString getGroupName();
		//设置组的名称
		void setGroupName(QString group);
		//参数组名称/参数名称
		QString genAbsoluteName();
		//记录观察者
		void appendObserver(ConfigOption::ParameterObserver* obs);
		//获取观察者列表
		QList<ConfigOption::ParameterObserver*> getObserverList();
		//判断数值是否一样
		virtual bool isSameValueWith(ParameterBase* p);
		//状态拷贝
		virtual void copyStatus(ParameterBase* p);
		///数据写入工程文件
		virtual void writeParameter(QDomDocument* doc, QDomElement* parent);
		///从工程文件读入数据
		virtual void readParameter(QDomElement* e);
		//类型转化为字符串
		static QString ParaTypeToString(ParaType t);
		//字符串转化为参数类型
		static ParaType StringToParaType(QString stype);

	signals:
		//信号，参数值发生变化
		void dataChanged();


	protected:
		ParaType _type{ Para_Blank };
		QString _describe{};
		QString _chinese{};
		bool _editable{ true };
		bool _visible{ true };
		QList<ConfigOption::ParameterObserver*> _observerList{};

		//****与参数隶属的模块相关信息,与DataBase相同********
		ModuleType _moduleType{ Module_None };
		int _dataID{ -1 };
		int _dataIndex{ -1 };
		QString _groupName{};
		//**************************************************
	};

}


#endif
