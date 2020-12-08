#ifndef COLUMNDATAFORM_H
#define COLUMNDATAFORM_H

#include <QWidget>
#include <QTextCodec>
#include "curve_struct.h"
#include "curveanalyse_global.h"

namespace Ui {
class ColumnDataForm;
}

class ColumnDataForm : public QWidget
{
    Q_OBJECT

public:
    explicit ColumnDataForm(QWidget *parent = 0);
    ~ColumnDataForm();
    void update_columnDataWidget(int index_file);
    void update_columnDataWidget_from_curveData(int index_file,QVector<int> curve_col_list);
	void func_tableWidget_clicked(int tep_file_index, int tep_column_index);
	void set_tableWidget_checked(int tep_column_index, Qt::CheckState status);
    curve_ini_data *curve_data_ptr;
    int cur_index_file,cur_index_curve;
	virtual void reTranslate();
public slots:
	void slot_update_clear_data();
private slots:
    void on_tableWidget_clicked(const QModelIndex &index);
	
signals:
    void sig_add_curve(int tep_index_file,int col);
    void sig_del_curve(int tep_index_file,int col);
private:
    Ui::ColumnDataForm *ui;
    QTextCodec *codec;
	
};

#endif // COLUMNDATAFORM_H
