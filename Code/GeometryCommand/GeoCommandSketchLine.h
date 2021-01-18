#ifndef GEOCOMMANDSKETCHLINE_H_
#define GEOCOMMANDSKETCHLINE_H_

#include "GeoCommandSketchBase.h"

class gp_Pnt;

namespace Command
{
	class GEOMETRYCOMMANDAPI GeoCommanSketchLine : public GeocommandSketchBase
	{
	public:
		GeoCommanSketchLine(MainWidget::SketchViewProvider* view, GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommanSketchLine() = default;

		bool execute()override;
		void undo() override;
		void redo() override;

	};


}

#endif