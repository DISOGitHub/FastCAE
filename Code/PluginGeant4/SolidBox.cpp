#include "SolidBox.h"
#include "Value3D.h"
#include "Value.h"
#include <QDomElement>
#include <BRepPrimAPI_MakeBox.hxx>
#include <TopoDS_Shape.hxx>

namespace Geant4 
{
	SolidBox::SolidBox()
	{
		_parameter = new Value3D;
		m_xValue = new Value;
		m_yValue = new Value;
		m_zValue = new Value;
		this->setType(SolidType::SolidBox);
	}

	SolidBox::~SolidBox()
	{
		if (nullptr != _parameter) delete _parameter;
		if (nullptr != m_xValue) delete m_xValue;
		if (nullptr != m_yValue) delete m_yValue;
		if (nullptr != m_zValue) delete m_zValue;
	}

	void SolidBox::setXValue(QString s)
	{
		bool ok = false;
		double v = s.toDouble(&ok);
		if (ok || s.isEmpty())
		{
			m_xValue->setValue(v);
			this->appendProperty("x", v);
		}
		else
		{
			m_xValue->setReferance(s);
			this->appendProperty("x", s);
		}
	}

	void SolidBox::setYValue(QString s)
	{
		bool ok = false;
		double v = s.toDouble(&ok);
		if (ok || s.isEmpty())
		{
			m_yValue->setValue(v);
			this->appendProperty("y", v);
		}
		else
		{
			m_yValue->setReferance(s);
			this->appendProperty("y", s);
		}
	}

	void SolidBox::setZValue(QString s)
	{
		bool ok = false;
		double v = s.toDouble(&ok);
		if (ok || s.isEmpty())
		{
			m_zValue->setValue(v);
			this->appendProperty("z", v);
		}
		else
		{
			m_zValue->setReferance(s);
			this->appendProperty("z", s);
		}
	}

	Value* SolidBox::getXValue()
	{
		return m_xValue;
	}

	Value* SolidBox::getYValue()
	{
		return m_yValue;
	}

	Value* SolidBox::getZValue()
	{
		return m_zValue;
	}

	void SolidBox::generateShape()
	{
		double coor[3] = { 0 };
		this->getShapeParameter(coor);

		gp_Pnt pt(coor[0]/-2.0, coor[1] / -2.0, coor[2] / -2.0);
		TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(pt, coor[0], coor[1], coor[2]).Shape();
		_shape = new TopoDS_Shape;
		*_shape = aTopoBox;
	}

	void SolidBox::setParameterUnit(QString unit)
	{
		if (unit.isEmpty())
			unit = "mm";
		//_parameter->setUnit(unit);
		m_unit = unit;
		this->appendProperty("Unit", unit);
	}

	QString SolidBox::getParameterUnit()
	{
		//return _parameter->getUnit();
		return m_unit;
	}


	void SolidBox::setShapeParameter(double* p)
	{
		_parameter->setValue(p[0], p[1], p[2]);
		this->appendProperty("Length", p[0]);
		this->appendProperty("Width", p[1]);
		this->appendProperty("High", p[2]);
	}

	void SolidBox::getShapeParameter(double* p)
	{
		//_parameter->getValue(p);
		p[0] = m_xValue->getValue();
		p[1] = m_yValue->getValue();
		p[2] = m_zValue->getValue();
	}

	void SolidBox::readGDML(QDomElement& boxele)
	{
		QString name = boxele.attribute("name");
		this->setName(name);
		QString unit = boxele.attribute("lunit");
		this->setParameterUnit(unit);
// 		double p[3] = { 0 };
// 		p[0] = boxele.attribute("x").toDouble();
// 		p[1] = boxele.attribute("y").toDouble();
// 		p[2] = boxele.attribute("z").toDouble();
		
		//this->setParameterUnit(unit);
		/*this->setShapeParameter(p);*/

		QString x = boxele.attribute("x");
		QString y = boxele.attribute("y");
		QString z = boxele.attribute("z");
		this->setXValue(x);
		this->setYValue(y);
		this->setZValue(z);
	}

	void SolidBox::writeGDML(QDomDocument& doc, QDomElement& ele)
	{
		QDomElement box = doc.createElement("box");
		box.setAttribute("name", this->getName());
		box.setAttribute("lunit", m_unit);
		QString x = m_xValue->getReference();
		QString y = m_yValue->getReference();
		QString z = m_zValue->getReference();
		if (x.isEmpty())
			box.setAttribute("x", m_xValue->getValue());
		else
			box.setAttribute("x", x);
		if (y.isEmpty())
			box.setAttribute("y", m_yValue->getValue());
		else
			box.setAttribute("y", y);
		if (z.isEmpty())
			box.setAttribute("z", m_zValue->getValue());
		else
			box.setAttribute("z", z);
		ele.appendChild(box);
	}

}