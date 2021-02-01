#ifndef DOWNTASKDIALOG_H
#define DOWNTASKDIALOG_H

#include <QDialog>
#include "TaskData.h"

namespace Ui {
class DownTaskDialog;
}

namespace Plugins {

/**
 * @brief The DownTaskDialog class
 * 结果文件下载对话框 *
 */
class DownTaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DownTaskDialog(QWidget *parent = nullptr);
    ~DownTaskDialog();
    void init();
    /**
     * @brief updateTaskData *处理数据处理模块的结果函数*
     * @param ptr *任务数据,用于更新前端界面*
     */
    void updateTaskData(TaskData::Ptr &&ptr);
    bool removeDownloadTask(QString name);
    void addItem(const TaskData::Ptr &ptr); /// TaskData::Ptr&& ptr
    void updateTranslatorUI();

public slots:
    void onFileSize(QString fileName, int size);
    void onDownedSize(QString fileName, int schedule);
    void onSpeedSize(QString fileName, QString s);

private slots:
    void on_pushButton_clicked();

    void on_clearPushButton_clicked();

private:
    Ui::DownTaskDialog *ui;
};

}
#endif // DOWNTASKDIALOG_H
