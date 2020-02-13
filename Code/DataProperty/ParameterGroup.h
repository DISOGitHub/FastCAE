#ifndef _PARAMETERGROUP_H_
#define _PARAMETERGROUP_H_

#include "ParameterList.h"
#include "ParameterBase.h"
#include <QDataStream>
#include <QString>

namespace DataProperty
{
	class ParameterBase;
	class DataBase;

	class DATAPROPERTYAPI ParameterGroup : public ParameterList
	{
	public:
		ParameterGroup() = default;
		~ParameterGroup() = default;
		//拷贝data中的内容
		void copy(ParameterGroup* data);

		void appendParameter(ParameterBase* p) override;
		ParameterBase* appendParameter(ParaType type) override;
		//设置参数组的名称
		void setDescribe(QString des);
		//获取参数组名称
		QString getDescribe();
		//设置可见性
		void setVisible(bool v);
		//获取可见状态
		bool isVisible();
		//拷贝g的状态
		void copyStates(ParameterGroup* g);

		void writeParameters(QDomDocument* doc, QDomElement* parent) override;
		void readParameters(QDomElement* ele) override;

		virtual void dataToStream(QDataStream* datas) override;

	private:
		QString _describe{};
		bool _visible{ true };
	};


}


#endif