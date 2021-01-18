#ifndef GEOCPMMANDREMOVE_H_
#define GEOCPMMANDREMOVE_H_

#include "GeoCommandBase.h"

namespace Geometry
{
	class GeometrySet;
}

namespace Command
{
	class GEOMETRYCOMMANDAPI GeoCommandRemove : public GeoCommandBase
	{
		Q_OBJECT
	public:
		GeoCommandRemove(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommandRemove();

		void setIndex(int index);

		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

	private:
		Geometry::GeometrySet* _res{};
		int _index{ -1 };

		bool needRelease{ false };

	};
}



#endif