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
		//����������������
		void setVariableName(QString x, QString y, QString z);
		//����ֵ
		void setValue(double* v);
		void setValue(double x, double y, double z);
		//��ȡֵ
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