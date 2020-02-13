#ifndef _BCVECTORBASE_H_
#define _BCVECTORBASE_H_

#include "BCBase.h"
#include "BCBaseAPI.h"

namespace DataProperty
{
	class ParameterDouble;
}

namespace BCBase
{
	class BCBASEAPI BCVectorBase : public BCBase
	{
	public:
		BCVectorBase();
		BCVectorBase(BCType type, double* v);
		BCVectorBase(BCType type, double x, double y, double z);
		~BCVectorBase() = default;
		//设置三个变量名称
		void setVariableName(QString x, QString y, QString z);
		//设置值
		void setValue(double* v);
		void setValue(double x, double y, double z);
		//获取值
		void getValue(double* v);

		virtual QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		virtual void readDataFromProjectFile(QDomElement* ele) override;

	protected:
		DataProperty::ParameterDouble* _valueX{};
		DataProperty::ParameterDouble* _valueY{};
		DataProperty::ParameterDouble* _valueZ{};
	};

}

#endif