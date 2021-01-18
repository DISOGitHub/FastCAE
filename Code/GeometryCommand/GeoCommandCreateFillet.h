#ifndef GEOCOMMANDCREATEFILLET_H_
#define GEOCOMMANDCREATEFILLET_H_


#include "geometryCommandAPI.h"
#include "GeoCommandBase.h"
#include <QString>
#include <QMultiHash>

namespace Geometry
{
	class GeometrySet;
}

class TopoDS_Shape;

namespace Command
{
	class GEOMETRYCOMMANDAPI CommandCreateFillet : public GeoCommandBase
	{
		Q_OBJECT
	public:
		CommandCreateFillet(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~CommandCreateFillet() = default;

		void setShapeList(QMultiHash<Geometry::GeometrySet*, int> s);
		void setRadius(double d);

		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;


	private:
		QString _name{"Fillet_%1"};
//		Geometry::GeometrySet* _result{};
		QMultiHash<Geometry::GeometrySet*, int> _shapeHash{};
		double _radius{ 0.0 };

		QHash<Geometry::GeometrySet*, Geometry::GeometrySet*> _inputOutputHash{};
	};
}


#endif