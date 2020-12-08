#include "pipelineobjglopropform.h"
#include "ui_pipelineobjglopropform.h"

pipelineObjGloPropForm::pipelineObjGloPropForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pipelineObjGloPropForm)
{
    ui->setupUi(this);
}

pipelineObjGloPropForm::~pipelineObjGloPropForm()
{
    delete ui;
}
