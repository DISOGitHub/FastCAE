#ifndef _PROCESSBAR_H_
#define _PROCESSBAR_H_

#include "moduleBaseAPI.h"
#include <QWidget>

namespace Ui
{
	class ProcessBar;
}
namespace GUI
{
	class MainWindow;
}

namespace ModuleBase
{
	class ThreadTask;

	class MODULEBASEAPI ProcessBar : public QWidget
	{
		Q_OBJECT
	public:
		ProcessBar(GUI::MainWindow* mainWindow, ThreadTask* task, bool autoclose = true);
		ProcessBar(GUI::MainWindow* mw, QString name, bool autoclose = true);
		~ProcessBar();

		void setProcess(int d);
		void setAutoClose(bool autoclose);
		bool isAutoClose();
		void reTranslate();
		void setProcessRange(int min, int max);
		void setBusy();
		bool isBusy();
		void buttonVisible(bool on);

		void setInformation(QString s);



	signals:
		void closeProcess(QWidget*);
		void closeThread();

	private slots:
//		void peocessChanged(int p);
		void on_closeButton_clicked();
		void onAutoCloseQCBClicked();

// 	protected:
// 		void closeEvent(QCloseEvent *);

	private:
		Ui::ProcessBar* _ui{};
		GUI::MainWindow* _mainWindow{};
		bool _autoClose{ false };
		bool _isBusy{ false };


		ThreadTask* _task{ nullptr };
	};
}

#endif
