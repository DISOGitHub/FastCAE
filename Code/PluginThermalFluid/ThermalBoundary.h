#ifndef THERMALBOUNDARY_H
#define THERMALBOUNDARY_H

#include <QString>
#include "DataProperty/DataBase.h"
#include "ThermalData.h"

enum BoundaryType
{
	BNONE = 0,
	BINTER,
	BSINK,//定温
	QAREA,//热流
	QLENG,//梁
	QTOTA,//总功率
	QVOLU,//热产
	QNULL,//每单元
	IAREA,//热流-变
	ILENG,//梁-变
	ITOTA,//总功率-变
	IVOLU,//热产-变
	INULL,//变温
};

namespace ThermalFluid{
	class ThermalData;
	class BoundaryTabTypeData;

	class ThermalBoundary : public DataProperty::DataBase
	{
	public:
		ThermalBoundary(ThermalData* data);
		~ThermalBoundary();

		void setGroup(QString g);
		QString getGroup();

		void setTypeAndUnit(const QString& type);
		//QString getType();
		BoundaryType getType();
		QString getUnit();

 		void setXValue(double v);
 		double getXValue();

		//void setQ(QString f1, QString f2, double yl, double yf, double yt);
		//double getQ();

		void setTypeAndUnit(ThermalType t1, ThermalType t2);
		void setTypeAndUnit(ThermalType t2);
		//void setQ(ThermalType t1, ThermalType t2, double yl, double yf, double yt);
		
		/*void setQ(ThermalType t1, ThermalType t2);*/
		void setQ(ThermalType t1, ThermalType t2,double v);
		/*void setQ(ThermalType t1, int mul);*/
		void setQ(ThermalType t1, int mul, BoundaryTabTypeData* td = nullptr);
		void appendQ(double* q);
		int getQCount();
		double* getQFromIndex(int index);

		QStringList getThermalType(BoundaryType btype = BNONE);

		void changeQValue(BoundaryType t, double v, BoundaryTabTypeData* td);

		void setTableTypeID(int id);
		int getTableTypeID();

		void clearQList();

		void writeTbData(QDomDocument* doc, QDomElement* tb);
		void readTbData(QDomElement* aTb);
	private:
		QString _group;
		BoundaryType _type{ BINTER };
		QString _unit;
		double _xValue{ 0 };
		QList<double*> _q{};
		int _tabID{ -1 };

		ThermalData* _thermalData;

	};
}
#endif // THERMALBOUNDARY_H

