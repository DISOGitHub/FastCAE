#ifndef GEOCOMMANDFILLGAP_H_
#define GEOCOMMANDFILLGAP_H_

#include "GeoCommandBase.h"
#include <QList>
#include <QHash>

namespace Geometry
{
	class GeometrySet;
}

namespace Command
{

	class GEOMETRYCOMMANDAPI CommandFillGap : public GeoCommandBase
	{
	public:
		CommandFillGap(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~CommandFillGap() = default;

		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;
		
		void setEdges(QList<QPair<Geometry::GeometrySet*, int>> s);
		void setFaces(QList<QPair<Geometry::GeometrySet*, int>> s);
		void setSolids(QList<QPair<Geometry::GeometrySet*, int>> s);
		/*0:线;1:面;2:体;*/
		void setGapType(int i);
	private:
		TopoDS_Shape getGapShape(TopoDS_Shape shape1, TopoDS_Shape shape2);

	private:

		QList<QPair<Geometry::GeometrySet*, int>> _shapelist{};
		int _optionindex{};
		Geometry::GeometrySet* _result{};
	};

}




#endif