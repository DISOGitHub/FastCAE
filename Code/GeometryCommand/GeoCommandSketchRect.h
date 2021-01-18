#ifndef GEOCOMMANDSKETCHRECT_H_
#define GEOCOMMANDSKETCHRECT_H_

#include "GeoCommandSketchBase.h"

class gp_Pnt;

namespace Command
{
	class GEOMETRYCOMMANDAPI GeoCommanSketchRect : public GeocommandSketchBase
	{
	public:
		GeoCommanSketchRect(MainWidget::SketchViewProvider* view, GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommanSketchRect() = default;

		bool execute()override;
		void undo()override;
		void redo() override;
	private:
		//Geometry::GeometryData* _geoData{};
	};


}

#endif