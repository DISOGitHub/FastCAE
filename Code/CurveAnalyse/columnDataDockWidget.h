#ifndef COLUMNDATADOCKWIDGET_H
#define COLUMNDATADOCKWIDGET_H
#include <QDockWidget>
#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QTreeWidget>
#include <QContextMenuEvent>
#include "qstringlist.h"
#include "columndataform.h"
#include "curve_struct.h"
#include "curveanalyse_global.h"

class ColumnDataDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit ColumnDataDockWidget(QWidget *parent = 0);
    ~ColumnDataDockWidget();
    void initWidget();
	ColumnDataForm* get_column_dataForm() { return column_dataForm; };
	virtual void reTranslate();
signals:

public slots:

    void slot_console_file_data_val_show(int indexfile);
    void slot_console_file_data_val_show_from_curveData(int indexfile,QVector<int> curve_col_list);
private:
	ColumnDataForm* column_dataForm;
};

#endif // CURVETREEDOCKWIDGET_H
