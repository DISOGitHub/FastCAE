#ifndef _DIALOGLOADMATERIAL_H_
#define _DIALOGLOADMATERIAL_H_

#include "SelfDefObject/QFDialog.h"
#include <QHash>
#include <QStringList>

namespace GUI
{
	class MainWindow;
}
namespace Ui
{
	class LoadMaterialDialog;
}


namespace Material
{
	class Material;

	class LoadMaterialDialog : public QFDialog
	{
		Q_OBJECT

	public:
		LoadMaterialDialog(GUI::MainWindow* m, QHash<QString, Material*> ms);
		~LoadMaterialDialog();

	private:
		void init();
		void accept() override;
		QStringList getSelectedMaterials();

	private slots:
		virtual void on_addButton_clicked();
		virtual void on_removeButton_clicked();

	private:
		Ui::LoadMaterialDialog* _ui{};
		QHash<QString, Material*> _maHash{};

	};

}


#endif
