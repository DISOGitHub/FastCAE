#include "projectWindowBase.h"
#include <ui_projectWindowBase.h>
#include "moduleBase/ModelTreeWidgetBase.h"
#include <QObject>
#include "mainWindow/mainWindow.h"

namespace ModuleBase
{
	ProjectWindowBase::ProjectWindowBase(GUI::MainWindow *mainwindow) :
		DockWidgetBase(mainwindow, ModuleType::ProjectWindows)
	{
		_ui = new Ui::ProjectWindow();
		_ui->setupUi(this);
		Init();
	}

	ProjectWindowBase::~ProjectWindowBase()
	{
		delete _ui;
	}

	void ProjectWindowBase::Init()
	{
		QStringList lables;
		lables << tr("Name") << tr("Property");
		setHorizontalHeader(lables);
	}
	void ProjectWindowBase::addTreeWidget(ModelTreeWidgetBase* treewidget)
	{
		if (NULL == _ui)
			return;
		_ui->projectTab->addWidget(treewidget);
	}
	void ProjectWindowBase::setHorizontalHeader(const QString label1, const QString label2)
	{
		if (NULL == _ui)
			return;
		QStringList lables;
		lables << label1 << label2;
		_ui->propertyTable->setHorizontalHeaderLabels(lables);
	}
	void ProjectWindowBase::setHorizontalHeader(const QStringList _lables)
	{
		if (NULL == _ui)
			return;
		_ui->propertyTable->setHorizontalHeaderLabels(_lables);
	}

	void ProjectWindowBase::reTranslate()
	{
		if (NULL == _ui)
			return;
		_ui->retranslateUi(this);
		setHorizontalHeader( tr("Name"), tr("Property"));
		_modelTreeWidget->updateTree();
	}
	void ProjectWindowBase::on_TreeMouseEvent(int evevntType, QTreeWidgetItem* item, int proID)
	{

	}
}
