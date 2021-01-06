#ifndef CALFILTERPROPFORM_H
#define CALFILTERPROPFORM_H

#include <QWidget>
#include "graphicsanalyse_global.h"
#include "PipelineObject.h"
#include "pipelineobjpropwidget.h"
namespace Ui {
class calFilterPropForm;
}

class calFilterPropForm : public QWidget
{
    Q_OBJECT

public:
    explicit calFilterPropForm(QWidget *parent = 0);
    ~calFilterPropForm();
	void updateForm();
	struct calculatorFilter_pipelineObj_propData *calculatorFilter_propData;
	PipelineObject* cur_pipelineObj;
	virtual void reTranslate();
private slots:
    void on_clear_pushButton_clicked();

    void on_left_pushButton_clicked();

    void on_right_pushButton_clicked();

    void on_ln_pushButton_clicked();

    void on_log10_pushButton_clicked();

    void on_add_pushButton_clicked();

    void on_sin_pushButton_clicked();
    void on_asin_pushButton_clicked();
    void on_sinh_pushButton_clicked();

    void on_cos_pushButton_clicked();
    void on_acos_pushButton_clicked();
    void on_cosh_pushButton_clicked();

    void on_tan_pushButton_clicked();
    void on_atan_pushButton_clicked();
    void on_tanh_pushButton_clicked();

    void on_abs_pushButton_clicked();

    void on_ceil_pushButton_clicked();

    void on_sqrtxy_pushButton_clicked();

    void on_sqrt_pushButton_clicked();

    void on_floor_pushButton_clicked();

    void on_exp_pushButton_clicked();

    void on_sub_pushButton_clicked();

    void on_mul_pushButton_clicked();

    void on_div_pushButton_clicked();

    void on_scalars_comboBox_currentIndexChanged(int index);
    void on_vectors_comboBox_currentIndexChanged(int index);
    void on_cal_lineEdit_textChanged(const QString &arg1);

    void on_resultName_lineEdit_textChanged(const QString &arg1);

    void on_v1v2_pushButton_clicked();

    void on_mag_pushButton_clicked();

    void on_norm_pushButton_clicked();

    void on_iHat_pushButton_clicked();

    void on_jHat_pushButton_clicked();

    void on_kHat_pushButton_clicked();

signals:
	void sig_enable_applyBtn();
private:
    Ui::calFilterPropForm *ui;
	PipelineObjPropWidget* _parentWin{};
	void input(QString str);
};

#endif // CALFILTERPROPFORM_H

