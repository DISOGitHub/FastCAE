#ifndef CREATESPHERECOMMAND_H_
#define CREATESPHERECOMMAND_H_

#include "GeoCommandBase.h"
#include <QString>

namespace Geometry
{
	class GeometrySet;
}

namespace Command
{
	class GEOMETRYCOMMANDAPI GeoCommandCreateSphere : public GeoCommandBase
	{
	public: 
		GeoCommandCreateSphere(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommandCreateSphere() = default;

		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

		void setLocation(double* loc);
		void setRadius(double r);
		void setName(QString name);

	private:
		QString _name{};
		double _location[3];
		double _radius{ 0.0 };

		Geometry::GeometrySet* _result{};
	};
}

#endif