#ifndef TASKMGEWIDGET_H
#define TASKMGEWIDGET_H

#include <QWidget>
#include "TaskCommitAPI.h"
#include "TaskData.h"

namespace GUI
{
	class MainWindow;
}

namespace Ui {
class TaskMgeWidget;
}

class QPushButton;
class QTreeWidgetItem;


namespace Plugins {

class DownTaskDialog;
class CommitTaskDialog;
class MonitorTaskInfo;

/**
 * @brief The ControlWidget class
 *
 */
class TASK_COMMIT ControlWidget : public QWidget
{
    Q_OBJECT
public:
    ControlWidget(QWidget *parent = nullptr);
    ~ControlWidget() = default;

    void reTranslate();

    void setEnableState(TASK_STATE state);

    QPushButton *getCommitButton() const;
    void setCommitButton(QPushButton *value);

    QPushButton *getCalButton() const;
    void setCalButton(QPushButton *value);

    QPushButton *getStopButton() const;
    void setStopButton(QPushButton *value);

    QPushButton *getDowButton() const;
    void setDowButton(QPushButton *value);

    QPushButton *getDelButton() const;
    void setDelButton(QPushButton *value);

    QPushButton *getUpdateButton() const;
    void setUpdateButton(QPushButton *value);

signals:
    void commit_clicked();
    void cal_clicked();
    void del_clicked();
    void dow_clicked();
    void stop_clicked();
    void query_clicked();

private:
    QPushButton* commitButton = nullptr;
    QPushButton* calButton = nullptr;
    QPushButton* stopButton = nullptr;
    QPushButton* dowButton = nullptr;
    QPushButton* delButton = nullptr;
    QPushButton* updateButton = nullptr;
};

/**
 * @brief The TaskMgeWidget class
 * 任务管理窗口 *
 */
class TaskMgeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TaskMgeWidget(GUI::MainWindow * mw);
    ~TaskMgeWidget();
    void init();
    void initAllTaskState(); /** 初始化类型与显示字符串映射表 */
    /**
     * @brief updateTaskData *处理数据处理模块的结果函数*
     * @param ptr *任务数据,用于更新前端界面*
     */
    void updataTaskData(TaskData::Ptr&& ptr);
    void commitTask(CommitRunable* cit);
    void calcTask(ExecuteRunable* exec);
    void downTask(ResultRunable *result);
    void updateTranslatorUI();
    void addMonitor(const TaskData::Ptr &ptr);
    void addMonitor(const TaskData &p);
    void updateTableWidget(const TaskStore::Ptr &ptr);
    void setItemState(const QString& jobName, TASK_STATE s);
    void updataUsedTime(const QString &jobName, const QString &time);
    ControlWidget* getControlWidgetFromTable(QString taskName);
    void removwItemFromTaskName(QString name);
    void clearTask();

signals:
    void taskMgeWidgetDestoryed();

public: /// gui
    void addItem(const TaskData::Ptr &ptr);
    void addItem(const TaskData *ptr);

public slots:
    void onCommitClick(); /** 任务上传 */

    void onCalClick();    /** 任务计算 */

    void onDelClick();    /** 删除任务 */

    void onDowClick();    /** 结果下载 */

    void onStopClick();   /** 停止计算 */

    void onQueryClick();  /** 信息查询 */

    void onQuertInfo(TaskData::Ptr ptr);

    void onExecuteState(QString fileName, int s, int jobid);

    void onExecuteJobID(QString fileName, int id);

    void onCommitState(QString fileName, int s);

    void onDownState(QString fileName, int s);

    void onKillTimer();

    void on_PushButton_clicked();

    void on_ctkPushButton_clicked();

    void on_clearPushButton_clicked();

    void on_resultDownPushButton_clicked();

private slots:
    void on_commitPushButton_clicked();

private:
    DownTaskDialog* downDialog{};
    CommitTaskDialog* commitDialog{};
    MonitorTaskInfo* monitorRunable = nullptr;  /// will move
    Ui::TaskMgeWidget *ui{};

	GUI::MainWindow* _mainWindow{};
};

}


#endif // TASKMGEWIDGET_H
