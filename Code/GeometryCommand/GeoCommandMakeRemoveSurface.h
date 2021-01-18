#ifndef GEOCOMMANDMAKEREMOVESURFACE_H_
#define GEOCOMMANDMAKEREMOVESURFACE_H_


#include "geometryCommandAPI.h"
#include "GeoCommandBase.h"
#include <QString>
#include <QMultiHash>
#include <QHash>

namespace Geometry
{
	class GeometrySet;
}

class TopoDS_Shape;

namespace Command
{
	class GEOMETRYCOMMANDAPI CommandMakeRemoveSurface : public GeoCommandBase
	{
		Q_OBJECT
	public:
		CommandMakeRemoveSurface(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~CommandMakeRemoveSurface() = default;
		
		void setShapeList(QMultiHash<Geometry::GeometrySet*, int> s);
		
		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;
		
		
		
	private:

		QString _name{"RemoveSurface_%1"};
		QMultiHash<Geometry::GeometrySet*, int> _shapeHash{};
		QHash<Geometry::GeometrySet*, Geometry::GeometrySet*> _inputOutputHash{};
	};
}


#endif