#include "processBar.h"
#include "ui_processBar.h"
#include "mainWindow/mainWindow.h"
#include "ThreadTask.h"
#include "ProcessWindowBase.h"
#include <QDebug>
#include <QThread>

namespace ModuleBase
{
	ProcessBar::ProcessBar(GUI::MainWindow* mainWindow, ThreadTask* task, bool autoclose)
		:_ui(new Ui::ProcessBar), _task(task), _autoClose(autoclose), _mainWindow(mainWindow)
	{
		_ui->setupUi(this);
		_ui->closeButton->setVisible(false);
		_ui->autoCloseQCB->setText("Automatically close when finished");
		this->setBusy();  //默认繁忙模式
		setProcess(0);
		if (_autoClose)
		{
			_ui->closeButton->setVisible(false);
			_ui->autoCloseQCB->setVisible(false);
		}
		
		connect(this, SIGNAL(closeProcess(QWidget*)), _mainWindow, SIGNAL(stopSolve(QWidget*)));
		connect(_task, &ThreadTask::showInformation, this, &ProcessBar::setInformation);
		connect(_task, &ThreadTask::setFinishedStatus, this, &ProcessBar::setProcess);
		connect(_task, &ThreadTask::setRange, this, &ProcessBar::setProcessRange);
		connect(_ui->autoCloseQCB, SIGNAL(clicked()), this, SLOT(onAutoCloseQCBClicked()));
		connect(_task, &ThreadTask::showButton, this, &ProcessBar::buttonVisible);
	}

	ProcessBar::ProcessBar(GUI::MainWindow* mainWindow, QString name, bool a) 
		:_ui(new Ui::ProcessBar), _mainWindow(mainWindow), _autoClose(a)
	{
		_ui->setupUi(this);
		_ui->name->setText(name);
		this->setBusy();  //默认繁忙模式
		setProcess(0);
//		if (_autoClose) _ui->closeButton->setVisible(false);
		_ui->autoCloseQCB->setVisible(false);
		connect(this, SIGNAL(closeProcess(QWidget*)), _mainWindow, SIGNAL(stopSolve(QWidget*)));
	}
	ProcessBar::~ProcessBar()
	{
		if (_ui != nullptr)
			delete _ui;
		_ui = nullptr;

// 		if (_task != nullptr)
// 		{
// 			_task->destroyThread();
// 		}
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
		/*delete this;*/
	}

	void ProcessBar::onAutoCloseQCBClicked()
	{
		if (_ui->autoCloseQCB->isChecked())
		{
			_autoClose = true;
		}
		else
		{
			_autoClose = false;
		}
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

	void ProcessBar::setInformation(QString s)
	{
		_ui->name->setText(s);
	}

}
