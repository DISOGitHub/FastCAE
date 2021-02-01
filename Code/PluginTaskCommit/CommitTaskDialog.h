#ifndef COMMITTASKDIALOG_H
#define COMMITTASKDIALOG_H

#include <QDialog>
#include "TaskData.h"

namespace Ui {
class CommitTaskDialog;
}

namespace Plugins {

/**
 * @brief The CommitTaskDialog class
 * 任务上传对话框*
 */
class CommitTaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CommitTaskDialog(QWidget *parent = nullptr);
    ~CommitTaskDialog();
    void init();
    /**
     * @brief updateTaskData *处理数据处理模块的结果函数*
     * @param ptr *任务数据,用于更新前端界面*
     */
    void updateTaskData(TaskData::Ptr &&ptr);
    bool getRepeatUploadTask(QString name);
    void addItem(const TaskData::Ptr &ptr); /// TaskData::Ptr&& ptr
    void updateTranslatorUI();

public slots:
    void onFileSize(QString fileName, int size);
    void onCommitedSize(QString fileName, int schedule);
    void onSpeedSize(QString fileName, QString s);

private slots:
    void on_clearPushButton_clicked();

    void on_quitPushButton_clicked();

private:
    Ui::CommitTaskDialog *ui;
};

}
#endif // CommitTaskDialog_H
