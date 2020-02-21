#ifndef _PARAMETERLIST_H_
#define _PARAMETERLIST_H_

#include <QList>
#include "DataPropertyAPI.h"
#include "ParameterBase.h"
// #include "ParameterInt.h"
// #include "ParameterDouble.h"
// #include "ParameterBool.h"
// #include "ParameterSelectable.h"
// #include "ParameterTable.h"
// #include "ParameterString.h"
// #include "ParameterPath.h"

class QDataStream;
class QDomDocument;
class QDomElement;

namespace DataProperty
{
	class ParameterBase;
	class DataBase;

	class DATAPROPERTYAPI ParameterList
	{
	public:
		ParameterList() = default;
		~ParameterList();
		//从data中拷贝参数列表
		void copy(ParameterList* data);
		//添加参数
		virtual void appendParameter(ParameterBase* para);
		//根据类型创建新的参数，并放入列表
		virtual ParameterBase* appendParameter(ParaType type);
		//获取第i个参数
		ParameterBase* getParameterAt(const int i);
		//获取参数个数
		int getParameterCount();
		//获取可见的参数个数
		int getVisibleParaCount();
		//移除参数
		virtual void removeParameter(ParameterBase* p);
		//移除第i个参数
		void removeParameterAt(int i);
		//获取第i和可见的参数
		ParameterBase* getVisibleParameterAt(const int i);
		//根据类型创建新的参数，不会放入参数列表
		static ParameterBase* createParameterByType(ParaType t);
		//根据类型创建新的参数，不会放入参数列表
		static ParameterBase* createParameterByType(QString stype);
		//根据创建新参数，并拷贝p的内容，不会放入参数列表
		static ParameterBase* copyParameter(ParameterBase* p);

		///写出参数数据
		virtual void writeParameters(QDomDocument* doc, QDomElement* parent);
		//读入数据
		virtual void readParameters(QDomElement* ele);
		//根据名称获取参数
		virtual ParameterBase* getParameterByName(QString name);
		//获取md5
		virtual void dataToStream(QDataStream* datas);
		//获取全部参数列表
		QList<ParameterBase*> getParaList();


	protected:
		QList<ParameterBase*> _paraList{};
	};
}

#endif
