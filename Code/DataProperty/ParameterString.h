#ifndef _PARAMETERSTRING_H_
#define _PARAMETERSTRING_H_

#include "ParameterBase.h"
#include <QString>

namespace DataProperty
{

	class DATAPROPERTYAPI ParameterString : public ParameterBase
	{
	public:
		ParameterString();
		~ParameterString() = default;

		void copy(ParameterBase* ori, bool valueOnly = false) override;
		//获取值
		QString getValue();
		//设置值
		void setValue(QString s);
		bool isSameValueWith(ParameterBase* p) override;

		void writeParameter(QDomDocument* doc, QDomElement* parent);
		void readParameter(QDomElement* e) override;

		QString valueToString() override;
		void setValueFromString(QString v) override;

	private:
		QString _value{};
	};

}


#endif