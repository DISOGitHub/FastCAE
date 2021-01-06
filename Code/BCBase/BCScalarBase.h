#ifndef _BCSCALARBASE_H_
#define _BCSCALARBASE_H_

#include "BCBaseAPI.h"
#include "BCBase.h"

namespace DataProperty
{
	class ParameterDouble;
}

namespace BCBase
{
	class BCBASEAPI BCScalarBase : public BCBase
	{
	public:
		BCScalarBase();
		BCScalarBase(BCType type);
		~BCScalarBase() = default;
		//设置值
		void setValue(double v);
		//获取值
		double getValue();
		//设置变量名称
		void setVariableName(QString s);

		virtual QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		virtual void readDataFromProjectFile(QDomElement* ele) override;

	protected:
		DataProperty::ParameterDouble* _value{};

	};

}

#endif