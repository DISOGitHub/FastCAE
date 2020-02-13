#ifndef SAVESCRIPTDIALOG_H
#define SAVESCRIPTDIALOG_H

#include <QDialog>
#include "global.h"
#include "qfiledialog.h"
namespace Ui {
class saveScriptDialog;
}
class saveScriptDialog : public QDialog
{
    Q_OBJECT
public:
	explicit saveScriptDialog(QWidget *parent = 0);
	~saveScriptDialog();
	saveScript_prop get_saveScript_prop_data() { return saveScript_prop_data; };
	void setImageSize(QSize tep_size);
	saveScript_prop saveScript_prop_data;
private slots:
    void on_saveImage_checkBox_clicked(bool checked);
    void on_imagePath_pushButton_clicked();

    void on_saveAvi_checkBox_clicked(bool checked);

    void on_aviPath_pushButton_clicked();
	void on_scriptPath_pushButton_clicked();

    void on_save_pushButton_clicked();

    void on_close_pushButton_clicked();

    void on_wid_lineEdit_textChanged(const QString &arg1);

    void on_hei_lineEdit_textChanged(const QString &arg1);

    void on_autoExit_checkBox_clicked(bool checked);

    void on_exitTime_spinBox_valueChanged(int arg1);

    void on_plotSize_checkBox_clicked(bool checked);

    void on_adapt_checkBox_clicked(bool checked);

private:
	Ui::saveScriptDialog *ui;
	
};

#endif // SAVEIMAGEDIALOG_H
