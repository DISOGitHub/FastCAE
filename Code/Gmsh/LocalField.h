#ifndef LOCALFIELD_H
#define LOCALFIELD_H

#include <QString>

enum FieldType{
	UnDefine = 0,
	BoxField,
	BallField,
};

namespace Py
{
	class PythonAagent;
}

namespace Gmsh{
	class LocalField
	{
	public:
		LocalField();
		~LocalField();

		virtual void appendFields(Py::PythonAagent* py) = 0;
		virtual QStringList getInformation() = 0;

		FieldType _type{ UnDefine };

		double _thickness{ 0 };
		double _vIn{ 0 };
		double _vOut{ 0 };

		bool _backgroundField{ false };
	};

	class Box:public LocalField
	{
	public:
		Box() = default;

		void appendFields(Py::PythonAagent* py) override;
		QStringList getInformation() override;

		double _xMin{ 0 };
		double _xMax{ 0 };
		double _yMin{ 0 };
		double _yMax{ 0 };
		double _zMin{ 0 };
		double _zMax{ 0 };
	};

	class Ball:public LocalField
	{
	public:
		Ball() = default;

		void appendFields(Py::PythonAagent* py) override;
		QStringList getInformation() override;

		double _radius{ 0 };
		double _xCenter{ 0 };
		double _yCenter{ 0 };
		double _zCenter{ 0 };
	};

	class Cylinder :public LocalField
	{
	public:
		void appendFields(Py::PythonAagent* py) override;
		QStringList getInformation() override;

		double _radius{ 0 };
		double _xCenter{ 0 };
		double _yCenter{ 0 };
		double _zCenter{ 0 };
		double _xAxis{ 0 };
		double _yAxis{ 0 };
		double _zAxis{ 0 };
		double _length{ 0 };
		double _offset[3];
	};
}

#endif // _DEBUG


