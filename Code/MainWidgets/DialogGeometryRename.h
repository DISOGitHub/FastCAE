#ifndef GEOMETRYRENAMEDIALOG_H
#define GEOMETRYRENAMEDIALOG_H

#include "SelfDefObject/QFDialog.h"

namespace Ui
{
	class DialogRename;
}

namespace Geometry
{
	class GeometrySet;
	class GeoComponent;
}

namespace DataProperty
{
	class DataBase;
}

class QTreeWidgetItem;

namespace MainWidget
{
	class GeometryRenameDialog : public QFDialog
	{
		Q_OBJECT

	public:
		GeometryRenameDialog(GUI::MainWindow* m, DataProperty::DataBase* data, QTreeWidgetItem* item);
		~GeometryRenameDialog();
		
	private:
		void accept() override;

	signals:
		//更新属性
		void disPlayProp(DataProperty::DataBase* pops);

	private:
		Ui::DialogRename* _ui{};
		QTreeWidgetItem* _treeItem{};
		DataProperty::DataBase* _data{};
	};
}

#endif
