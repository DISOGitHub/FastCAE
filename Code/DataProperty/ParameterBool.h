#ifndef _PARAMEYTERBOOL_H_
#define _PARAMEYTERBOOL_H_

#include "ParameterBase.h"

namespace DataProperty
{
	class DATAPROPERTYAPI ParameterBool : public ParameterBase
	{
	public:
		ParameterBool();
		~ParameterBool() = default;

		void copy(ParameterBase* ori, bool valueOnly = false) override;
		//设置值
		void setValue(bool ok);
		//获取值
		bool getValue();
		bool isSameValueWith(ParameterBase* p) override;
		void writeParameter(QDomDocument* doc, QDomElement* parent) override;
		void readParameter(QDomElement* e) override;
		QString valueToString() override;
		void setValueFromString(QString v) override;

	private:
		bool _value{ false };

	};
}


#endif