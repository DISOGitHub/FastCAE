#include "ThermalBoundary.h"
#include "ThermalData.h"
#include "BoundaryTabTypeData.h"
#include "DataProperty/PropertyString.h"
#include "DataProperty/PropertyDouble.h"
#include <QDomDocument>
#include <qDebug>
//#pragma execution_character_set("utf-8")

namespace ThermalFluid{
	ThermalBoundary::ThermalBoundary(ThermalData* data) :_thermalData(data)
	{
	}


	ThermalBoundary::~ThermalBoundary()
	{
		clearQList();
	}

	void ThermalBoundary::setGroup(QString g)
	{
		_group = g;
	}

	QString ThermalBoundary::getGroup()
	{
		return _group;
	}

// 	void ThermalBoundary::setTypeAndUnit(QString f1, QString f2)
// 	{
// 		if (f1 == "SINK")
// 		{
// 			_type = "定温";
// 			_unit = "C";
// 		}
// 		else if (f1 == "QNODE")
// 		{
// 			if (f2 == "AREA")
// 			{
// 				_type = "热流";
// 				_unit = "W/m^2";
// 			}
// 			else if (f2 == "LENG")
// 			{
// 				_type = "梁";
// 				_unit = "W/m";
// 			}
// 			else if (f2 == "TOTA")
// 			{
// 				_type = "总功率";
// 				_unit = "W";
// 			}
// 			else if (f2 == "VOLU")
// 			{
// 				_type = "热产";
// 				_unit = "W/m^3";
// 			}
// 			else
// 			{
// 				_type = "每单元";
// 				_unit = "W";
// 			}
// 		}
// 		else if (f1 == "INTERP")
// 		{
// 			if (f2 == "AREA")
// 			{
// 				_type = "热流-变";
// 				_unit = "W/m^2";
// 			}
// 			else if (f2 == "LENG")
// 			{
// 				_type = "梁-变";
// 				_unit = "倍 W/m";
// 			}
// 			else if (f2 == "TOTA")
// 			{
// 				_type = "总功率-变";
// 				_unit = "W";
// 			}
// 			else if (f2 == "VOLU")
// 			{
// 				_type = "热产-变";
// 				_unit = "倍 W/m^3";
// 			}
// 			else
// 			{
// 				_type = "每单元-变";
// 				_unit = "W";
// 			}
// 		}
// 
// 	}

	void ThermalBoundary::setTypeAndUnit(ThermalType t1, ThermalType t2)
	{
		if (t1 == ThermalType::SINK)
		{
			_type = BoundaryType::BSINK;//"定温"
			appendProperty(QObject::tr("tbType"), QObject::tr("BSINK"));
			_unit = "C";
		}
		else if (t1 == ThermalType::QNODE)
		{
			if (t2 == ThermalType::AREA)
			{
				_type = BoundaryType::QAREA;//"热流"
				appendProperty(QObject::tr("tbType"), QObject::tr("QAREA"));
				_unit = "W/m^2";
			}
			else if (t2 == ThermalType::LENG)
			{
				_type = BoundaryType::QLENG;//"梁"
				appendProperty(QObject::tr("tbType"), QObject::tr("QLENG"));
				_unit = "W/m";
			}
			else if (t2 == ThermalType::TOTA)
			{
				_type = BoundaryType::QTOTA;//"总功率"
				appendProperty(QObject::tr("tbType"), QObject::tr("QTOTA"));
				_unit = "W";
			}
			else if (t2 == ThermalType::VOLU)
			{
				_type = BoundaryType::QVOLU;//"热产"
				appendProperty(QObject::tr("tbType"), QObject::tr("QVOLU"));
				_unit = "W/m^3";
			}
			else
			{
				_type = BoundaryType::QNULL;//"每单元"
				appendProperty(QObject::tr("tbType"), QObject::tr("QNULL"));
				_unit = "W";
			}
		}
		appendProperty(QObject::tr("Unit"), _unit);
	}

