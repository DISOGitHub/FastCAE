#include "LocalField.h"
#include "python/PyAgent.h"
#include <QStringList>

namespace Gmsh{
	LocalField::LocalField()
	{
	}


	LocalField::~LocalField()
	{
	}

	void Box::appendFields(Py::PythonAagent* py)
	{
		int bg = 0;
		if (this->_backgroundField)
			bg = 1;

		QString  code = QString("gmsher.appendBoxFields(%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11)")
			.arg(1).arg(this->_thickness).arg(this->_vIn).arg(this->_vOut).arg(this->_xMax).arg(this->_xMin)
			.arg(this->_yMax).arg(this->_yMin).arg(this->_zMax).arg(this->_zMin).arg(this->_backgroundField);

		py->submit(code);

	}

	QStringList Box::getInformation()
	{
		QStringList list{};
		QString type, info, loc;
		type = "Box";
		loc = QString("P:(%1, %2, %3) L:%4 W:%5 H:%6").arg(this->_xMin).arg(this->_yMin).arg(this->_zMin)
			.arg(this->_xMax - this->_xMin).arg(this->_yMax - this->_yMin).arg(this->_zMax - this->_zMin);
		info = QString("VIn:%1 VOut:%2 Tck:%3").arg(this->_vIn).arg(this->_vOut).arg(this->_thickness);

		list.append(type);
		list.append(loc);
		list.append(info);

		return list;
	}

	void Ball::appendFields(Py::PythonAagent* py)
	{
		int bg = 0;
		if (this->_backgroundField)
			bg = 1;

		QString  code = QString("gmsher.appendBallFields(%1,%2,%3,%4,%5,%6,%7,%8,%9)")
			.arg(2).arg(this->_radius).arg(this->_thickness).arg(this->_vIn).arg(this->_vOut)
			.arg(this->_xCenter).arg(this->_yCenter).arg(this->_zCenter).arg(this->_backgroundField);

		py->submit(code);


	}

	QStringList Ball::getInformation()
	{
		QStringList list{};
		QString type, info, loc;
		type = "Ball";
		loc = QString("P:(%1, %2, %3) R:%4").arg(this->_xCenter).arg(this->_yCenter).arg(this->_zCenter).arg(this->_radius);
		info = QString("VIn:%1 VOut:%2 Tck:%3").arg(this->_vIn).arg(this->_vOut).arg(this->_thickness);

		list.append(type);
		list.append(loc);
		list.append(info);

		return list;
	}

	void Cylinder::appendFields(Py::PythonAagent* py)
	{
		int bg = 0;
		if (this->_backgroundField)
			bg = 1;
		
		QString  code = QString("gmsher.appendCylinderFields(%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11)")
			.arg(3).arg(this->_radius).arg(this->_vIn).arg(this->_vOut).arg(this->_xAxis)
			.arg(this->_xCenter + this->_offset[0]).arg(this->_yAxis).arg(this->_yCenter + this->_offset[1])
			.arg(this->_zAxis).arg(this->_zCenter + this->_offset[2]).arg(this->_backgroundField);

		py->submit(code);

	}

	QStringList Cylinder::getInformation()
	{
		QStringList list{};
		QString type, info, loc;
		type = "Cylinder";
		loc = QString("P:(%1, %2, %3) R:%4 L:%5").arg(this->_xCenter).arg(this->_yCenter).arg(this->_zCenter).arg(this->_radius).arg(this->_length);
		info = QString("VIn:%1 VOut:%2").arg(this->_vIn).arg(this->_vOut);

		list.append(type);
		list.append(loc);
		list.append(info);

		return list;
	}

}