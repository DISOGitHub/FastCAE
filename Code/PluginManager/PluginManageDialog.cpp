#include <ui_PluginManageDialog.h>
#include "PluginManageDialog.h"
#include "mainWindow/mainWindow.h"
#include "PluginManager.h"
#include "settings/busAPI.h"
#include <QDir>
#include <QMessageBox>

namespace Plugins
{
	PluginManageDialog::PluginManageDialog(GUI::MainWindow* mw, PluginManager * m)
		:_manager(m), _mainWindow(mw)
	{
		_ui = new Ui::PluginManageDialog;
		_ui->setupUi(this);
		_ui->availableList->setSelectionMode(QAbstractItemView::ExtendedSelection);
		_ui->selectedList->setSelectionMode(QAbstractItemView::ExtendedSelection);
		this->init();
		connect(this, SIGNAL(updateMainWindowInteface()), mw, SIGNAL(updateInterfaces()));
	}

	PluginManageDialog::~PluginManageDialog()
	{
		if (_ui != nullptr)
			delete _ui;
	}

	void PluginManageDialog::init()
	{
		QStringList insplugs = Setting::BusAPI::instance()->getPlugins();
		for (int i = 0; i < insplugs.size(); ++i)
		{
			QString name = insplugs.at(i);
			this->appendItemToSelectedList(name);
		}
		
		QString suffix = "dll";
		QString pre = "plugin";
#ifdef Q_OS_LINUX
		suffix = "so";
		pre = "libplugin";
#endif
		const QString plugdir = QApplication::applicationDirPath() + "/plugins/";
		QDir dir(plugdir);
		dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
		QFileInfoList list = dir.entryInfoList();
		for (int i = 0; i < list.size(); ++i)
		{
			QFileInfo fileInfo = list.at(i);
			QString plugpath = fileInfo.absoluteFilePath();
			QString pluginname = fileInfo.fileName();
			if (insplugs.contains(pluginname)) continue;
			if (!pluginname.toLower().startsWith(pre)) continue;
			if (!pluginname.toLower().endsWith(suffix)) continue;
			this->appendItemToAvailableList(pluginname);
		}
	}


	void PluginManageDialog::on_addButton_clicked()
	{
		int iSelectListItemNumber = _ui->selectedList->count();

		QList<QListWidgetItem*> itemList = _ui->availableList->selectedItems();
		const int n = itemList.size();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem* availitem = itemList.at(i);
			const int row = _ui->availableList->row(availitem);
			QListWidgetItem* selectitem = new QListWidgetItem(QIcon("://QUI/icon/Plugin_ins.png"), availitem->text());

			_ui->selectedList->addItem(selectitem);
			_ui->availableList->takeItem(row);
		}
	}
	void PluginManageDialog::on_removeButton_clicked()
	{
		QList<QListWidgetItem*> itemList = _ui->selectedList->selectedItems();
		const int n = itemList.size();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem* selectitem = itemList.at(i);
			const int row = _ui->selectedList->row(selectitem);
			QListWidgetItem* availitem = new QListWidgetItem(QIcon("://QUI/icon/Plugin_ava.png"), selectitem->text());
			_ui->availableList->addItem(availitem);

			_ui->selectedList->takeItem(row);
		}

	}

	void PluginManageDialog::appendItemToAvailableList(QString name)
	{
		QListWidgetItem* item = new QListWidgetItem(QIcon("://QUI/icon/Plugin_ava.png"), name);
		_ui->availableList->addItem(item);
	}
	void PluginManageDialog::appendItemToSelectedList(QString name)
	{
		QListWidgetItem* item = new QListWidgetItem(QIcon("://QUI/icon/Plugin_ins.png"), name);
		_ui->selectedList->addItem(item);
	}

	QStringList PluginManageDialog::getSelectedItemNames()
	{
		QStringList names;
		const int n = _ui->selectedList->count();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem*item = _ui->selectedList->item(i);
			const QString text = item->text();
			names.append(text);
		}
		return names;
	}
	void PluginManageDialog::accept()
	{
		QStringList selectedNames = this->getSelectedItemNames();
		QStringList installedNames = Setting::BusAPI::instance()->getPlugins();
		
		QStringList commonList;

		for(QString name : installedNames)
		{
			if (selectedNames.contains(name))
				commonList.append(name);
		}
		QStringList ins = commonList;

		QStringList failres;
		for (QString name : installedNames)
		{
			if (!commonList.contains(name))
			{
				bool ok = _manager->releasePlugin(name);
				{
					if (!ok)
					{
						failres.append(name);
						ins.append(name);
					}
						
				}
			}
				
		}

		QStringList failins;
		for (QString name : selectedNames)
		{
			if (!commonList.contains(name))
			{
				bool ok = _manager->loadPlugin(name);
				if (!ok)
				{
					failins.append(name);
				}
				ins.append(name);
			}
		}
		if (!failins.isEmpty())
		{
			QString info = failins.join(",")  + QString(tr(" load failed!"));
			QMessageBox::warning(this, tr("Warning"), info);
		}

		if (!failres.isEmpty())
		{
			QString info = failres.join(",") + QString(tr(" uninstall failed!"));
			QMessageBox::warning(this, tr("Warning"), info);
		}
		Setting::BusAPI::instance()->setPlugins(ins);
		emit this->updateMainWindowInteface();
		QDialog::accept();
	}


}


