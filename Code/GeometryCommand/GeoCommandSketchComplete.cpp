#include "GeoCommandSketchComplete.h"
#include "GeoCommandList.h"
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <TopoDS_Shape.hxx>
#include <QString>
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"


namespace Command
{
	GeoCommandSketchComplete::GeoCommandSketchComplete(GUI::MainWindow* m, MainWidget::PreWindow* p)
		:GeoCommandBase(m, p)
	{
	}

	bool GeoCommandSketchComplete::execute()
	{
		QList<GeoCommandBase*> clist = _commandList->getSketchUndoList();
		const int n = clist.size();
		if (n < 1) return false;

		TopoDS_Compound comp;
		BRep_Builder builder;
		builder.MakeCompound(comp);

		for (int i = 0; i < n; ++i)
		{
			auto c = clist.at(i);
			TopoDS_Shape* s = c->getResultShape();
			if (s == nullptr) continue;
			builder.Add(comp, *s);
		}
		
		TopoDS_Shape* shape = new TopoDS_Shape;
		*shape = comp;

		const int id = Geometry::GeometrySet::getMaxID() + 1;
		QString name = QString("Sketch_%1").arg(id);
		Geometry::GeometrySet* set = new Geometry::GeometrySet;
		set->setShape(shape);
		set->setName(name);
		_geoData->appendGeometrySet(set);
		_result = set;

		emit showSet(set);
		emit updateGeoTree();
		_commandList->clearSketchUndoCommands();
	
		return GeoCommandBase::execute();
	}

	void GeoCommandSketchComplete::undo()
	{
		_geoData->removeTopGeometrySet(_result);
		emit removeDisplayActor(_result);
		emit updateGeoTree();
	}

	void GeoCommandSketchComplete::redo()
	{
		_geoData->appendGeometrySet(_result);
		emit showSet(_result);
		emit updateGeoTree();
	}

	void GeoCommandSketchComplete::releaseResult()
	{
		if (_result != nullptr) delete _result;
	}


}