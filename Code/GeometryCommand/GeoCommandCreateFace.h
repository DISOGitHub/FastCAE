#ifndef GEOCOMMANDCREATEFACE_H_
#define GEOCOMMANDCREATEFACE_H_


#include "geometryCommandAPI.h"
#include "GeoCommandBase.h"
#include <QString>
#include <QMultiHash>
#include <QHash>

namespace Geometry
{
	class GeometrySet;
}
class vtkActor;
class TopoDS_Shape;

namespace Command
{
	class GEOMETRYCOMMANDAPI CommandCreateFace : public GeoCommandBase
	{
		Q_OBJECT
	public:
		CommandCreateFace(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~CommandCreateFace() = default;

		void setShapeList(QMultiHash<Geometry::GeometrySet*, int> s);
		void setActor(QList<vtkActor*> ac);
		void setName(QString name);

		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;


	private:
		QString _name{};
		QMultiHash<Geometry::GeometrySet*, int> _shapeHash{};
		QList<vtkActor*> _actor;
		Geometry::GeometrySet* _result{};
	};
}


#endif