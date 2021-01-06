#ifndef SAVEIMAGEDIALOG_H
#define SAVEIMAGEDIALOG_H

#include <QDialog>
#include "global.h"
#include "qfiledialog.h"
namespace Ui {
class saveImageDialog;
}
class saveImageDialog : public QDialog
{
    Q_OBJECT
public:
    explicit saveImageDialog(QWidget *parent = 0);
    ~saveImageDialog();
	saveImage_prop get_saveImage_prop_data() { return saveImage_prop_data; };
	void setImageSize(QSize tep_size);
private slots:
    void on_saveImage_checkBox_clicked(bool checked);
    void on_imagePath_pushButton_clicked();

    void on_savePOV_checkBox_clicked(bool checked);

    void on_povPath_pushButton_clicked();

    void on_save_pushButton_clicked();

    void on_close_pushButton_clicked();

    void on_wid_lineEdit_textChanged(const QString &arg1);

    void on_hei_lineEdit_textChanged(const QString &arg1);

private:
    Ui::saveImageDialog *ui;
	saveImage_prop saveImage_prop_data;
};

#endif // SAVEIMAGEDIALOG_H
