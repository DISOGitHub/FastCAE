#include "PipelineObjPropDockWidget.h"

PipelineObjPropDockWidget::PipelineObjPropDockWidget(QWidget *parent) :
    QDockWidget(parent)
{
	this->setWindowTitle(tr("Pipeline properties"));
	propWidget = new PipelineObjPropWidget;
	setWidget(propWidget);
	wid = 200;
	hei = 400;
}

PipelineObjPropDockWidget::~PipelineObjPropDockWidget()
{
    //delete ui;
}

void PipelineObjPropDockWidget::reTranslate()
{
	this->setWindowTitle(tr("Pipeline properties"));
	propWidget->reTranslate();
}
