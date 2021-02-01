#ifndef MONITORTASKINFO_H
#define MONITORTASKINFO_H

#include "ActionType.h"
#include <QObject>
#include <QThread>
#include <QQueue>
#include "TaskData.h"
#include <atomic>
#include <QRunnable>

namespace Plugins{

/**
 * @brief The MonitorRunnable class
 * 计算任务监控执行类 *
 */
class MonitorRunnable : public QObject,  public QRunnable
{
    Q_OBJECT
public:
    MonitorRunnable(Plugins::SelectTaskTransferData::Ptr&& ptr);

    ~MonitorRunnable()
    {

    }

    void run() override;
    void notifyRun();
signals:
    void signalTaskInfo(TaskData::Ptr ptr);
private:
    Plugins::SelectTaskTransferData::Ptr ptr;
};

/**
 * @brief The MonitorTaskInfo class
 * 计算任务监控线程 *
 */
class MonitorTaskInfo : public QThread
{
    Q_OBJECT
public:
//    using Ptr = std::shared_ptr<MonitorTaskInfo>;
    MonitorTaskInfo();

    ~MonitorTaskInfo()
    {

    }
    void run() override;
    virtual void timerEvent( QTimerEvent *event) override;
    void notifyRun();
    void addMonitorTask(SelectTaskTransferData::Ptr &ptr);
    void removeMonitorTask(const QString& jobName);
    void startMonitorTimer();
    void killMonitorTimer();
    bool getQuit() const;
    void setQuit(bool value);

signals:
    void signalTaskData(TaskData::Ptr ptr);

public slots:
    void onTaskData(TaskData::Ptr ptr);

private:
    bool quit{false};
    int timerID;
    QQueue<Plugins::SelectTaskTransferData::Ptr> tasks;
};
}
#endif // MONITORTASKINFO_H
