#include "EditorSolverManager.h"
#include "ui_EditorSolverManager.h"
#include "EditorSolverValue.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/SolverConfig.h"
#include "ConfigOptions/SolverInfo.h"
#include <QCoreApplication>
#include <QFile>
#include <QDomElement>
#include <QDomDocument>
#include <QTextStream>
#include <QDebug>

namespace FastCAEDesigner
{
	EditorSolverManager::EditorSolverManager(GUI::MainWindow* mw) : /*QFDialog(mw),*/
		_ui(new Ui::EditorSolverManager), _mainWindow(mw)
	{
		_ui->setupUi(this);
		connect(_ui->solverList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(listItemClicked(QListWidgetItem*)));
		_solvers = ConfigOption::ConfigOption::getInstance()->getSolverOption();
		qDebug() << _solvers->getMesherCount();
		update();
	}
	EditorSolverManager::~EditorSolverManager()
	{
		//this->writeConfig();
		delete _ui;
	}
	
	void EditorSolverManager::on_out_AddSolverButton_clicked()
	{
		EditorSolverValue dlg(_mainWindow, this, nullptr);
		dlg.exec();
	}
	
	void EditorSolverManager::on_out_RemoveButton_clicked()
	{
		QListWidgetItem* item = _ui->solverList->currentItem();
		if (item == nullptr) return;

		bool ok = false;
		int index = item->data(Qt::UserRole).toInt(&ok);
		if (!ok) return;
		_solvers->removeSolver(index);
		this->update();
	}

	void EditorSolverManager::update()
	{
		_ui -> solverList->clear();

		const int nsolver = _solvers->getSolverCount();
		for (int i = 0; i < nsolver; ++i)
		{
			ConfigOption::SolverInfo* info = _solvers->getSolverAt(i);
			QString name = info->getName();
			QListWidgetItem *item = new QListWidgetItem(name);
			item->setData(Qt::UserRole, i);
			_ui->solverList->addItem(item);
		}
	}

	void EditorSolverManager::on_out_ModifyButton_clicked()
	{
		QListWidgetItem* item = _ui->solverList->currentItem();
		if (item == nullptr) return;

		bool ok = false;
		int index = item->data(Qt::UserRole).toInt(&ok);
		if (!ok) return;
		ConfigOption::SolverInfo* info = _solvers->getSolverAt(index);
		if (info == nullptr) return;
		EditorSolverValue dlg(_mainWindow, this, info);
		dlg.exec();



	}

	void EditorSolverManager::writeConfig()
	{
		QString exePath = qApp->applicationDirPath();
		QString configPath = exePath + "/../ConfigFiles/";
		if (_solvers == nullptr)
			return;
		_solvers->write(configPath + "SolverConfig.config");
	}

	void EditorSolverManager::setDependencyFiles(QStringList list)
	{
		_dependencyFiles.append(list);
	}

	QList<QStringList> EditorSolverManager::getDependecyFiles()
	{
		return _dependencyFiles;
	}


}