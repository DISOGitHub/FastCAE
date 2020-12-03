#include "TFMaterial.h"
#include <QDomDocument>

namespace ThermalFluid{
	TFMaterial::TFMaterial()
	{
	}


	TFMaterial::~TFMaterial()
	{
	}

	void TFMaterial::setRho(double r)
	{
		_rho = r;
	}

	double TFMaterial::getRho()
	{
		return _rho;
	}

	void TFMaterial::setCpp(double c)
	{
		_cpp = c;
	}

	double TFMaterial::getCpp()
	{
		return _cpp;
	}

	void TFMaterial::setKtherm(double k)
	{
		_ktherm = k;
	}

	double TFMaterial::getKtherm()
	{
		return _ktherm;
	}

	void TFMaterial::setEmiss(double e)
	{
		_emiss = e;
	}

	double TFMaterial::getEmiss()
	{
		return _emiss;
	}

	void TFMaterial::setAbsorp(double a)
	{
		_absorp = a;
	}

	double TFMaterial::getAbsorp()
	{
		return _absorp;
	}

	void TFMaterial::setSpecularity(double s)
	{
		_specularity = s;
	}

	double TFMaterial::getSpecularity()
	{
		return _specularity;
	}

	void TFMaterial::setRemiss(double re)
	{
		_remiss = re;
	}

	double TFMaterial::getRemiss()
	{
		return _remiss;
	}

	void TFMaterial::setRabsor(double ra)
	{
		_rabsor = ra;
	}

	double TFMaterial::getRabsor()
	{
		return _rabsor;
	}

	void TFMaterial::setRsolspe(double rs)
	{
		_rsolspe = rs;
	}

	double TFMaterial::getRsolspe()
	{
		return _rsolspe;
	}

	void TFMaterial::setProperty(QString p, QString v,double y[3])
	{
		double val = 0;
		double yl = y[0];
		double yt = y[1];
		double yf = y[2];

		val = v.toDouble();

		if (p == "RHO") _rho = val*yl*yl*yl*yl / yf;
		else if (p == "CPP") _cpp = val*yt / (yl*yl);
		else if (p == "KTHERM") _ktherm = val*yt/yf;
		else if (p == "E")
		{
			if (val <= 0)
				_emiss = 0;
			else
				_emiss = val;
		}
		else if (p == "ABSORP")
		{
			if (val <= 0)
				_absorp = 0;
			else
				_absorp = val;
		}
		else if (p == "SPECULARITY"){
			if (val <= 0)
				_specularity = 0;
			else
				_specularity = val;
		}
		else if (p == "REMISS"){
			if (val <= 0)
				_remiss = 0;
			else
				_remiss = val;
		}
		else if (p == "RABSOR"){
			if (val <= 0)
				_rabsor = 0;
			else
				_rabsor = val;
		}
		else if (p == "RSOLSPE")
		{
			if (val <= 0)
				_rsolspe = 0;
			else
				_rsolspe = val;
		}
	}

	void TFMaterial::appendToPropertyHash(QString p, QString v)
	{
		_propertyHash.insert(p, v);
	}

	QString TFMaterial::getPropertyValueFromHash(QString p)
	{
		return _propertyHash.value(p);
	}

