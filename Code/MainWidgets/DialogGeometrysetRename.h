#ifndef GEOMETRYSETRENAMEDIALOG_H
#define GEOMETRYSETRENAMEDIALOG_H

#include "SelfDefObject/QFDialog.h"

namespace Ui
{
	class DialogSetRename;
}
namespace Geometry
{
	class GeometrySet;
}

class QTreeWidgetItem;

namespace MainWidget
{
	
	class GeometrysetRenameDialog : public QFDialog
	{
		Q_OBJECT

	public:
		GeometrysetRenameDialog(GUI::MainWindow* m, Geometry::GeometrySet* gset, QTreeWidgetItem* item);
		~GeometrysetRenameDialog();
		
	private:
		void accept() override;

	private:
		Ui::DialogSetRename* _ui{};
		Geometry::GeometrySet* _gset;
		QTreeWidgetItem* _treeItem{};
	};
}

#endif
