#ifndef _PLUGINMANAGERDIALOG_H_
#define _PLUGINMANAGERDIALOG_H_

#include <QDialog>

namespace Ui
{
	class PluginManageDialog;
}

namespace GUI
{
	class MainWindow;
}

namespace Plugins
{
	class PluginManager;

	class PluginManageDialog : public QDialog
	{
		Q_OBJECT
	public:
		PluginManageDialog(GUI::MainWindow* mainwidnow, PluginManager* m);
		~PluginManageDialog();

	signals:
		void updateMainWindowInteface();

	private slots:
	    void on_addButton_clicked();
		void on_removeButton_clicked();
		void accept() override;

	private:
		void init();
		void appendItemToAvailableList(QString);
		void appendItemToSelectedList(QString);
		QStringList getSelectedItemNames();

	private:
		Ui::PluginManageDialog* _ui{};
		GUI::MainWindow* _mainWindow{};

		PluginManager* _manager{};
	};
}



#endif