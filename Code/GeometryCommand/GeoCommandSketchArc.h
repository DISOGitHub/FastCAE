#ifndef GEOCOMMANDSKETCHARC_H_
#define GEOCOMMANDSKETCHARC_H_

#include "GeoCommandSketchBase.h"

class gp_Pnt;
//class gp_Dir;
namespace Command
{
	class GEOMETRYCOMMANDAPI GeoCommanSketchArc : public GeocommandSketchBase
	{
	public:
		GeoCommanSketchArc(MainWidget::SketchViewProvider* view, GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommanSketchArc() = default;

		bool execute()override;
		void undo() override;
		void redo() override;
	
	private:

	};


}

#endif