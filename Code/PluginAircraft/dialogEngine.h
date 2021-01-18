#ifndef DIALOGENGINE_H_
#define DIALOGENGINE_H_

#include <QDialog>
#include <QTableWidget>
class QTableWidget;

namespace Ui
{
	class DialogEngine;
}

namespace Aircraft{


	class  CreateEngineDialog :  public QDialog
	{
		Q_OBJECT
	public:
		CreateEngineDialog();
		~CreateEngineDialog() = default;
	
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

		Ui::DialogEngine* _ui{};
		QTableWidget* _tableWidget;
		QStringList _headlist{};
		QList<QList<double>> _datalist{};
	};
}



#endif