#ifndef _PARAMETERDOUBLE_H_
#define _PARAMETERDOUBLE_H_

#include "ParameterBase.h"


namespace DataProperty
{
	class DATAPROPERTYAPI ParameterDouble : public ParameterBase
	{
	public:
		ParameterDouble();
		~ParameterDouble() = default;

		void copy(ParameterBase* ori, bool valueOnly = false) override;
		//����ֵ
		void setValue(double v);
		//��ȡֵ
		double getValue();
		//����ֵ����Ч��Χ
		void setRange(double* range);
		//��ȡֵ����Ч��Χ
		void getRange(double* range);
		//���þ��ȣ�С�����λ��
		void setAccuracy(int a);
		//��ȡ����
		int getAccuracy();
		//���õ�λ�����٣�
		void setUnit(QString u);
		//��ȡ��λ�����٣�
		QString getUnit();
		bool isSameValueWith(ParameterBase* p) override;

		QString valueToString() override;
		void writeParameter(QDomDocument* doc, QDomElement* parent) override;
		void readParameter(QDomElement* e) override;
		void setValueFromString(QString v) override;
	
	private:
		double _value{ 0.0 };
		double _range[2];
		int _accuracy{ 4 };
		QString _unit{};

	};
}


#endif
