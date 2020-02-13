#include "ProcessWindowBase.h"
#include "ui_ProcessWindowBase.h"
#include "mainWindow/mainWindow.h"
#include "moduleBase/processBar.h"

namespace ModuleBase
{

	ProcessWindowBase::ProcessWindowBase(GUI::MainWindow* mainwindow, QWidget* parent /* = 0 */) :
		DockWidgetBase(mainwindow, ModuleType::ProcessWindows)
	{
		_ui = new Ui::ProcessWindow();
		_ui->setupUi(this);
		connect(mainwindow, SIGNAL(addProcessBarSig(QWidget*)), this, SLOT(addProcess(QWidget*)));
		connect(mainwindow, SIGNAL(stopSolve(QWidget*)), this, SLOT(removeWidget(QWidget*)));
	}
	ProcessWindowBase::~ProcessWindowBase()
	{
		const int n = _processList.size();
		for (int i = 0; i < n; ++i)
		{
			QWidget*w = _processList.at(i);
			delete w;
		}
		_processList.clear();
		delete _ui;
		
	}

	void ProcessWindowBase::reTranslate()
	{
		_ui->retranslateUi(this);
		for (int i = 0; i < _processList.size(); ++i)
		{
			ModuleBase::ProcessBar* b = dynamic_cast<ModuleBase::ProcessBar*>(_processList.at(i));
			if (b != nullptr)
				b->reTranslate();
		}
		DockWidgetBase::reTranslate();
	}
	void ProcessWindowBase::addProcess(QWidget* w)
	{
		_ui->verticalLayout->addWidget(w);
		_processList.append(w);
		//connect((ProcessBar*)w, SIGNAL(processFinished(QWidget*)), this, SLOT(removeWidget(QWidget*)));
	}
	void ProcessWindowBase::removeWidget(QWidget* w)
	{
		_ui->verticalLayout->removeWidget(w);
		w->setParent(nullptr);
		if (_processList.contains(w))
		{
			int i = _processList.indexOf(w);
			_processList.removeAt(i);
		}

		delete w;
	}

}