	void TFMaterial::writeMaterialData(QDomDocument* doc, QDomElement* mater)
	{		
		QDomElement aMaterial = doc->createElement("aMaterial");
		aMaterial.setAttribute("id", getID());
		aMaterial.setAttribute("Name", getName());
		aMaterial.setAttribute("RHO", _propertyHash.value("RHO"));
		aMaterial.setAttribute("CPP", _propertyHash.value("CPP"));
		aMaterial.setAttribute("KTHERM", _propertyHash.value("KTHERM"));
		aMaterial.setAttribute("E", _propertyHash.value("E"));
		aMaterial.setAttribute("ABSORPTIVITY", _propertyHash.value("ABSORPTIVITY"));
		aMaterial.setAttribute("SPECULARITY", _propertyHash.value("SPECULARITY"));
		aMaterial.setAttribute("REMISS", _propertyHash.value("REMISS"));
		aMaterial.setAttribute("RABSOR", _propertyHash.value("RABSOR"));
		aMaterial.setAttribute("RSOLSPE", _propertyHash.value("RSOLSPE"));
		aMaterial.setAttribute("ELECRES", _propertyHash.value("ELECRES"));
		aMaterial.setAttribute("VISC", _propertyHash.value("VISC"));
		aMaterial.setAttribute("HTLAT", _propertyHash.value("HTLAT"));
		aMaterial.setAttribute("CAPAPH", _propertyHash.value("CAPAPH"));
		aMaterial.setAttribute("TPHASE", _propertyHash.value("TPHASE"));
		aMaterial.setAttribute("IRSPEC", _propertyHash.value("IRSPEC"));
		aMaterial.setAttribute("RIRSPEC", _propertyHash.value("RIRSPEC"));
		aMaterial.setAttribute("TRANSMISS", _propertyHash.value("TRANSMISS"));
		aMaterial.setAttribute("IRTRANS", _propertyHash.value("IRTRANS"));
		aMaterial.setAttribute("IREFFRONT", _propertyHash.value("IREFFRONT"));
		aMaterial.setAttribute("IREFBACK", _propertyHash.value("IREFBACK"));
		aMaterial.setAttribute("SOLAREXTINCT", _propertyHash.value("SOLAREXTINCT"));
		aMaterial.setAttribute("IREXTINCT", _propertyHash.value("IREXTINCT"));
		aMaterial.setAttribute("ABLAT", _propertyHash.value("ABLAT"));
		mater->appendChild(aMaterial);
	}

	void TFMaterial::readMaterialData(QDomElement* aMater)
	{
		if (!aMater->isNull())
		{
			setID(aMater->attribute("id").toInt());
			setName(aMater->attribute("Name"));

			_propertyHash.insert("RHO", aMater->attribute("RHO"));
			_propertyHash.insert("CPP", aMater->attribute("CPP"));
			_propertyHash.insert("KTHERM", aMater->attribute("KTHERM"));
			_propertyHash.insert("E", aMater->attribute("E"));
			_propertyHash.insert("ABSORPTIVITY", aMater->attribute("ABSORPTIVITY"));
			_propertyHash.insert("SPECULARITY", aMater->attribute("SPECULARITY"));
			_propertyHash.insert("REMISS", aMater->attribute("REMISS"));
			_propertyHash.insert("RABSOR", aMater->attribute("RABSOR"));
			_propertyHash.insert("RSOLSPE", aMater->attribute("RSOLSPE"));
			_propertyHash.insert("ELECRES", aMater->attribute("ELECRES"));
			_propertyHash.insert("VISC", aMater->attribute("VISC"));
			_propertyHash.insert("HTLAT", aMater->attribute("HTLAT"));
			_propertyHash.insert("CAPAPH", aMater->attribute("CAPAPH"));
			_propertyHash.insert("TPHASE", aMater->attribute("TPHASE"));
			_propertyHash.insert("IRSPEC", aMater->attribute("IRSPEC"));
			_propertyHash.insert("RIRSPEC", aMater->attribute("RIRSPEC"));
			_propertyHash.insert("TRANSMISS", aMater->attribute("TRANSMISS"));
			_propertyHash.insert("IRTRANS", aMater->attribute("IRTRANS"));
			_propertyHash.insert("IREFFRONT", aMater->attribute("IREFFRONT"));
			_propertyHash.insert("IREFBACK", aMater->attribute("IREFBACK"));
			_propertyHash.insert("SOLAREXTINCT", aMater->attribute("SOLAREXTINCT"));
			_propertyHash.insert("IREXTINCT", aMater->attribute("IREXTINCT"));
			_propertyHash.insert("ABLAT", aMater->attribute("ABLAT"));			
		}
	}

}