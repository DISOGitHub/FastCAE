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
		//����ֵ
		void setValue(double v);
		//��ȡֵ
		double getValue();
		//���ñ�������
		void setVariableName(QString s);

		virtual QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		virtual void readDataFromProjectFile(QDomElement* ele) override;

	protected:
		DataProperty::ParameterDouble* _value{};

	};

}

#endif