#ifndef GEOCOMMANDCREATECYLINDER_H_
#define GEOCOMMANDCREATECYLINDER_H_

#include "geometryCommandAPI.h"
#include "GeoCommandBase.h"
#include <QString>

namespace Geometry
{
	class GeometrySet;
}

namespace Command
{
	class GEOMETRYCOMMANDAPI GeoCommandCreateCylinder : public GeoCommandBase
	{
	public:
		GeoCommandCreateCylinder(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommandCreateCylinder() = default;

		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

		void setName(QString name);
		void setLocation(double* loc);
		void setAxis(double* axis);
		void setRadius(double r);
		void setLength(double l);

	private:
		QString _name{};
		double _location[3];
		double _axis[3];
		double _radius{ 0.0 };
		double _length{ 0.0 };

		Geometry::GeometrySet* _result{};

	};
}



#endif