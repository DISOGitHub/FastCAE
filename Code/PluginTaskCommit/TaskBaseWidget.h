#ifndef TASKBASEWIDGET_H
#define TASKBASEWIDGET_H

#include <QWidget>

namespace TaskCommit{

class TaskBaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TaskBaseWidget(QWidget *parent = nullptr);
    ~TaskBaseWidget();
};

}
#endif // TASKBASEWIDGET_H
