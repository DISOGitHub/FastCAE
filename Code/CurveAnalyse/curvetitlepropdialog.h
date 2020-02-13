#ifndef CURVETITLEPROPDIALOG_H
#define CURVETITLEPROPDIALOG_H

#include <QDialog>
#include "curve_struct.h"
namespace Ui {
class curveTitlePropDialog;
}

class curveTitlePropDialog : public QDialog
{
    Q_OBJECT

public:
	explicit curveTitlePropDialog(QWidget *parent = 0);
	~curveTitlePropDialog();
	curve_ini_data *curve_data_ptr;
	void initForm();
private slots:
    void on_title_name_lineEdit_textChanged(QString);
    void on_title_fontType_comboBox_currentIndexChanged(int);
    void on_title_fontColor_pushButton_clicked();
    void on_title_fontSize_spinBox_valueChanged(int);
signals:
    void sig_setTitle();
private:
	Ui::curveTitlePropDialog *ui;
	
};

#endif // CURVETITLEPROPDIALOG_H
