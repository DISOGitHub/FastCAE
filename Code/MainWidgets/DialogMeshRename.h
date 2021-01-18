#ifndef DIALOGMESHRENAME_H__
#define DIALOGMESHRENAME_H__

#include "SelfDefObject/QFDialog.h"
#include "DataProperty/modelTreeItemType.h"

namespace Ui
{
	class DialogRename;
}
namespace GUI
{
	class MainWindow;
}
namespace MeshData
{
	class MeshData;
}
namespace DataProperty
{
	class DataBase;
}

class QTreeWidgetItem;

namespace MainWidget
{
	class MeshRenameDialog : public QFDialog
	{
		Q_OBJECT
	public:
		MeshRenameDialog(GUI::MainWindow* mw, QTreeWidgetItem* item);
		~MeshRenameDialog();

	private:
		void accept() override;

	signals:
		//更新属性
		void disPlayProp(DataProperty::DataBase* pops);

	private:
		Ui::DialogRename* _ui{};
		QTreeWidgetItem* _item{};

		MeshData::MeshData* _meshdata{};
		TreeItemType _type{};
		DataProperty::DataBase* _data{};

	};
}



#endif // !DIALOGMESHRENAME_H__
