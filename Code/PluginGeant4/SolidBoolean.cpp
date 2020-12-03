#include "SolidBoolean.h"
#include "Position.h"
#include "Rotation.h"
#include <QDomElement>

namespace Geant4 {

	SolidBoolean::SolidBoolean()
	{
		
	}

	SolidBoolean::~SolidBoolean()
	{
		if (m_position != nullptr)delete m_position;
		if (m_rotation != nullptr)delete m_rotation;
	}

	void SolidBoolean::setFirst(QString s)
	{
		m_first = s;
		this->appendProperty("first", s);
	}

	QString SolidBoolean::getFirst()
	{
		return m_first;
	}

	void SolidBoolean::setSecond(QString s)
	{
		m_second = s;
		this->appendProperty("second", m_second);
	}

	QString SolidBoolean::getSecond()
	{
		return m_second;
	}

	void SolidBoolean::setPositionValue(QStringList list)
	{
		QString ref = list.at(0);
		QString name = list.at(1);
		QString unit = list.at(2);
		QString x = list.at(3);
		QString y = list.at(4);
		QString z = list.at(5);

		if (ref.isEmpty())
		{
			if (m_position == nullptr)
				m_position = new Position;
			m_position->setName(name);
			m_position->setUnit(unit);
			m_position->setValue(x, y, z);
		}
		else
		{
			m_positionref = ref;
			this->appendProperty("PositionRef", ref);
		}
		
	}

	void SolidBoolean::getPositionValue(QStringList& list)
	{
		list.append(m_positionref);
		if (m_position != nullptr)
		{
			list.append(m_position->getName());
			list.append(m_position->getUnit());
			m_position->getValue(list);
		}
	}

	void SolidBoolean::setRotationValue(QStringList list)
	{
		QString ref = list.at(0);
		QString name = list.at(1);
		QString unit = list.at(2);
		QString x = list.at(3);
		QString y = list.at(4);
		QString z = list.at(5);

		if (ref.isEmpty())
		{
			if (m_rotation == nullptr)
				m_rotation = new Rotation;
			m_rotation->setName(name);
			m_rotation->setUnit(unit);
			m_rotation->setValue(x, y, z);
		}
		else
		{
			m_rotationref = ref;
			this->appendProperty("RotationRef", ref);
		}
	}

	void SolidBoolean::getRotationValue(QStringList& list)
	{
		list.append(m_rotationref);
		if (m_rotation != nullptr)
		{
			list.append(m_rotation->getName());
			list.append(m_rotation->getUnit());
			m_rotation->getValue(list);
		}
	}

	void SolidBoolean::generateShape()
	{

	}

	void SolidBoolean::readGDML(QDomElement& ele)
	{
		QString type = ele.tagName();
		this->setType(SringToType(type));
		QString name = ele.attribute("name");
		this->setName(name);
		QDomNodeList nodelist = ele.childNodes();
		for (int i = 0; i < nodelist.size(); i++)
		{
			QDomElement nele = nodelist.at(i).toElement();
			QString name = nele.tagName();
			if (name == "first")
			{
				QString s = nele.attribute("ref");
				this->setFirst(s);
			}				
			else if (name == "second")
			{
				QString s = nele.attribute("ref");
				this->setSecond(s);
			}
			else if (name == "positionref")
			{
				m_positionref = nele.attribute("ref");
				this->appendProperty("PositionRef", m_positionref);
			}
			else if (name == "rotationref")
			{
				m_rotationref = nele.attribute("ref");
				this->appendProperty("RotationRef", m_rotationref);
			}
			else if (name == "position")
			{
				m_position = new Position;
				m_position->readGDML(nele);
			}
			else if (name == "rotation")
			{
				m_rotation = new Rotation;
				m_rotation->readGDML(nele);
			}
		}

	}

	void SolidBoolean::writeGDML(QDomDocument& doc, QDomElement& ele)
	{
		QDomElement boolean = doc.createElement(TypeToString(_type).toLower());
		boolean.setAttribute("name", this->getName());
		if (!m_first.isEmpty())
		{
			QDomElement first = doc.createElement("first");
			first.setAttribute("ref", m_first);
			boolean.appendChild(first);
		}
		if (!m_second.isEmpty())
		{
			QDomElement second = doc.createElement("second");
			second.setAttribute("ref", m_second);
			boolean.appendChild(second);
		}
		if (!m_positionref.isEmpty())
		{
			QDomElement pref = doc.createElement("positionref");
			pref.setAttribute("ref", m_positionref);
			boolean.appendChild(pref);
		}
		if (!m_rotationref.isEmpty())
		{
			QDomElement rref = doc.createElement("rotationref");
			rref.setAttribute("ref", m_rotationref);
			boolean.appendChild(rref);
		}
		if (m_position != nullptr)
			m_position->writeGDML(doc, boolean);
		if (m_rotation != nullptr)
			m_rotation->writeGDML(doc, boolean);

		ele.appendChild(boolean);
	}

}