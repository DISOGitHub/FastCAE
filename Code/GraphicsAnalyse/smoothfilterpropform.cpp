#include "smoothfilterpropform.h"
#include "ui_smoothfilterpropform.h"

smoothFilterPropForm::smoothFilterPropForm(PipelineObjPropWidget *parent) :
QWidget(parent), _parentWin(parent),
    ui(new Ui::smoothFilterPropForm)
{
    ui->setupUi(this);
	smoothFilter_propData = NULL;
	cur_pipelineObj = NULL;
	//add for yumeiguo error
	ui->meshSmooth_radioButton->setVisible(false);
	ui->smooth_groupBox->setVisible(false);
	//add for yumeiguo error
}

smoothFilterPropForm::~smoothFilterPropForm()
{
    delete ui;
}

void smoothFilterPropForm::on_coef_spinBox_valueChanged(int arg1)
{
	if (cur_pipelineObj == NULL)
		return;
	if (smoothFilter_propData == NULL)
		return;
	smoothFilter_propData->smooth_coef = arg1;
	emit sig_enable_applyBtn();
}

void smoothFilterPropForm::updateForm()
{
	ui->coef_spinBox->blockSignals(true);
	ui->coef_spinBox->setValue(smoothFilter_propData->smooth_coef);
	ui->coef_spinBox->blockSignals(false);
	ui->cellToPoint_radioButton->setChecked(smoothFilter_propData->flag_cellToPoint);
	ui->meshSmooth_radioButton->setChecked(smoothFilter_propData->flag_meshSmooth);
	ui->smooth_groupBox->setVisible(smoothFilter_propData->flag_meshSmooth);
}

void smoothFilterPropForm::on_cellToPoint_radioButton_clicked(bool checked)
{
	smoothFilter_propData->flag_cellToPoint = checked;
	smoothFilter_propData->flag_meshSmooth = !checked;
	ui->smooth_groupBox->setVisible(!checked);
	emit _parentWin->sig_python_smooth(cur_pipelineObj, 0, 0);
	emit sig_enable_applyBtn();
}

void smoothFilterPropForm::on_meshSmooth_radioButton_clicked(bool checked)
{
	smoothFilter_propData->flag_meshSmooth = checked;
	smoothFilter_propData->flag_cellToPoint = !checked;
	ui->smooth_groupBox->setVisible(checked);
	emit sig_enable_applyBtn();
}

void smoothFilterPropForm::reTranslate()
{
	ui->retranslateUi(this);
}
