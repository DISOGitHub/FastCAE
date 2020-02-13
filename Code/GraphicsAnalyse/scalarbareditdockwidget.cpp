#include "scalarbareditdockwidget.h"
#include "qmessagebox.h"

scalarBarEditDockWidget::scalarBarEditDockWidget(QWidget *parent) : QDockWidget(parent)
{
	this->setWindowTitle(tr("Color Map Editor"));
	scalarBarEdit_Form = new ScalarBarEditForm(this);
	setWidget(scalarBarEdit_Form);
}

void scalarBarEditDockWidget::reTranslate()
{
	this->setWindowTitle(tr("Color Map Editor"));
	scalarBarEdit_Form->reTranslate();
}