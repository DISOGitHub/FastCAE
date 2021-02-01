#ifndef GEOCOMMANDMAKESWEEP_H_
#define GEOCOMMANDMAKESWEEP_H_

#include "GeoCommandBase.h"
#include <QMultiHash>
#include <QPair>

namespace Geometry
{
	class GeometrySet;
}

namespace Command
{
	class GEOMETRYCOMMANDAPI GeoCommandMakeSweep : public GeoCommandBase
	{
	public:
		GeoCommandMakeSweep(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommandMakeSweep() = default;

		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

		void setSection(QMultiHash<Geometry::GeometrySet*, int> s);
		void setPath(QPair<Geometry::GeometrySet*, int> p);
		void isSolid(bool is);

	private:
		QMultiHash<Geometry::GeometrySet*, int> _sectionEdge{};
		QPair<Geometry::GeometrySet*, int> _pathEdge{};
		bool _isSolid{ false };

		Geometry::GeometrySet* _result{};
	};
}



#endif