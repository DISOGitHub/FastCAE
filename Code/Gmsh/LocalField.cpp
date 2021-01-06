#include "LocalField.h"
#include "python/PyAgent.h"
#include <QStringList>

namespace Gmsh{
	
	void Box::appendLocals(Py::PythonAagent* py)
	{
		QString  code = QString("gmsher.appendBoxFields(%1,%2,%3,%4,%5,%6,%7,%8,%9,%10)")
			.arg(this->_thickness).arg(this->_vIn).arg(this->_vOut).arg(this->_x).arg(this->_y)
			.arg(this->_z).arg(this->_length).arg(this->_width).arg(this->_height).arg(this->_backgroundField);

		py->submit(code);

	}

	QStringList Box::getInformation()
	{
		QStringList list{};
		QString type, info, loc;
		type = "Box";
		loc = QString("P:(%1, %2, %3) L:%4 W:%5 H:%6").arg(this->_x).arg(this->_y).arg(this->_z)
			.arg(this->_length).arg(this->_width).arg(this->_height);
		info = QString("VIn:%1 VOut:%2 Tck:%3").arg(this->_vIn).arg(this->_vOut).arg(this->_thickness);

		list.append(type);
		list.append(loc);
		list.append(info);

		return list;
	}

	void Box::copy(LocalDensity* data)
	{
		if (data == nullptr)return;
		LocalDensity::copy(data);
		Box* d = dynamic_cast<Box*>(data);
		if (d == nullptr)return;

		this->_length = d->_length;
		this->_width = d->_width;
		this->_height = d->_height;
		
	}

	void Ball::appendLocals(Py::PythonAagent* py)
	{
		QString  code = QString("gmsher.appendBallFields(%1,%2,%3,%4,%5,%6,%7,%8)")
			.arg(this->_radius).arg(this->_thickness).arg(this->_vIn).arg(this->_vOut)
			.arg(this->_x).arg(this->_y).arg(this->_z).arg(this->_backgroundField);

		py->submit(code);


	}

	QStringList Ball::getInformation()
	{
		QStringList list{};
		QString type, info, loc;
		type = "Ball";
		loc = QString("P:(%1, %2, %3) R:%4").arg(this->_x).arg(this->_y).arg(this->_z).arg(this->_radius);
		info = QString("VIn:%1 VOut:%2 Tck:%3").arg(this->_vIn).arg(this->_vOut).arg(this->_thickness);

		list.append(type);
		list.append(loc);
		list.append(info);

		return list;
	}

	void Ball::copy(LocalDensity* data)
	{
		if (data == nullptr) return;
		LocalDensity::copy(data);
		Ball* d = dynamic_cast<Ball*>(data);
		if (d == nullptr) return;
		this->_radius = d->_radius;
	}

	void Cylinder::appendLocals(Py::PythonAagent* py)
	{		
		QString  code = QString("gmsher.appendCylinderFields(%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11)")
			.arg(this->_radius).arg(this->_length).arg(this->_x).arg(this->_y)
			.arg(this->_z).arg(this->_xAxis).arg(this->_yAxis).arg(this->_zAxis)
			.arg(this->_vIn).arg(this->_vOut).arg(this->_backgroundField);

		py->submit(code);

	}

	QStringList Cylinder::getInformation()
	{
		QStringList list{};
		QString type, info, loc;
		type = "Cylinder";
		loc = QString("P:(%1, %2, %3) R:%4 L:%5").arg(this->_x).arg(this->_y).arg(this->_z).arg(this->_radius).arg(this->_length);
		info = QString("VIn:%1 VOut:%2").arg(this->_vIn).arg(this->_vOut);

		list.append(type);
		list.append(loc);
		list.append(info);

		return list;
	}

	void Cylinder::copy(LocalDensity* data)
	{
		if (data == nullptr) return;
		LocalDensity::copy(data);
		Cylinder* d = dynamic_cast<Cylinder*>(data);
		if (d == nullptr) return;
		this->_length = d->_length;
		this->_xAxis = d->_xAxis;
		this->_yAxis = d->_yAxis;
		this->_zAxis = d->_zAxis;
		this->_radius = d->_radius;
	}

	void LocalPoint::appendLocals(Py::PythonAagent* py)
	{
		QString  code = QString("gmsher.appendSizeFiled(%1,%2,%3,%4)")/*.arg(0)*/.arg(this->_x).arg(this->_y).arg(this->_z).arg(this->_value);
		py->submit(code);
	}

