#ifndef GEOCOMMANDSKETCHCIRCLE_H_
#define GEOCOMMANDSKETCHCIRCLE_H_

#include "GeoCommandSketchBase.h"

class gp_Pnt;
//class gp_Dir;
namespace Command
{
	class GEOMETRYCOMMANDAPI GeoCommanSketchCircle : public GeocommandSketchBase
	{
	public:
		GeoCommanSketchCircle(MainWidget::SketchViewProvider* view, GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommanSketchCircle() = default;

		bool execute()override;
		void undo() override;
		void redo() override;

	};


}

#endif