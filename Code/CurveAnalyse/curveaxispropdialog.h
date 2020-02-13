#ifndef CURVEAXISPROPDIALOG_H
#define CURVEAXISPROPDIALOG_H

#include <QDialog>
#include "curve_struct.h"
#include "qcustomplot.h"

namespace Ui {
class curveAxisPropDialog;
}

class curveAxisPropDialog : public QDialog
{
    Q_OBJECT

public:
    explicit curveAxisPropDialog(QWidget *parent = 0);
    ~curveAxisPropDialog();
    curve_ini_data *curve_data_ptr;
    void setAixsId(QString tep_id){axisID=tep_id;};
	void initForm();
private slots:
    void on_axis_name_lineEdit1_textChanged(QString);
    void on_axis_fontType_comboBox1_currentIndexChanged(int);
    void on_axis_fontColor_pushButton1_clicked();
    void on_axis_fontSize_spinBox1_valueChanged(int);
    void on_axis_dataSource_comboBox1_currentIndexChanged(int);
    void on_axis_dataFile_comboBox1_currentIndexChanged(int);
    void on_axis_dataColumn_comboBox1_currentIndexChanged(int);
    void on_lineEdit_min_editingFinished();

    void on_lineEdit_max_editingFinished();

    void on_rescale_pushButton_clicked();

signals:
    void sig_setAxisName(QString);
    void sig_axis_font(QString);
    void sig_setAxisRange(QString);
    void sig_xAxis_dataSource_change(QString);
private:
    Ui::curveAxisPropDialog *ui;
    QString axisID;
    global_plot_axis_prop* axisPropPtr;
};

#endif // CURVEAXISPROPDIALOG_H
