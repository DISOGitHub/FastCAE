#ifndef _PARAMETERINT_H_
#define _PARAMETERINT_H_

#include "ParameterBase.h"

namespace DataProperty
{
	class DATAPROPERTYAPI ParameterInt : public ParameterBase
	{
	public:
		ParameterInt();
		~ParameterInt() = default;

		void copy(ParameterBase* ori, bool valueOnly = false) override;
	    //设置值
		void setValue(int v);
		//获取值
		int getValue();
		//设置范围
		void setRange(int* range);
		//获取范围
		void getRange(int* range);
		//设置单位(量纲)
		void setUnit(QString unit);
		//获取单位(量纲)
		QString getUnit();
		bool isSameValueWith(ParameterBase* p) override;

		void writeParameter(QDomDocument* doc, QDomElement* parent) override;
		void readParameter(QDomElement* e) override;
		QString valueToString() override;
		//通过字符串设置值
		void setValueFromString(QString v);

	private:
		int _value{ 0 };
		int _range[2];
		QString _unit{};
	};
}


#endif
