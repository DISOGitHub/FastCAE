#ifndef DIALOGDISOTOPE_H_
#define DIALOGDISOTOPE_H_

#include "Geant4PluginAPI.h"
#include <QDialog>

class QTableWidget;

namespace Ui
{
	class DialogIsotope;
}

namespace Geant4{
	class MaterialManager;
	class GDMLIsotope; 

	class  CreateIsotopeDialog :  public QDialog
	{
		Q_OBJECT
	public:
		CreateIsotopeDialog(GDMLIsotope*ele);
		~CreateIsotopeDialog()=default;
	
	private:
		void init();
		void closeEvent(QCloseEvent *);
		void reject() override;
		void accept() override;
		void updateOneIsotope();



	private:
		Ui::DialogIsotope* _ui{};
		QTableWidget* m_tableWidget;
		GDMLIsotope*_oneIsotope;
	};
}



#endif