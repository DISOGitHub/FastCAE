//*******************************
//报告显示窗口
//libaojun
//*******************************
#ifndef REPORTWINDOW_H_
#define REPORTWINDOW_H_

#include "xgeneratereport_global.h"
#include <QWidget>
#include <QString>

class QAxWidget;
namespace GUI
{
	class MainWindow;
}
namespace Ui
{
	class ReportWindow;
}

namespace XReport
{
	class XGENERATEREPORT_EXPORT ReportWindow : public QWidget
	{
		Q_OBJECT
	public:
		ReportWindow(GUI::MainWindow* mainwindow, QString file);
		~ReportWindow();
		//语言切换
		virtual void reTranslate();

	signals:
		void closeThisWindow(XReport::ReportWindow* w);

	private:
		void open();
		void closeDoc();
		void resizeEvent(QResizeEvent *e) override;
		void closeEvent(QCloseEvent* e) override;

	private:
		GUI::MainWindow* _mainWindow{};
		Ui::ReportWindow* _ui{};
		QString _docFile{};
		QAxWidget* _officeCon{};
	};
}


#endif