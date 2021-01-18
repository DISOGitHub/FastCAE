#ifndef DIALOGAERODYNAMICL_H_
#define DIALOGAERODYNAMICL_H_

#include "AircraftPluginAPI.h"
#include <QDialog>
#include <QTableWidget>
class QTableWidget;

namespace Ui
{
	class DialogAerodynamic;
}

namespace Aircraft{


	class  CreateAerodynamicDialog :  public QDialog
	{
		Q_OBJECT
	public:
		CreateAerodynamicDialog( );
		~CreateAerodynamicDialog() = default;
	
	private:
		void init();
		void closeEvent(QCloseEvent *);
		void reject() override;
		void accept() override;
		void fillTab();

	private slots:
		void on_SaveButton_clicked();
		void on_ImportButton_clicked();

	private:

		Ui::DialogAerodynamic* _ui{};
		QTableWidget* _tableWidget;
		QStringList _headlist{};
		QList<QList<double>> _datalist{};

	};
}



#endif