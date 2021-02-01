#ifndef GEOCOMMANDSKETCHCOMPLATE_H_
#define GEOCOMMANDSKETCHCOMPLATE_H_

#include "GeoCommandBase.h"

namespace Geometry
{
	class GeometrySet;
}

namespace Command
{
	class GEOMETRYCOMMANDAPI GeoCommandSketchComplete : public GeoCommandBase
	{
		Q_OBJECT
	public:
		GeoCommandSketchComplete(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommandSketchComplete() = default;

		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

	private:
		Geometry::GeometrySet* _result{};
	};


}


#endif