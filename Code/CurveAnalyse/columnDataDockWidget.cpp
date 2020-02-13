#include "columnDataDockWidget.h"

ColumnDataDockWidget::ColumnDataDockWidget(QWidget *parent) : QDockWidget(parent)
{
    setStyleSheet("QWidget{background-color: #eaf2f8;}""QDockWidget::title{border-radius: 6px; background-image: url(:/images/project_title.png);}");
    this->setWindowTitle(tr("column data"));
    initWidget();
}

ColumnDataDockWidget::~ColumnDataDockWidget()
{
	if (column_dataForm != NULL)
	{
		delete column_dataForm;
		column_dataForm = NULL;
	}
}

void ColumnDataDockWidget::initWidget()
{
    column_dataForm = new ColumnDataForm(this);
    setWidget(column_dataForm);

}

void ColumnDataDockWidget::slot_console_file_data_val_show(int index_file)
{
    column_dataForm->update_columnDataWidget(index_file);
}

void ColumnDataDockWidget::slot_console_file_data_val_show_from_curveData(int index_file,QVector<int> curve_col_list)
{
    column_dataForm->update_columnDataWidget_from_curveData(index_file,curve_col_list);
}

void ColumnDataDockWidget::reTranslate()
{
	this->setWindowTitle(tr("column data"));
}
