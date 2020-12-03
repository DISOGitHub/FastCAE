#ifndef BOUNDARYTABTYPEDATA_H
#define BOUNDARYTABTYPEDATA_H

#include <QList>
#include <QString>
#include "DataProperty/DataBase.h"

enum TabType
{
	TNONE = 0,
	TQNODE, 
	TEMP,
	ROTAT,

};

class QDomDocument;
class QDomElement;

namespace ThermalFluid{
	class BoundaryTabTypeData :public DataProperty::DataBase
	{
	public:
		BoundaryTabTypeData();
		~BoundaryTabTypeData();

// 		void setID(int id);
// 		int getID();

		void setType(TabType t);
		TabType getType();

		void appendYX(double y,double x);
		int getYXCount();
		double* getYXValue(int index);

		TabType stringToTabType(QString s);
		QString tabTypeToString(TabType t);

		void clearYXList();

		void writeBoundaryTabTypeData(QDomDocument* , QDomElement* );
		void readBoundaryTabTypeData(QDomElement* );
	private:
		//int _id{ -1 };
		TabType _type{ TNONE };
		QList<double*> _yx;
		
	};
}
#endif
