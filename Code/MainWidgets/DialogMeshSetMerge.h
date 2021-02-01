#ifndef _DIALOGMERGEMESHSET_H_
#define _DIALOGMERGEMESHSET_H_

#include "SelfDefObject/QFDialog.h"

namespace Ui
{
	class MeshSetMergeDialog;
}
namespace MeshData
{
	class MeshData;
};


namespace MainWidget
{
	class MeshWidget;

	class MeshSetMergeDialog :  public QFDialog
	{
		Q_OBJECT
	public:
		MeshSetMergeDialog(GUI::MainWindow* mw, MeshWidget* w);
		~MeshSetMergeDialog();

	private slots:
		void on_typeChanged();

		void on_addButton_clicked();
		void on_removeButton_clicked();
		void on_btn_add_clicked();
		void on_btn_del_clicked();


	private:
		void init();
		QString getNameFromUi();
		QList<int> getMergeIDs();
		QList<int> getCutIDs();

		void accept() override;

	private:
		Ui::MeshSetMergeDialog* _ui{};
		MeshData::MeshData* _meshData{};
		MeshWidget* _meshwidget{};
	};

}



#endif