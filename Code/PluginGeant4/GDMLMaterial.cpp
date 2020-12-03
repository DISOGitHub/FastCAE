#include "Value.h"
#include "GDMLMaterial.h"
#include <QDomElement>
#include <QDomNodeList>
#include <qdebug.h>

namespace Geant4
{
	GDMLMaterial::GDMLMaterial()
	{
		_value = new Value;
	}


	GDMLMaterial::~GDMLMaterial()
	{
		delete _value;
	}

	void GDMLMaterial::setName(const QString &name)
	{
		_name = name;
		this->appendProperty("Name", name);
	}

	void GDMLMaterial::setState(QString state)
	{
		if (state.isEmpty()) return;
		_state = state;
		this->appendProperty("State", state);
	}

	QString GDMLMaterial::getState()
	{
		return _state;
	}
	void GDMLMaterial::setFormula(QString f)
	{
		_formula = f;

	}

	QString GDMLMaterial::getFormula()
	{
		return _formula;
	}

	void GDMLMaterial::setZ(QString Z)
	{
		_Z = Z;
	}

	QString GDMLMaterial::getZ()
	{
		return _Z;
	}
	
	void GDMLMaterial::appendMaterialType(MaterialType m)
	{
		_materialtypelist.append(m);
	}

	void GDMLMaterial::appendFraction(Fraction f)
	{
		_fracList.append(f);
	}

	void GDMLMaterial::appendComposite(Composite com)
	{
		_compositelist.append(com);
	}
	
	QList<Geant4::MaterialType> GDMLMaterial::getMaterialTypeList()
	{
		return _materialtypelist;
	}

	MaterialType GDMLMaterial::getMaterialTypeByIndex(int index)
	{
		if (index < 0 || index >= _materialtypelist.size())
			return MaterialType{};

		return _materialtypelist.at(index);
	}

	int GDMLMaterial::getMaterialTypeCount()
	{
		return _materialtypelist.size();
	}

	QList<Geant4::Fraction> GDMLMaterial::getFractionList()
	{
		return _fracList;
	}

	Geant4::Fraction GDMLMaterial::getFractionByIndex(int index)
	{
		if (index < 0 || index >= _fracList.size())
			return Fraction{};

		return _fracList.at(index);
	}

	int GDMLMaterial::getFractionCount()
	{
		return _fracList.size();
	}

	QList<Geant4::Composite> GDMLMaterial::getCompositeList()
	{
		return _compositelist;
	}

	Geant4::Composite GDMLMaterial::getCompositeByIndex(int index)
	{
		if (index < 0 || index >= _compositelist.size())
			return Composite{};

		return _compositelist.at(index);
	}

	int GDMLMaterial::getCompositeCount()
	{
		return _compositelist.size();
	}

	void GDMLMaterial::setValue(double v)
	{
		_value->setValue(v);
	}

	double GDMLMaterial::getValue()
	{
		return _value->getValue();
	}

	void GDMLMaterial::clearMaterial()
	{
		_compositelist.clear();
		_materialtypelist.clear();
		_fracList.clear();
		
	}

	void GDMLMaterial::writeGDML(QDomDocument& doc, QDomElement& materil)
	{

		for (int i = 0; i < _fracList.size(); i++)
		{
			Fraction fra = _fracList[i];

			QDomElement fraele = doc.createElement("fraction");
			fraele.setAttribute("n", fra.n);
			fraele.setAttribute("ref", fra.ref);
			materil.appendChild(fraele);
		}
		for (int i = 0; i < _materialtypelist.size(); i++)
		{
			MaterialType dtp = _materialtypelist[i];
			QDomElement dtpele;
			QString type = dtp.type;
			if (type=="density")
			{
				dtpele = doc.createElement("D");
			}
			else if (type == "temperature")
			{
				dtpele = doc.createElement("T");
			}
			else if (type == "pascal")
			{
				dtpele = doc.createElement("P");
			}
			dtpele.setAttribute("value", dtp.value);
			dtpele.setAttribute("unit", dtp.unit);
			materil.appendChild(dtpele);

		}
		for (int i = 0; i < _compositelist.size(); i++)
		{
			Composite site = _compositelist[i];
			QDomElement siteele = doc.createElement("composite");
			siteele.setAttribute("n", site.n);
			siteele.setAttribute("ref", site.ref);
			materil.appendChild(siteele);
		}
	}

	void GDMLMaterial::readGDML(QDomElement& ele)
	{
		QString name = ele.attribute("name");
		QString s = ele.attribute("state");
		QString f = ele.attribute("formula");
		QString z = ele.attribute("Z");
		this->setName(name);
		this->setState(s);
		this->setFormula(f);
		this->setZ(z);
		QDomNodeList chillist = ele.childNodes();
		for (int i = 0; i < chillist.size(); i++)
		{
			QDomNode onechild = chillist.at(i);
			QString nodename = onechild.nodeName();
			QDomElement elechild = onechild.toElement();
			if (nodename == "D")
			{

				MaterialType temp;
				temp.type = "density";
				temp.unit = QString("g/cm3");
				QString unit= elechild.attribute("unit");
				if (!unit.isEmpty()) temp.unit = elechild.attribute("unit");
				temp.value = elechild.attribute("value").toDouble();
				this->appendMaterialType(temp);
				this->appendProperty("D", QString::number(temp.value) + temp.unit);

			}
			else if (nodename == "T")
			{
				MaterialType temp;
				temp.type = "temperature";
				temp.unit = QString("K");
				QString unit= elechild.attribute("unit");
			
				if (!unit.isEmpty()) temp.unit = elechild.attribute("unit");
				temp.value = elechild.attribute("value").toDouble();
				this->appendMaterialType(temp);
				this->appendProperty("T", QString::number(temp.value) + temp.unit);

			}
			else if (nodename == "P")
			{
				MaterialType temp;
				temp.type = "pressure";
				temp.unit = QString("pascal");
				QString unit = elechild.attribute("unit");

				if (!unit.isEmpty()) temp.unit = elechild.attribute("unit");
				temp.value = elechild.attribute("value").toDouble();
				this->appendMaterialType(temp);
				this->appendProperty("P", QString::number(temp.value) + temp.unit);

			}
			else if (nodename == "fraction")
			{
				Fraction fra;
				fra.n = elechild.attribute("n").toDouble();
				fra.ref = elechild.attribute("ref");
				this->appendFraction(fra);
			}
			else if (nodename == "composite")
			{
				Composite site;
				site.n = elechild.attribute("n").toDouble();
				site.ref = elechild.attribute("ref");
				this->appendComposite(site);
			}
			else if (nodename == "atom")
			{
				double v = elechild.attribute("value").toDouble();
				this->setValue(v);
			}
		}

	}

}


