#ifndef DIALOGREMOVEMATERIAL_H_
#define DIALOGREMOVEMATERIAL_H_

#include "SelfDefObject/QFDialog.h"
#include <QHash>

class QListWidgetItem;

namespace Ui
{
	class RemoveMaterialDialog;
}

namespace GUI
{
	class MainWindow;
}

namespace Material
{
	class  Material;

	class  RemoveMaterialDialog : public QFDialog
	{
		Q_OBJECT
	public:
		RemoveMaterialDialog(GUI::MainWindow* m, QHash<QString, Material*>* ms);
		~RemoveMaterialDialog();

	private:
		void init();
		bool isAllChecked();
		bool isAllUnChecked();

	private slots:
	    void on_cancelPushButton_clicked();
		void on_removePushButton_clicked();
		void on_selectAll();
		void on_clearSelect();
		void itemChanged(QListWidgetItem *  item);

	private:
		Ui::RemoveMaterialDialog* _ui{};
		QHash<QString, Material*>* _data{};
	};
}



#endif