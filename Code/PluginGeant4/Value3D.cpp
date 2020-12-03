#include <Value3D.h>
#include "Value.h"
namespace Geant4
{
	Value3D::Value3D()
	{
		_x = new Value;
		_y = new Value;
		_z = new Value;
	}

	Value3D::~Value3D()
	{
		if(_x != nullptr) delete _x;
		if(_y != nullptr) delete _y;
		if(_z != nullptr) delete _z;
	}

	void Value3D::getValue(double* v)
	{
		v[0] = _x->getValue();
		v[1] = _y->getValue();
		v[2] = _z->getValue();
	}

	void Value3D::getValue(QStringList& list)
	{
		QString x = _x->getReference();
		QString y = _y->getReference();
		QString z = _z->getReference();

		if (x.isEmpty())
			list.append(QString::number(_x->getValue()));
		else
			list.append(x);

		if (y.isEmpty())
			list.append(QString::number(_y->getValue()));
		else
			list.append(y);

		if (z.isEmpty())
			list.append(QString::number(_z->getValue()));
		else
			list.append(z);
	}

	void Value3D::setName(QString name)
	{
		_name = name;
	}

	QString Value3D::getName()
	{
		return _name;
	}

	void Value3D::setUnit(QString unit)
	{
		_unit = unit;
	}

	QString Value3D::getUnit()
	{
		return _unit;
	}

	void Value3D::setValue(double x, double y, double z)
	{
		
		_x->setValue(x);
		_y->setValue(y);
		_z->setValue(z);
	}

	void Value3D::setValue(QString x, QString y, QString z)
	{
		bool ok = false;
		double xv = x.toDouble(&ok);
		if (!ok)
			_x->setReferance(x);
		else
			_x->setValue(xv);

		double yv = y.toDouble(&ok);
		if (!ok)
			_y->setReferance(y);
		else
			_y->setValue(yv);
		
		double zv = z.toDouble(&ok);
		if (!ok)
			_z->setReferance(z);
		else
			_z->setValue(zv);
	}

	Geant4::Value* Value3D::getValueX()
	{
		return  _x;
	}

	Geant4::Value* Value3D::getValueY()
	{
		return _y;
	}

	Value* Value3D::getValueZ()
	{
		return _z;
	}

}