	QStringList LocalPoint::getInformation()
	{
		QStringList list{};
		QString type, info, loc;
		type = "Point";
		loc = QString("P:(%1, %2, %3)").arg(this->_x).arg(this->_y).arg(this->_z);
		info = QString("Val:%1").arg(this->_value);

		list.append(type);
		list.append(loc);
		list.append(info);

		return list;
	}

	void LocalPoint::copy(LocalDensity* data)
	{
		if (data == nullptr) return;
		LocalDensity::copy(data);
		LocalPoint* d = dynamic_cast<LocalPoint*>(data);
		if (d == nullptr) return;
		this->_value = d->_value;
	}

	void LocalDensity::copy(LocalDensity* data)
	{
		if (data == nullptr) return;
		this->_x = data->_x;
		this->_y = data->_y;
		this->_z = data->_z;
		this->_thickness = data->_thickness;
		this->_vIn = data->_vIn;
		this->_vOut = data->_vOut;
		this->_backgroundField = data->_backgroundField;
		this->_type = data->_type;
	}

	void SolidFields::appendLocals(Py::PythonAagent* py)
	{
		QList<int> setlist = this->_solidHash.uniqueKeys();
		for (int id : setlist)
		{
			QList<int> indexlist = this->_solidHash.values(id);
			for (int index : indexlist)
			{
				QString  code = QString("gmsher.appendSolidsFields(%1,%2,%3,%4,%5,%6)").arg(this->_vIn)
					.arg(this->_vOut).arg(this->_thickness).arg(id).arg(index).arg(this->_backgroundField)/*.arg(this->_index)*/;
				py->submit(code);
			}
		}
	}

	QStringList SolidFields::getInformation()
	{

		QStringList list{};
		QString type, info, loc;
		type = "Solids";
		loc = QString("Num:%1").arg(_solidHash.size());
		info = QString("VIn:%1 VOut:%2").arg(this->_vIn).arg(this->_vOut);

		list.append(type);
		list.append(loc);
		list.append(info);

		return list;
	}

	void SolidFields::copy(LocalDensity* data)
	{
		if (data == nullptr) return;
		LocalDensity::copy(data);
		SolidFields* d = dynamic_cast<SolidFields*>(data);
		if (d == nullptr) return;
		this->_solidHash = d->_solidHash;
	}

	void SolidFields::setIndex(int d)
	{
		this->_index = d;
	}

	void Frustum::appendLocals(Py::PythonAagent* py)
	{
		QString  code = QString("gmsher.appendFrustumFields(%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14)")
			.arg(this->_iRadius).arg(this->_oRadius).arg(this->_iTopRadius).arg(this->_oTopRadius)
			.arg(this->_length).arg(this->_x).arg(this->_y)	.arg(this->_z).arg(this->_xAxis).arg(this->_yAxis)
			.arg(this->_zAxis).arg(this->_vIn).arg(this->_vOut).arg(this->_backgroundField);

		py->submit(code);
	}

	QStringList Frustum::getInformation()
	{
		QStringList list;
		QString type, info, loc;
		type = "Frustum";
		loc = QString("P:(%1, %2, %3) IR1:%4 OR1:%5 IR2:%6 OR2:%7 L:%8").arg(this->_x).arg(this->_y).arg(this->_z)
			.arg(this->_iRadius).arg(this->_oRadius).arg(this->_iTopRadius).arg(this->_oTopRadius).arg(this->_length);
		info = QString("VIn:%1 VOut:%2").arg(this->_vIn).arg(this->_vOut);

		list.append(type);
		list.append(loc);
		list.append(info);
		return list;
	}

	void Frustum::copy(LocalDensity* data)
	{
		if (data == nullptr) return;
		LocalDensity::copy(data);
		Frustum* d = dynamic_cast<Frustum*>(data);
		if (d == nullptr) return;
		this->_iRadius = d->_iRadius;
		this->_oRadius = d->_oRadius;
		this->_iTopRadius = d->_iTopRadius;
		this->_oTopRadius = d->_oTopRadius;
		this->_length = d->_length;
		this->_xAxis = d->_xAxis;
		this->_yAxis = d->_yAxis;
		this->_zAxis = d->_zAxis;
	}

}