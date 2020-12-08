#ifndef GEOCOMMANDMAKELOFT_H_
#define GEOCOMMANDMAKELOFT_H_

#include "geometryCommandAPI.h"
#include "GeoCommandBase.h"
#include <QMultiHash>
#include <QHash>
#include "moduleBase/ModuleType.h"
#include <TopoDS_Edge.hxx>
#include<TopoDS_Wire.hxx>


namespace Geometry
{
	class GeometrySet;
}
namespace Command
{
	class GEOMETRYCOMMANDAPI GeoCommandMakeLoft : public GeoCommandBase
	{
	public:
		GeoCommandMakeLoft(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommandMakeLoft() = default;

		bool execute()override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

		void setShapeList(QList<QMultiHash<Geometry::GeometrySet*, int>> s);
		void setName(QString name);
		void isSolid(bool s);
		
	private:
		std::list<TopoDS_Edge> bulidEdgeList(QMultiHash<Geometry::GeometrySet*, int> s);

	private:
		QList<QMultiHash<Geometry::GeometrySet*, int>> _shapeHash{};
		bool _isSolid{false};
		QString _name{};

		Geometry::GeometrySet* _result{};


	};














}
























#endif