	void ThermalBoundary::setTypeAndUnit(ThermalType t2)
	{
		BoundaryTabTypeData* td = _thermalData->getTabTypeData(_tabID);

		if (td == nullptr)
			return;

		TabType tab = td->getType();

		if (tab == TabType::TQNODE)
		{
			switch (t2)
			{
			case ThermalType::AREA:
				_type = BoundaryType::IAREA;//"热流-变"
				_unit = "W/m^2";
				appendProperty(QObject::tr("tbType"), QObject::tr("IAREA"));
				break;
			case ThermalType::LENG:
				_type = BoundaryType::ILENG;//"梁-变"
				_unit = QObject::tr("times W/m");//"倍 W/m"
				appendProperty(QObject::tr("tbType"), QObject::tr("ILENG"));
				break;
			case ThermalType::TOTA:
				_type = BoundaryType::ITOTA;//"总功率-变"
				_unit = "W";
				appendProperty(QObject::tr("tbType"), QObject::tr("ITOTA"));
				break;
			case ThermalType::VOLU:
				_type = BoundaryType::IVOLU;//"热产-变"
				_unit = QObject::tr("times W/m^3");//"倍 W/m^3"
				appendProperty(QObject::tr("tbType"), QObject::tr("IVOLU"));
				break;
			default:
				break;
			}
		}
		else if (tab == TabType::TEMP)
		{
			_type = BoundaryType::INULL; //变温
			appendProperty(QObject::tr("tbType"), QObject::tr("INULL"));
			_unit = "C";
		}
		appendProperty(QObject::tr("Unit"), _unit);
	}

// 	void ThermalBoundary::setQ(ThermalType t1, ThermalType t2)
// 	{
// 		clearQList();
// 
// 		double xt = _thermalData->getXTemper();
// 		double yl = _thermalData->getYLength();
// 		double yf = _thermalData->getYFroce();
// 
// 		double* q =new double ;
// 		if (t1 == ThermalType::SINK)	
// 			*q = _xValue / xt;
// 		else if (t1 == ThermalType::QNODE)
// 		{
// 			if (t2 == ThermalType::AREA)
// 				*q = _xValue * yl / yf;			
// 			else if (t2 == ThermalType::LENG)			
// 				*q = _xValue / yf;			
// 			else if (t2 == ThermalType::TOTA){
// 				*q = _xValue / yl / yf;
// 			}
// 			else if (t2 == ThermalType::VOLU)			
// 				*q = _xValue * yl * yl / yf;			
// 			else
// 				*q = _xValue / yl / yf;			
// 		}
// 
// 		DataProperty::PropertyDouble* pDouble = dynamic_cast<DataProperty::PropertyDouble*>(getPropertyByName(QObject::tr("QValue")));
// 		if (!pDouble)
// 			appendProperty(QObject::tr("QValue"), *q);
// 		else
// 			pDouble->setValue(*q);
// 		appendQ(q);
// 	}

// 	void ThermalBoundary::setQ(ThermalType t1, int mul)
// 	{
// 		clearQList();
// 
// 		double yl = _thermalData->getYLength();
// 		double yf = _thermalData->getYFroce();
// 		double xt = _thermalData->getXTemper();
// 
// 		BoundaryTabTypeData* td = _thermalData->getTabTypeData(_tabID);
// 
// 		if (td == nullptr)
// 			return;
// 
// 		TabType tab = td->getType();
// 		int n = td->getYXCount();
// 
// 		QString dbstr;
// 		if (tab == TabType::TQNODE)
// 		{
// 			switch (t1)
// 			{
// 			case ThermalType::AREA:
// 				for (int i = 0; i < n; i++)
// 				{
// 					double* yx = td->getYXValue(i);
// 					double* v = new double[2];
// 
// 					v[0] = yx[0] * mul * yl / yf;
// 					v[1] = yx[1] * mul * yl / yf;					
// 					dbstr.append(QString::number(v[0])).append(" , ").append(QString::number(v[1])).append(" , ");
// 					appendQ(v);
// 				}
// 				break;
// 			case ThermalType::LENG:
// 				for (int i = 0; i < n; i++)
// 				{
// 					double* yx = td->getYXValue(i);
// 					double* v =new double[2];
// 
// 					v[0] = yx[0] * mul / yf;
// 					v[1] = yx[1] * mul / yf;
// 					dbstr.append(QString::number(v[0])).append(" , ").append(QString::number(v[1])).append(" , ");					
// 					appendQ(v);
// 				}
// 				break;
// 			case ThermalType::TOTA:
// 				for (int i = 0; i < n; i++)
// 				{
// 					double* yx = td->getYXValue(i);
// 					double* v = new double[2];
// 
// 					v[0] = yx[0] * mul / yl / yf;
// 					v[1] = yx[1] * mul / yl / yf;
// 					dbstr.append(QString::number(v[0])).append(" , ").append(QString::number(v[1])).append(" , ");					
// 					appendQ(v);
// 				}
// 				break;
// 			case ThermalType::VOLU:
// 				for (int i = 0; i < n; i++)
// 				{
// 					double* yx = td->getYXValue(i);
// 					double* v = new double[2];
// 
// 					v[0] = yx[0] * mul * yl * yl / yf;
// 					v[1] = yx[1] * mul * yl * yl / yf;					
// 					dbstr.append(QString::number(v[0])).append(" , ").append(QString::number(v[1])).append(" , ");
// 					appendQ(v);
// 				}
// 				break;
// 			default:
// 				break;
// 			}
// 		}
// 		else if (tab == TabType::TEMP)
// 		{
// 			for (int i = 0; i < n; i++)
// 			{
// 				double* yx = td->getYXValue(i);
// 				double* v = new double[2];
// 
// 				v[0] = yx[0] * mul * yl / xt;
// 				v[1] = yx[1] * mul * yl / xt;
// 				dbstr.append(QString::number(v[0])).append(" , ").append(QString::number(v[1])).append(" , ");				
// 				appendQ(v);
// 			}
// 		}
// 		QString dbstr1 = dbstr.left(dbstr.size() - 2);		
// 		DataProperty::PropertyString* pStr = dynamic_cast<DataProperty::PropertyString*>(getPropertyByName(QObject::tr("QValue")));
// 		if (!pStr)
// 			appendProperty(QObject::tr("QValue"), dbstr1);
// 		else
// 			pStr->setValue(dbstr1);
// 	}

