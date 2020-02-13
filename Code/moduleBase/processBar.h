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
	class MODULEBASEAPI ProcessBar : public QWidget
	{
		Q_OBJECT
	public:
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

	signals:
		void closeProcess(QWidget*);

	private slots:
//		void peocessChanged(int p);
		void on_closeButton_clicked();

	private:
		Ui::ProcessBar* _ui{};
		GUI::MainWindow* _mainWindow{};
		bool _autoClose{ false };
		bool _isBusy{ false };
	};
}

#endif
