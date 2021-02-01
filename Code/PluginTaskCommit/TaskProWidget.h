#ifndef TASKPROWIDGET_H
#define TASKPROWIDGET_H

#include <QWidget>
#include "TaskCommitAPI.h"
#include "TaskData.h"

class QTreeWidgetItem;
class QAction;
class QMenu;

QT_BEGIN_NAMESPACE
namespace Ui { class TaskProWidget; }
QT_END_NAMESPACE

namespace Plugins {

class TaskMgeWidget;

/**
 * @brief The TaskProWidget class
 * 任务树 *
 */
class TaskProWidget : public QWidget
{
    Q_OBJECT

public:
    TaskProWidget(QWidget *parent = nullptr);
    ~TaskProWidget();
    void init();
    /**
     * @brief updateTaskData *处理数据处理模块的结果函数*
     * @param ptr *任务数据,用于更新前端界面*
     */
    void updataTaskData(TaskData::Ptr&& ptr);
    bool getActiveTaskMgrWidget();
    QTreeWidgetItem* getItemFromTaskNode(QString taskName);
    void addItem(QTreeWidgetItem* parent, const TaskData::Ptr &ptr);
    void addItem(QTreeWidgetItem* parent, const TaskData &p);
    void changeState(TASK_STATE s);
    TaskMgeWidget *getTMgeWidget() const;
    void setTMgeWidget(TaskMgeWidget *value);
    void clearTask();
    void reTranslate();

private slots:
    void onCreateTask();
    void onCommitTask();
    void onCalcTask();
    void onDownResult();
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void on_treeWidget_itemPressed(QTreeWidgetItem *item, int column);

private:
    Ui::TaskProWidget *ui;
    TaskMgeWidget* tMgeWidget;
    QTreeWidgetItem *_tasksItem;
    QTreeWidgetItem *_resultsItem;
    QTreeWidgetItem *curItem;
    QAction *crt;
    QAction *commit;
    QAction *calc;
    QAction *down;
    QAction *hcl;
    QAction *of;
    QAction *query;
    QMenu* taskTreeMenu;
    QMenu* resultTreeMenu;
};
#endif // TASKPROWIDGET_H

}
