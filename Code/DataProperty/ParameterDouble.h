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
		//设置值
		void setValue(double v);
		//获取值
		double getValue();
		//设置值的有效范围
		void setRange(double* range);
		//获取值的有效范围
		void getRange(double* range);
		//设置精度，小数点后位数
		void setAccuracy(int a);
		//获取精度
		int getAccuracy();
		//设置单位（量纲）
		void setUnit(QString u);
		//获取单位（量纲）
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
