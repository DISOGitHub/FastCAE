#ifndef GEOCPMMANDREMOVEDATUM_H_
#define GEOCPMMANDREMOVEDATUM_H_

#include "GeoCommandBase.h"

namespace Command
{
	class GEOMETRYCOMMANDAPI GeoCommandRemoveDatum : public GeoCommandBase
	{
		Q_OBJECT
	public:
		GeoCommandRemoveDatum(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommandRemoveDatum();

		void setIndex(int index);

		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

	private:
		int _index{ -1 };
		Geometry::GeometryDatum* _removeOne{};
		bool needRelease{ false };


	};
}



#endif