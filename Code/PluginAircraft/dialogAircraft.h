#ifndef DIALOGAIRCRAFT_H_
#define DIALOGAIRCRAFT_H_

#include <QDialog>
#include <QTableWidget>
class QTableWidget;

namespace Ui
{
	class DialogAircraft;
}

namespace Aircraft{


	class  CreateAircraftDialog :  public QDialog
	{
		Q_OBJECT
	public:
		CreateAircraftDialog();
		~CreateAircraftDialog() = default;
	
	private:
		void init();
		void closeEvent(QCloseEvent *);
		void reject() override;
		void accept() override;

	private:

		Ui::DialogAircraft* _ui{};
	};
}



#endif