#include "ReportWindow.h"
#include "ui_ReportWindow.h"
#include "mainWindow/mainWindow.h"
#include <QAxWidget>

namespace XReport
{
	ReportWindow::ReportWindow(GUI::MainWindow* mainwindow, QString file) :
		_mainWindow(mainwindow), _ui(new Ui::ReportWindow), _docFile(file)
	{
		_ui->setupUi(this);
		connect(this,SIGNAL(closeThisWindow(XReport::ReportWindow*)), mainwindow, SIGNAL(closeReportWindowSig(XReport::ReportWindow*)));
		open();
	}
	ReportWindow::~ReportWindow()
	{
		if (_ui != nullptr) delete _ui;
		closeDoc();
	}
	void ReportWindow::open()
	{
		_officeCon = new QAxWidget("Word.Application", this);
		_officeCon->dynamicCall("SetVisible (bool Visible)", "false");//²»ÏÔÊ¾´°Ìå
		_officeCon->setProperty("DisplayAlerts", false);
		auto rect = _ui->OfficeWidget->geometry();
		_officeCon->setGeometry(rect);
		_officeCon->setControl(_docFile);
		_officeCon->show();
	}
	void ReportWindow::resizeEvent(QResizeEvent *e)
	{
		auto rect = _ui->OfficeWidget->geometry();
		if (_officeCon != nullptr)
			_officeCon->setGeometry(rect);
		QWidget::resizeEvent(e);
	}
	void ReportWindow::closeDoc()
	{
		if (_officeCon != nullptr)
		{
			_officeCon->close();
			_officeCon->clear();
			delete _officeCon;
			_officeCon = nullptr;
		}
	}
	void ReportWindow::closeEvent(QCloseEvent* e)
	{
		emit closeThisWindow(this);
		QWidget::closeEvent(e);
	}
	void ReportWindow::reTranslate()
	{
		_ui->retranslateUi(this);
	}
	

}