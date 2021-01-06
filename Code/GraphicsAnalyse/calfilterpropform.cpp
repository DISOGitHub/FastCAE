#include "calfilterpropform.h"
#include "ui_calfilterpropform.h"
#include "qdebug.h"
calFilterPropForm::calFilterPropForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::calFilterPropForm)
{
    ui->setupUi(this);
	calculatorFilter_propData = NULL;
	cur_pipelineObj = NULL;
}

calFilterPropForm::~calFilterPropForm()
{
    delete ui;
}
void calFilterPropForm::updateForm()
{
	QString tep_str;
    ui->resultName_lineEdit->setText(calculatorFilter_propData->resultName);
	ui->cal_lineEdit->setText(calculatorFilter_propData->formula);
    ui->scalars_comboBox->blockSignals(true);
    ui->scalars_comboBox->clear();
    ui->scalars_comboBox->addItem("Scalars");
	for (int i = 0; i < cur_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarList.count(); i++)
	{
		tep_str = cur_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.scalarList.at(i);
        if (tep_str.indexOf("_cell000")< 0)
		{
            ui->scalars_comboBox->addItem(tep_str);
		}
	}
    ui->scalars_comboBox->blockSignals(false);
    ui->scalars_comboBox->setCurrentIndex(0);

    ui->vectors_comboBox->blockSignals(true);
    ui->vectors_comboBox->clear();
    ui->vectors_comboBox->addItem("Vectors");
    for (int i = 0; i < cur_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.vectorList.count(); i++)
    {
        tep_str = cur_pipelineObj->mPipeLineObjProp.pipelineObj_base_propData.vectorList.at(i);
        if (tep_str.indexOf("_cell000")< 0)
        {
            ui->vectors_comboBox->addItem(tep_str);
        }
    }
    ui->vectors_comboBox->blockSignals(false);
    ui->vectors_comboBox->setCurrentIndex(0);
}

void calFilterPropForm::reTranslate()
{
	ui->retranslateUi(this);
}
void calFilterPropForm::on_clear_pushButton_clicked()
{
	ui->cal_lineEdit->clear();
    calculatorFilter_propData->formula ="";
}

void calFilterPropForm::input(QString str)
{
	ui->cal_lineEdit->setText(ui->cal_lineEdit->text() + str);
}

void calFilterPropForm::on_left_pushButton_clicked()
{
	input("(");
}

void calFilterPropForm::on_right_pushButton_clicked()
{
	input(")");
}

void calFilterPropForm::on_ln_pushButton_clicked()
{
	input("ln");
}

void calFilterPropForm::on_log10_pushButton_clicked()
{
	input("lg");
}

void calFilterPropForm::on_add_pushButton_clicked()
{
	input("+");
}

void calFilterPropForm::on_sin_pushButton_clicked()
{
	input("sin");
}
void calFilterPropForm::on_asin_pushButton_clicked()
{
	input("asin");
}
void calFilterPropForm::on_sinh_pushButton_clicked()
{
	input("sinh");
}
void calFilterPropForm::on_cos_pushButton_clicked()
{
	input("cos");
}
void calFilterPropForm::on_acos_pushButton_clicked()
{
	input("acos");
}
void calFilterPropForm::on_cosh_pushButton_clicked()
{
	input("cosh");
}
void calFilterPropForm::on_tan_pushButton_clicked()
{
	input("tan");
}
void calFilterPropForm::on_atan_pushButton_clicked()
{
	input("atan");
}
void calFilterPropForm::on_tanh_pushButton_clicked()
{
	input("tanh");
}

void calFilterPropForm::on_abs_pushButton_clicked()
{
	input("fabs");
}

void calFilterPropForm::on_ceil_pushButton_clicked()
{
	input("ceil");
}

void calFilterPropForm::on_sqrtxy_pushButton_clicked()
{
	input("^");
}

void calFilterPropForm::on_sqrt_pushButton_clicked()
{
	input("sqrt");
}

void calFilterPropForm::on_floor_pushButton_clicked()
{
	input("floor");
}

void calFilterPropForm::on_exp_pushButton_clicked()
{
	input("exp");
}

void calFilterPropForm::on_sub_pushButton_clicked()
{
	input("-");
}

void calFilterPropForm::on_mul_pushButton_clicked()
{
	input("*");
}

void calFilterPropForm::on_div_pushButton_clicked()
{
	input("/");
}

void calFilterPropForm::on_scalars_comboBox_currentIndexChanged(int index)
{
	if (index == 0)
		return;
	QString tep_str = ui->scalars_comboBox->currentText();
	input(tep_str);
}
void calFilterPropForm::on_vectors_comboBox_currentIndexChanged(int index)
{
    if (index == 0)
        return;
    QString tep_str = ui->vectors_comboBox->currentText();
    input(tep_str);
}
void calFilterPropForm::on_cal_lineEdit_textChanged(const QString &arg1)
{
    calculatorFilter_propData->formula=arg1;
	emit sig_enable_applyBtn();
}

void calFilterPropForm::on_resultName_lineEdit_textChanged(const QString &arg1)
{
	calculatorFilter_propData->resultName = arg1;
	emit sig_enable_applyBtn();
}

void calFilterPropForm::on_v1v2_pushButton_clicked()
{
	input(".");
}

void calFilterPropForm::on_mag_pushButton_clicked()
{
	input("mag");
}

void calFilterPropForm::on_norm_pushButton_clicked()
{
	input("norm");
}

void calFilterPropForm::on_iHat_pushButton_clicked()
{
	input("iHat");
}

void calFilterPropForm::on_jHat_pushButton_clicked()
{
	input("jHat");
}

void calFilterPropForm::on_kHat_pushButton_clicked()
{
	input("kHat");
}
