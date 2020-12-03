#ifndef LOCALFIELD_H
#define LOCALFIELD_H

#include <QString>
#include <QMultiHash>

enum LocalType{
	UnDefine = 0,
	PointSize,
	BoxField,
	BallField,
	CylinderField,
	SolidField,
	FrustumField,
};

namespace Py
{
	class PythonAagent;
}

namespace Gmsh{

	class LocalDensity
	{
	public:
		//python提交信息
		virtual void appendLocals(Py::PythonAagent* py) = 0;
		//获取区域信息
		virtual QStringList getInformation() = 0;
		//数据拷贝
		virtual void copy(LocalDensity* data);

		LocalType _type{ UnDefine };

		double _x{ 0 };
		double _y{ 0 };
		double _z{ 0 };

		double _thickness{ 0 };
		double _vIn{ 0 };
		double _vOut{ 0 };

		bool _backgroundField{ false };
	};


	class LocalPoint:public LocalDensity
	{
	public:
		LocalPoint() = default;
		~LocalPoint() = default;

		void appendLocals(Py::PythonAagent* py) override;
		QStringList getInformation() override;
		void copy(LocalDensity* data) override;
		
		double _value{ 0 };

	};

	class Box :public LocalDensity
	{
	public:
		Box() = default;

		void appendLocals(Py::PythonAagent* py) override;
		QStringList getInformation() override;
		void copy(LocalDensity* data) override;

		double _length{ 0 };
		double _width{ 0 };
		double _height{ 0 };
	};

	class Ball :public LocalDensity
	{
	public:
		Ball() = default;

		void appendLocals(Py::PythonAagent* py) override;
		QStringList getInformation() override;
		void copy(LocalDensity* data) override;

		double _radius{ 0 };
	};

	class Cylinder :public LocalDensity
	{
	public:
		void appendLocals(Py::PythonAagent* py) override;
		QStringList getInformation() override;
		void copy(LocalDensity* data) override;

		double _radius{ 0 };
		double _xAxis{ 0 };
		double _yAxis{ 0 };
		double _zAxis{ 0 };
		double _length{ 0 };
	};

	class SolidFields :public LocalDensity
	{
	public:
		void appendLocals(Py::PythonAagent* py) override;
		QStringList getInformation() override;
		void copy(LocalDensity* data) override;

		QMultiHash<int, int> _solidHash;
		void setIndex(int d);

	private:
		int _index{ 0 };
	};

	class Frustum :public LocalDensity
	{
	public:
		void appendLocals(Py::PythonAagent* py) override;
		QStringList getInformation() override;
		void copy(LocalDensity* data) override;

		double _iRadius{ 0 };
		double _oRadius{ 0 };
		double _iTopRadius{ 0 };
		double _oTopRadius{ 0 };
		double _xAxis{ 0 };
		double _yAxis{ 0 };
		double _zAxis{ 0 };
		double _length{ 0 };

	};
}

#endif // _DEBUG


