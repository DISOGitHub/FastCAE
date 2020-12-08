#ifndef GEOCOMMANDIMPORT_H_
#define GEOCOMMANDIMPORT_H_

#include "GeoCommandBase.h"
#include "moduleBase/messageWindowBase.h"
//#include "geometry/GeometrySet.h"

namespace GUI
{
	class MainWindow;
}

namespace Command
{
	class GEOMETRYCOMMANDAPI GeoCommandImport : public GeoCommandBase
	{
		Q_OBJECT
	public:
		GeoCommandImport(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommandImport() = default;

		void setFileName(QStringList name);

		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

	signals:
		void showMessage(ModuleBase::Message);

	private:
		QStringList _fileNames{};
		QList<Geometry::GeometrySet*> _resSet{};
		GUI::MainWindow *_mw;		
	};
}



#endif
