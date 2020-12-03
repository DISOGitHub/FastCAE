#ifndef DIALOGMATERIAL_H_
#define DIALOGMATERIAL_H_

#include "Geant4PluginAPI.h"
#include <QDialog>
#include <QTableWidget>
class QTableWidget;

namespace Ui
{
	class DialogMaterial;
}

namespace Geant4{
	class MaterialManager;
	class GDMLElement;
	class GDMLMaterial;

	class  CreateMaterialDialog :  public QDialog
	{
		Q_OBJECT
	public:
		CreateMaterialDialog(GDMLMaterial* material );
		~CreateMaterialDialog() = default;
	
	private:
		void init();
		void closeEvent(QCloseEvent *);
		void reject() override;
		void accept() override;
		void updateOneMaterial();

		void fillDTPTableWidget(int nN);
		void fillFraTableWidget(int n);
		void fillComTableWidege(int n);

	private:

		Ui::DialogMaterial* _ui{};
		QTableWidget* DTP_tableWidget;
		QTableWidget* fra_tableWidget;
		QTableWidget* Com_tablewidege;
		MaterialManager* m_Material;

		GDMLMaterial* _material{};
	};
}



#endif