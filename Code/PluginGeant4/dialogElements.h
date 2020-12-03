#ifndef DIALOGDELEMENTS_H_
#define DIALOGDELEMENTS_H_

#include "Geant4PluginAPI.h"
#include <QDialog>

class QTableWidget;

namespace Ui
{
	class DialogElement;
}

namespace Geant4{
	class MaterialManager;
	class GDMLElement;

	class  CreateElementsDialog :  public QDialog
	{
		Q_OBJECT
	public:
		CreateElementsDialog(GDMLElement*ele);
		~CreateElementsDialog()=default;
	
	private:
		void init();
		void closeEvent(QCloseEvent *);
		void reject() override;
		void accept() override;
		void updateOneElement();
		void fillTableWidget(int n);

	private:
		Ui::DialogElement* _ui{};
		QTableWidget* m_tableWidget;
		GDMLElement*_oneelement;
	};
}



#endif