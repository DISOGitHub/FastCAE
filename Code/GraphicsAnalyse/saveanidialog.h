#ifndef SAVEANIDIALOG_H
#define SAVEANIDIALOG_H
#include <QDialog>
#include "global.h"
#include "qfiledialog.h"
namespace Ui {
class saveAniDialog;
}

class saveAniDialog : public QDialog
{
    Q_OBJECT

public:
    explicit saveAniDialog(QWidget *parent = 0);
    ~saveAniDialog();
	void updateDialog(saveAni_prop* data);
private slots:
    void on_aniRate_doubleSpinBox_valueChanged(double arg1);

    void on_frameNum_spinBox_valueChanged(int arg1);

    void on_frameStep_spinBox_valueChanged(int arg1);

    void on_startFrame_spinBox_valueChanged(int arg1);

    void on_endFrame_spinBox_valueChanged(int arg1);

    void on_pngWid_lineEdit_editingFinished();

    void on_pngHei_lineEdit_editingFinished();

    void on_saveAni_Btn_clicked();

    void on_cancle_Btn_clicked();

    void on_imageFormat_comboBox_currentIndexChanged(int index);

    void on_imagePath_pushButton_clicked();
    void on_povPath_pushButton_clicked();

    void on_saveImage_checkBox_clicked(bool checked);

    void on_savePOV_checkBox_clicked(bool checked);

private:
    Ui::saveAniDialog *ui;
	saveAni_prop *ani_prop_data;
signals:
};

#endif // SAVEANIDIALOG_H
