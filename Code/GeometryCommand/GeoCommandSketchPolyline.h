#ifndef GEOCOMMANDSKETCHPOLYLINE_H_
#define GEOCOMMANDSKETCHPOLYLINE_H_

#include "GeoCommandSketchBase.h"

class gp_Pnt;
//class gp_Dir;
namespace Command
{
	class GEOMETRYCOMMANDAPI GeoCommanSketchPolyline : public GeocommandSketchBase
	{
	public:
		GeoCommanSketchPolyline(MainWidget::SketchViewProvider* view, GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommanSketchPolyline() = default;

		bool execute()override;
		void undo() override;
		void redo() override;
		void isClose(bool c);

	private:
		bool _isClose{ false }; 
	
	};


}

#endif