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
	    //����ֵ
		void setValue(int v);
		//��ȡֵ
		int getValue();
		//���÷�Χ
		void setRange(int* range);
		//��ȡ��Χ
		void getRange(int* range);
		//���õ�λ(����)
		void setUnit(QString unit);
		//��ȡ��λ(����)
		QString getUnit();
		bool isSameValueWith(ParameterBase* p) override;

		void writeParameter(QDomDocument* doc, QDomElement* parent) override;
		void readParameter(QDomElement* e) override;
		QString valueToString() override;
		//ͨ���ַ�������ֵ
		void setValueFromString(QString v);

	private:
		int _value{ 0 };
		int _range[2];
		QString _unit{};
	};
}


#endif