	void ThermalBoundary::setQ(ThermalType t1, ThermalType t2, double v)
	{
		clearQList();

		double xt = _thermalData->getXTemper();
		double yl = _thermalData->getYLength();
		double yf = _thermalData->getYFroce();

		double* q = new double;
		if (t1 == ThermalType::SINK)
			*q = v / xt;
		else if (t1 == ThermalType::QNODE)
		{
			if (t2 == ThermalType::AREA)
				*q = v * yl / yf;
			else if (t2 == ThermalType::LENG)
				*q = v / yf;
			else if (t2 == ThermalType::TOTA){
				*q = v / yl / yf;
			}
			else if (t2 == ThermalType::VOLU)
				*q = v * yl * yl / yf;
			else
				*q = v / yl / yf;
		}
		
		DataProperty::PropertyDouble* pDouble = dynamic_cast<DataProperty::PropertyDouble*>(getPropertyByName(QObject::tr("dQValue")));
		if (!pDouble)
			appendProperty(QObject::tr("dQValue"), *q);
		else
			pDouble->setValue(*q);
		removeProperty(QObject::tr("sQValue"));
		appendQ(q);
	}

	void ThermalBoundary::setQ(ThermalType t1, int mul, BoundaryTabTypeData* td)
	{
		clearQList();

		double yl = _thermalData->getYLength();
		double yf = _thermalData->getYFroce();
		double xt = _thermalData->getXTemper();

		if (td == nullptr)
		{
			td = _thermalData->getTabTypeData(_tabID);

			if (td == nullptr)
				return;
		}		

		TabType tab = td->getType();
		int n = td->getYXCount();

		QString dbstr;
		if (tab == TabType::TQNODE)
		{
			switch (t1)
			{
			case ThermalType::AREA:
				for (int i = 0; i < n; i++)
				{
					double* yx = td->getYXValue(i);
					double* v = new double[2];

					v[0] = yx[0] * mul * yl / yf;
					v[1] = yx[1] * mul * yl / yf;
					dbstr.append(QString::number(v[0])).append(" , ").append(QString::number(v[1])).append(" , ");
					appendQ(v);
				}
				break;
			case ThermalType::LENG:
				for (int i = 0; i < n; i++)
				{
					double* yx = td->getYXValue(i);
					double* v = new double[2];

					v[0] = yx[0] * mul / yf;
					v[1] = yx[1] * mul / yf;
					dbstr.append(QString::number(v[0])).append(" , ").append(QString::number(v[1])).append(" , ");
					appendQ(v);
				}
				break;
			case ThermalType::TOTA:
				for (int i = 0; i < n; i++)
				{
					double* yx = td->getYXValue(i);
					double* v = new double[2];

					v[0] = yx[0] * mul / yl / yf;
					v[1] = yx[1] * mul / yl / yf;
					dbstr.append(QString::number(v[0])).append(" , ").append(QString::number(v[1])).append(" , ");
					appendQ(v);
				}
				break;
			case ThermalType::VOLU:
				for (int i = 0; i < n; i++)
				{
					double* yx = td->getYXValue(i);
					double* v = new double[2];

					v[0] = yx[0] * mul * yl * yl / yf;
					v[1] = yx[1] * mul * yl * yl / yf;
					dbstr.append(QString::number(v[0])).append(" , ").append(QString::number(v[1])).append(" , ");
					appendQ(v);
				}
				break;
			default:
				break;
			}
		}
		else if (tab == TabType::TEMP)
		{
			for (int i = 0; i < n; i++)
			{
				double* yx = td->getYXValue(i);
				double* v = new double[2];

				v[0] = yx[0] * mul * yl / xt;
				v[1] = yx[1] * mul * yl / xt;
				dbstr.append(QString::number(v[0])).append(" , ").append(QString::number(v[1])).append(" , ");
				appendQ(v);
			}
		}
		QString dbstr1 = dbstr.left(dbstr.size() - 2);
		DataProperty::PropertyString* pStr = dynamic_cast<DataProperty::PropertyString*>(getPropertyByName(QObject::tr("sQValue")));
		if (!pStr)		
			appendProperty(QObject::tr("sQValue"), dbstr1);		
		else
			pStr->setValue(dbstr1);
		removeProperty(QObject::tr("dQValue"));
	}

