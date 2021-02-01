#ifndef CREATTASKDIALOG_H
#define CREATTASKDIALOG_H

#include "TaskData.h"

#include "SelfDefObject/QFDialog.h"

namespace Ui {
class CreatTaskDialog;
}

namespace Plugins {

/**
 * @brief The CreatTaskDialog class
 * 创建任务对话框 *
 */
class CreatTaskDialog : public QFDialog
{
    Q_OBJECT

public:
    explicit CreatTaskDialog(GUI::MainWindow*  m);
    ~CreatTaskDialog();
    void init();
    CreateTaskTransferData::Ptr getCreateTaskTransferDataPtr();

private slots:
    void on_createPushButton_clicked();
    
private:
    Ui::CreatTaskDialog *ui;
    CreateTaskTransferData::Ptr ptr;
};
}
#endif // CREATTASKDIALOG_H
