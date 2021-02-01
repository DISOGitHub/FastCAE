#ifndef TASKCOMMITPLUGIN_H
#define TASKCOMMITPLUGIN_H
#include "TaskCommitAPI.h"
#include "PluginManager/pluginBase.h"

class QAction;

namespace ModelData
{
	class ModelDataBase;
}

namespace Plugins {
	class TaskMgeWidget;
	class TaskProWidget;
}
class QMdiSubWindow;

namespace Plugins
{
	class TASK_COMMIT TaskCommitPlugin : public PluginBase
	{
		Q_OBJECT
	public:
		TaskCommitPlugin(GUI::MainWindow* m);
		~TaskCommitPlugin();

		///
		bool install() override;
		///
		bool uninstall() override;

		void reTranslate(QString lang) override;

		static GUI::MainWindow* getMainWindow() { return _mainWindow; };
		static Plugins::TaskCommitPlugin*getSelf() { return _taskCommitPlugin; };
        QMdiSubWindow* getTaskMgrWindow();
		bool createTaskMgrWidget();
		void printMessage(int type, const QString &msg);
        void clearData();
		static Plugins::TaskCommitPlugin* _taskCommitPlugin;

	public slots:
		void onViewTaskWindow();
		void onTaskWidowCloced();

	private:
		static GUI::MainWindow* _mainWindow;
		ModelData::ModelDataBase* _data{};
        TaskMgeWidget* _tmw{nullptr};
        TaskProWidget* _tpw{nullptr};
        QMdiSubWindow* _msw{nullptr};
		QAction* _action{};
	};
}


extern "C"
{
	void TASK_COMMIT Register(GUI::MainWindow* m, QList<Plugins::PluginBase*>* plugs);
}


#endif // TASKCOMMITPLUGIN_H