	void ThermalBoundary::appendQ(double* q)
	{
		_q.append(q);
	}

	int ThermalBoundary::getQCount()
	{
		return _q.count();
	}

	double* ThermalBoundary::getQFromIndex(int index)
	{
		if (index < 0 || index >= _q.size())
			return 0;

		return _q.at(index);
	}

	QStringList ThermalBoundary::getThermalType(BoundaryType btype)
	{
		QStringList type{};

		if (btype == BoundaryType::BNONE)
			btype = _type;
		if (btype == BoundaryType::BSINK)
			type.append("SINK");
		else if (btype == BoundaryType::BINTER)
		{
			type.append("SINK");
			type.append("INTERP");
		}
		else if (btype == BoundaryType::QAREA)
		{
			type.append("QNODE");
			type.append("AREA");
		}
		else if (btype == BoundaryType::QLENG)
		{
			type.append("QNODE");
			type.append("LENG");
		}
		else if (btype == BoundaryType::QTOTA)
		{
			type.append("QNODE");
			type.append("TOTAL");
		}
		else if (btype == BoundaryType::QVOLU)
		{
			type.append("QNODE");
			type.append("VOLU");
		}
		else if (btype == BoundaryType::QNULL)
		{
			type.append("QNODE");
			type.append("NULL");
		}
		else if (btype == BoundaryType::IAREA)
		{
			type.append("INTERP");
			type.append("AREA");
		}
		else if (btype == BoundaryType::ILENG)
		{
			type.append("INTERP");
			type.append("LENG");
		}
		else if (btype == BoundaryType::ITOTA)
		{
			type.append("INTERP");
			type.append("TOTAL");
		}
		else if (btype == BoundaryType::IVOLU)
		{
			type.append("INTERP");
			type.append("VOLU");
		}
		else if (btype == BoundaryType::INULL)
		{
			type.append("INTERP");
			type.append("NULL");
		}		
		return type;
	}

	void ThermalBoundary::changeQValue(BoundaryType b, double v, BoundaryTabTypeData* td)
	{
		ThermalType o = ThermalType::NONE;
		ThermalType t = ThermalType::NONE;

		QStringList list = getThermalType();
		o = _thermalData->stringToThermalType(list.at(0));
		if (list.size()>1)
			t = _thermalData->stringToThermalType(list.at(1));
		if (list.at(0) == "INTERP")	setQ(t, v, td);
		else setQ(o, t, v);		
	}

	void ThermalBoundary::setTableTypeID(int id)
	{
		_tabID = id;
	}

	int ThermalBoundary::getTableTypeID()
	{
		return _tabID;
	}

// 	QString ThermalBoundary::getType()
// 	{
// 		if (_type == BoundaryType::BSINK);
// 		else if (_type == BoundaryType::BSINK)
// 		{
// 		}
// 		else if (_type == BoundaryType::BSINK)
// 		{
// 		}
// 
// 		return "";
// 	}

	QString ThermalBoundary::getUnit()
	{
		return _unit;
	}

	void ThermalBoundary::setXValue(double v)
	{
		_xValue = v;
	}

