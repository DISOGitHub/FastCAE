#ifndef GEOCOMMANDSKETCHSPLINE_H_
#define GEOCOMMANDSKETCHSPLINE_H_

#include "GeoCommandSketchBase.h"

class gp_Pnt;

namespace Command
{
	class GEOMETRYCOMMANDAPI GeoCommanSketchSpline : public GeocommandSketchBase
	{
	public:
		GeoCommanSketchSpline(MainWidget::SketchViewProvider* view, GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommanSketchSpline() = default;

		bool execute()override;
		void undo() override;
		void redo() override;
// 		void isClose(bool c);
// 
// 	private:
// 		bool _isClose{ false };

	};


}



#endif