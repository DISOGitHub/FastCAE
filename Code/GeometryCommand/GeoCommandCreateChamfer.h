#ifndef GEOCOMMANDCREATECHAMFER_H_
#define GEOCOMMANDCREATECHAMFER_H_


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
	class GEOMETRYCOMMANDAPI CommandCreateChamfer : public GeoCommandBase
	{
		Q_OBJECT
	public:
		CommandCreateChamfer(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~CommandCreateChamfer() = default;

		void setShapeList(QMultiHash<Geometry::GeometrySet*, int> s);
		void setDistance(double d1 ,double d2);
		void setSymmetrical(bool is);

		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;


	private:
		QString _name{"Chamfer_%1"};
//		Geometry::GeometrySet* _result{};
		QMultiHash<Geometry::GeometrySet*, int> _shapeHash{};
		double _distance1{ 0.0 };
		double _distance2{ 0.0 };
		bool _symmetrical{ true };

		QHash<Geometry::GeometrySet*, Geometry::GeometrySet*> _inputOutputHash{};
	};
}


#endif