	double ThermalBoundary::getXValue()
	{
		return _xValue;
	}

// 	void ThermalBoundary::setQ(QString f1, QString f2, double yl, double yf, double xt)
// 	{
// 		if (f1 == "SINK")
// 		{
// 			_q = _xValue / xt;
// 		}
// 		else if (f1 == "QNODE")
// 		{
// 			if (f2 == "AREA")
// 			{
// 				_q = _xValue * yl / yf;
// 			}
// 			else if (f2 == "LENG")
// 			{
// 				_q = _xValue / yf;
// 			}
// 			else if (f2 == "TOTA")
// 			{
// 				_q = _xValue / yl / yf;
// 			}
// 			else if (f2 == "VOLU")
// 			{
// 				_q = _xValue * yl * yl / yf;
// 			}
// 			else
// 			{
// 				_q = _xValue / yl / yf;
// 			}
// 		}
// 		else if (f1 == "INTERP")
// 		{
// 			if (f2 == "AREA")
// 			{
// 			}
// 			else if (f2 == "LENG")
// 			{
// 			}
// 			else if (f2 == "TOTA")
// 			{
// 			}
// 			else if (f2 == "VOLU")
// 			{
// 			}
// 			else
// 			{
// 			}
// 		}
// 	}

// 	void ThermalBoundary::setQ(ThermalType t1, ThermalType t2, double yl, double yf, double yt)
// 	{
// 
// 	}
// 
// 	double ThermalBoundary::getQ()
// 	{
// 		return _q;
// 	}

	void ThermalBoundary::writeTbData(QDomDocument* doc, QDomElement* tb)
	{
		QString type1{};
		QStringList typelist = getThermalType();

		QDomElement aTb = doc->createElement("aThermalBoundary");
		aTb.setAttribute("id",getID());
		aTb.setAttribute("name", getName());
		aTb.setAttribute("groupName", _group);					
		aTb.setAttribute("uint", _unit);
		aTb.setAttribute("xValue", QString::number(_xValue, 'f', 10));
		aTb.setAttribute("tabID", QString::number(_tabID));			
		aTb.setAttribute("type0", getThermalType().at(0));
		if (typelist.size() == 2)
		{
			type1 = typelist.at(1);
			aTb.setAttribute("type1", type1);
		}		
		tb->appendChild(aTb);
	}

	void ThermalBoundary::readTbData(QDomElement *aTb)
	{
		if (!aTb->isNull())
		{
			setID(aTb->attribute("id").toInt());
			setName(aTb->attribute("name"));

			_group  = aTb->attribute("groupName");
			_unit   = aTb->attribute("uint");
			_xValue = aTb->attribute("xValue").toDouble();
			_tabID  = aTb->attribute("tabID").toInt();

			ThermalType type0 = _thermalData->stringToThermalType(aTb->attribute("type0"));
			ThermalType type1 = _thermalData->stringToThermalType(aTb->attribute("type1"));
			if (type0 == ThermalType::INTERP)
			{
				setTypeAndUnit(type1);
			}
			else
			{
				setTypeAndUnit(type0, type1);
			}			
		}
	}

	void ThermalBoundary::clearQList()
	{
		for (int i = 0; i < _q.size(); i++)
		{
			double* q = _q.at(i);
			delete q;
			//q = nullptr;
		}
		_q.clear();
	}

	void ThermalBoundary::setTypeAndUnit(const QString& type)
	{
		if (type == QObject::tr("BSINK"))
		{
			_type = BoundaryType::BSINK;
			_unit = "C";
		}
		else if (type == QObject::tr("QAREA"))
		{
			_unit = "W/m^2";
			_type = BoundaryType::QAREA;
		}
		else if (type == QObject::tr("QLENG"))
		{
			_unit = "W/m";
			_type = BoundaryType::QLENG;
		}
		else if (type == QObject::tr("QTOTA"))
		{
			_unit = "W";
			_type = BoundaryType::QTOTA;
		}
		else if (type == QObject::tr("QVOLU"))
		{
			_unit = "W/m^3";
			_type = BoundaryType::QVOLU;
		}
		else if (type == QObject::tr("QNULL"))
		{
			_unit = "W";
			_type = BoundaryType::QNULL;
		}
		else if (type == QObject::tr("IAREA"))
		{
			_unit = "W/m^2";
			_type = BoundaryType::IAREA;
		}
		else if (type == QObject::tr("ILENG"))
		{
			_unit = "times W/m";
			_type = BoundaryType::ILENG;
		}
		else if (type == QObject::tr("ITOTA"))
		{
			_unit = "W";
			_type = BoundaryType::ITOTA;
		}
		else if (type == QObject::tr("IVOLU"))
		{
			_unit = "times W/m^3";
			_type = BoundaryType::IVOLU;
		}
		else if (type == QObject::tr("INULL"))
		{
			_unit = "C";
			_type = BoundaryType::INULL;
		}
		else
		{
			_type = BoundaryType::BNONE;
		}
	}

	BoundaryType ThermalBoundary::getType()
	{
		return _type;
	}

}

