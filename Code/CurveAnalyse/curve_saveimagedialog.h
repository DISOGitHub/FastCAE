#ifndef CURVESAVEIMAGEDIALOG_H
#define CURVESAVEIMAGEDIALOG_H

#include <QDialog>
#include "curve_struct.h"
#include "qfiledialog.h"

namespace Ui {
class curve_saveImageDialog;
}

class curve_saveImageDialog : public QDialog
{
    Q_OBJECT

public:
	explicit curve_saveImageDialog(QWidget *parent = 0);
	~curve_saveImageDialog();
	curve_saveImage_prop get_saveImage_prop_data() { return saveImage_prop_data; };
private slots:
    void on_imagePath_pushButton_clicked();

    void on_save_pushButton_clicked();

    void on_close_pushButton_clicked();

    void on_wid_lineEdit_textChanged(const QString &arg1);

    void on_hei_lineEdit_textChanged(const QString &arg1);

private:
    Ui::curve_saveImageDialog *ui;
	curve_saveImage_prop saveImage_prop_data;
};

#endif // SAVEIMAGEDIALOG_H
