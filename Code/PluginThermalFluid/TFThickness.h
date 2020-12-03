#ifndef TFTHICKNESS_H
#define TFTHICKNESS_H

#include <QString>
#include "DataProperty/DataBase.h"

namespace ThermalFluid{
	class TFThickness : public DataProperty::DataBase
	{
	public:
		TFThickness();
		~TFThickness();

// 		void setId(int id);
// 		int getId();
// 
// 		void setName(QString name);
// 		QString getName();

		void setValue(double v);
		double getValue();

		void wirteThickData(QDomDocument* doc, QDomElement* thick);
		void readThickData(QDomElement* aThick);
	private:
// 		int _id;
// 		QString _name;
		double _value{ 0 };
	};
}
#endif // _DEBUG
