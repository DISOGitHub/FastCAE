#include "processBar.h"
#include "ui_processBar.h"
#include "mainWindow/mainWindow.h"

namespace ModuleBase
{
	ProcessBar::ProcessBar(GUI::MainWindow* mainWindow, QString name, bool a) 
		:_ui(new Ui::ProcessBar), _mainWindow(mainWindow), _autoClose(a)
	{
		_ui->setupUi(this);
		_ui->name->setText(name);
		this->setBusy();  //Ĭ�Ϸ�æģʽ
		setProcess(0);
//		if (_autoClose) _ui->closeButton->setVisible(false);
		connect(this, SIGNAL(closeProcess(QWidget*)), _mainWindow, SIGNAL(stopSolve(QWidget*)));
	}
	ProcessBar::~ProcessBar()
	{
		delete _ui;
	}
	void ProcessBar::setProcess(int d)
	{
		_ui->progressBar->setValue(d);
		if (_autoClose)
		{
			if (d >= 100) emit closeProcess(this);
		}
	}
	void ProcessBar::on_closeButton_clicked()
	{
		emit closeProcess(this);
	}
	void ProcessBar::setAutoClose(bool autoclose)
	{
		_autoClose = autoclose;
// 		if (_autoClose) _ui->closeButton->setVisible(false);
// 		else _ui->closeButton->setVisible(true);
	}
	bool ProcessBar::isAutoClose()
	{
		return _autoClose;
	}
	void ProcessBar::reTranslate()
	{
		_ui->retranslateUi(this);
	}

	void ProcessBar::setProcessRange(int min, int max)
	{
		_ui->progressBar->setMinimum(min);
		_ui->progressBar->setMaximum(max);
		if (min == 0 && max == 0)
			_isBusy = true;
		else
			_isBusy = false;
	}

	void ProcessBar::setBusy()
	{
		this->setProcessRange(0, 0);
		
	}

	bool ProcessBar::isBusy()
	{
		return  _isBusy;
	}

	void ProcessBar::buttonVisible(bool on)
	{
		_ui->closeButton->setVisible(on);

	}

}
