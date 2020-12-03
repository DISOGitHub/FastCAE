#include "SolidSphere.h"
#include "Value.h"
#include <QDomElement>

namespace Geant4 {
	SolidSphere::SolidSphere()
	{
		m_rmin = new Value;
		m_rmax = new Value;
		m_startphi = new Value;
		m_deltaphi = new Value;
		m_starttheta = new Value;
		m_deltatheta = new Value;
		this->setType(SolidType::SolidSphere);
	}

	SolidSphere::~SolidSphere()
	{
		if (m_rmin != nullptr)delete m_rmin;
		if (m_rmax != nullptr)delete m_rmax;
		if (m_startphi != nullptr)delete m_startphi;
		if (m_deltaphi != nullptr)delete m_deltaphi;
		if (m_starttheta != nullptr)delete m_starttheta;
		if (m_deltatheta != nullptr)delete m_deltatheta;
	}

	void SolidSphere::setRmin(QString s)
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

	void SolidSphere::setRmax(QString s)
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

	void SolidSphere::setStartphi(QString s)
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

	void SolidSphere::setDeltaphi(QString s)
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

	void SolidSphere::setStarttheta(QString s)
	{
		bool ok = false;
		double v = s.toDouble(&ok);
		if (ok || s.isEmpty()) {
			m_starttheta->setValue(v);
			this->appendProperty("Starttheta", v);
		}
			
		else {
			m_starttheta->setReferance(s);
			this->appendProperty("Starttheta", s);
		}
			
	}

	void SolidSphere::setDeltatheta(QString s)
	{
		bool ok = false;
		double v = s.toDouble(&ok);
		if (ok || s.isEmpty())
		{
			m_deltatheta->setValue(v);
			this->appendProperty("Deltatheta", v);
		}
		else
		{
			m_deltatheta->setReferance(s);
			this->appendProperty("Deltatheta", s);
		}
			
	}

	Value* SolidSphere::getRmin()
	{
		return m_rmin;
	}

	Value* SolidSphere::getRmax()
	{
		return m_rmax;
	}

	Value* SolidSphere::getStartphi()
	{
		return m_startphi;
	}

	Value* SolidSphere::getDeltaphi()
	{
		return m_deltaphi;
	}

	Value* SolidSphere::getStarttheta()
	{
		return m_starttheta;
	}

	Value* SolidSphere::getDeltatheta()
	{
		return m_deltatheta;
	}

	void SolidSphere::generateShape()
	{

	}

	void SolidSphere::setParameterUnit(QString unit)
	{
		if (unit.isEmpty())
			unit = "mm";

		m_lunit = unit;
		this->appendProperty("lunit", unit);
	}

	QString SolidSphere::getParameterUnit()
	{
		return m_lunit;
	}

	void SolidSphere::setParameterAunit(QString unit)
	{
		if (unit.isEmpty())
			unit = "rad";
		
		m_aunit = unit;
		this->appendProperty("aunit", unit);
	}

	QString SolidSphere::getParameterAunit()
	{
		return m_aunit;
	}

	void SolidSphere::getShapeParameter(double* p)
	{
		p[0] = m_rmin->getValue();
		p[1] = m_rmax->getValue();
		p[2] = m_startphi->getValue();
		p[3] = m_deltaphi->getValue();
		p[4] = m_starttheta->getValue();
		p[5] = m_deltatheta->getValue();
	}

	void SolidSphere::readGDML(QDomElement& ele)
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
		QString startphi = ele.attribute("startphi");
		this->setStartphi(startphi);
		QString deltaphi = ele.attribute("deltaphi");
		this->setDeltaphi(deltaphi);
		QString starttheta = ele.attribute("starttheta");
		this->setStarttheta(starttheta);
		QString deltatheta = ele.attribute("deltatheta");
		this->setDeltatheta(deltatheta);		
	}

	void SolidSphere::writeGDML(QDomDocument& doc, QDomElement& ele)
	{
		QDomElement sphere = doc.createElement("sphere");
		sphere.setAttribute("name", this->getName());
		sphere.setAttribute("lunit", m_lunit);
		sphere.setAttribute("aunit", m_aunit);
		QString min = m_rmin->getReference();
		QString max = m_rmax->getReference();
		QString sphi = m_startphi->getReference();
		QString dphi = m_deltaphi->getReference();
		QString stheta = m_starttheta->getReference();
		QString dtheta = m_deltatheta->getReference();
		if (min.isEmpty())
			sphere.setAttribute("rmin", m_rmin->getValue());
		else
			sphere.setAttribute("rmin", min);
		if (max.isEmpty())
			sphere.setAttribute("rmax", m_rmax->getValue());
		else
			sphere.setAttribute("rmax", max);
		if (sphi.isEmpty())
			sphere.setAttribute("startphi", m_startphi->getValue());
		else
			sphere.setAttribute("startphi", sphi);
		if (dphi.isEmpty())
			sphere.setAttribute("deltaphi", m_deltaphi->getValue());
		else
			sphere.setAttribute("deltaphi", dphi);
		if (stheta.isEmpty())
			sphere.setAttribute("starttheta", m_starttheta->getValue());
		else
			sphere.setAttribute("starttheta", stheta);
		if (dtheta.isEmpty())
			sphere.setAttribute("deltatheta", m_deltatheta->getValue());
		else
			sphere.setAttribute("deltatheta", dtheta);
		ele.appendChild(sphere);
	}

}