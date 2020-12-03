#include "SolidTube.h"
#include "Value.h"
#include <QDomElement>

namespace Geant4 {

	SolidTube::SolidTube()
	{
		m_rmin = new Value;
		m_rmax = new Value;
		m_z = new Value;
		m_startphi = new Value;
		m_deltaphi = new Value;
		this->setType(SolidType::SolidTube);
	}

	SolidTube::~SolidTube()
	{
		if (m_rmin != nullptr)delete m_rmin;
		if (m_rmax != nullptr)delete m_rmax;
		if (m_z != nullptr)delete m_z;
		if (m_startphi != nullptr)delete m_startphi;
		if (m_deltaphi != nullptr)delete m_deltaphi;
	}

	void SolidTube::setRmin(QString s)
	{
		bool ok = false;
		double v = s.toDouble(&ok);
		if (ok || s.isEmpty())
		{
			m_rmin->setValue(v);
			this->appendProperty("Rmin", v);
		}
		else {
			m_rmin->setReferance(s);
			this->appendProperty("Rmin", s);
		}
	}

	void SolidTube::setRmax(QString s)
	{
		bool ok = false;
		double v = s.toDouble(&ok);
		if (ok || s.isEmpty())
		{
			m_rmax->setValue(v);
			this->appendProperty("Rmax", v);
		}
		else
		{
			m_rmax->setReferance(s);
			this->appendProperty("Rmax", s);
		}
	}

	void SolidTube::setZValue(QString s)
	{
		bool ok = false;
		double v = s.toDouble(&ok);
		if (ok || s.isEmpty())
		{
			m_z->setValue(v);
			this->appendProperty("Deltaphi", v);
		}
		else
		{
			m_z->setReferance(s);
			this->appendProperty("Deltaphi", s);
		}
	}

	void SolidTube::setStartphi(QString s)
	{
		bool ok = false;
		double v = s.toDouble(&ok);
		if (ok || s.isEmpty())
		{
			m_startphi->setValue(v);
			this->appendProperty("Startphi", v);
		}
		else
		{
			m_startphi->setReferance(s);
			this->appendProperty("Startphi", s);
		}
	}

	void SolidTube::setDeltaphi(QString s)
	{
		bool ok = false;
		double v = s.toDouble(&ok);
		if (ok || s.isEmpty())
		{
			m_deltaphi->setValue(v);
			this->appendProperty("Deltaphi", v);
		}
		else
		{
			m_deltaphi->setReferance(s);
			this->appendProperty("Deltaphi", s);
		}
	}

	void SolidTube::getValue(QStringList& list)
	{
		QString rmin = m_rmin->getReference();
		QString rmax = m_rmax->getReference();
		QString z = m_z->getReference();
		QString startphi = m_startphi->getReference();
		QString deltaphi = m_deltaphi->getReference();

		if (rmin.isEmpty())
			list.append(QString::number(m_rmin->getValue()));
		else
			list.append(rmin);
		if (rmax.isEmpty())
			list.append(QString::number(m_rmax->getValue()));
		else
			list.append(rmax);
		if (z.isEmpty())
			list.append(QString::number(m_z->getValue()));
		else
			list.append(z);
		if (startphi.isEmpty())
			list.append(QString::number(m_startphi->getValue()));
		else
			list.append(startphi);
		if (deltaphi.isEmpty())
			list.append(QString::number(m_deltaphi->getValue()));
		else
			list.append(deltaphi);
	}

	void SolidTube::generateShape()
	{

	}

	void SolidTube::setParameterUnit(QString unit)
	{
		if (unit.isEmpty())
			unit = "mm";

		m_lunit = unit;
		this->appendProperty("lunit", unit);
	}

	QString SolidTube::getParameterUnit()
	{
		return m_lunit;
	}

	void SolidTube::setParameterAunit(QString unit)
	{
		if (unit.isEmpty())
			unit = "rad";

		m_aunit = unit;
		this->appendProperty("aunit", unit);
	}

	QString SolidTube::getParameterAunit()
	{
		return m_aunit;
	}

	void SolidTube::getShapeParameter(double* p)
	{
		p[0] = m_rmin->getValue();
		p[1] = m_rmax->getValue();
		p[2] = m_z->getValue();
		p[3] = m_startphi->getValue();
		p[4] = m_deltaphi->getValue();
	}

	void SolidTube::readGDML(QDomElement& ele)
	{
		QString name = ele.attribute("name");
		this->setName(name);
		QString lunit = ele.attribute("lunit");
		this->setParameterUnit(lunit);
		QString aunit = ele.attribute("aunit");
		this->setParameterAunit(aunit);
		QString rmin = ele.attribute("rmin");
		this->setRmin(rmin);
		QString rmax = ele.attribute("rmax");
		this->setRmax(rmax);
		QString z = ele.attribute("z");
		this->setZValue(z);
		QString startphi = ele.attribute("startphi");
		this->setStartphi(startphi);
		QString deltaphi = ele.attribute("deltaphi");
		this->setDeltaphi(deltaphi);
	}

	void SolidTube::writeGDML(QDomDocument& doc, QDomElement& ele)
	{
		QDomElement tube = doc.createElement("tube");
		tube.setAttribute("name", this->getName());
		tube.setAttribute("lunit", m_lunit);
		tube.setAttribute("aunit", m_aunit);
		QStringList list;
		this->getValue(list);
		tube.setAttribute("rmin", list.at(0));
		tube.setAttribute("rmax", list.at(1));
		tube.setAttribute("z", list.at(2));
		tube.setAttribute("startphi", list.at(3));
		tube.setAttribute("deltaphi", list.at(4));
		ele.appendChild(tube);
	}

}