#ifndef PIPELINEOBJGLOPROPFORM_H
#define PIPELINEOBJGLOPROPFORM_H

#include <QWidget>

namespace Ui {
class pipelineObjGloPropForm;
}

class pipelineObjGloPropForm : public QWidget
{
    Q_OBJECT

public:
    explicit pipelineObjGloPropForm(QWidget *parent = 0);
    ~pipelineObjGloPropForm();

private:
    Ui::pipelineObjGloPropForm *ui;
};

#endif // PIPELINEOBJGLOPROPFORM_H
