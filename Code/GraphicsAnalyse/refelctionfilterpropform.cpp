#include "refelctionfilterpropform.h"
#include "ui_refelctionfilterpropform.h"

refelctionFilterPropForm::refelctionFilterPropForm(PipelineObjPropWidget *parent) :
QWidget(parent), _parentWin(parent),
    ui(new Ui::refelctionFilterPropForm)
{
    ui->setupUi(this);
	reflectionFilter_propData = NULL;
	cur_pipelineObj = NULL;
}

refelctionFilterPropForm::~refelctionFilterPropForm()
{
    delete ui;
}

void refelctionFilterPropForm::on_comboBox_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	if (cur_pipelineObj == NULL)
		return;
	if (reflectionFilter_propData == NULL)
		return;
	reflectionFilter_propData->reflection_axis = (enum ReflectionPlane) index;
	emit _parentWin->sig_python_reflectionAxes(cur_pipelineObj);
	emit sig_enable_applyBtn();
}

void refelctionFilterPropForm::updateForm()
{
	ui->comboBox->blockSignals(true);
	ui->comboBox->setCurrentIndex((int)reflectionFilter_propData->reflection_axis);
	ui->comboBox->blockSignals(false);
	ui->copyInput_checkBox->setChecked(reflectionFilter_propData->flag_copyInput);
}

void refelctionFilterPropForm::reTranslate()
{
	ui->retranslateUi(this);
}
void refelctionFilterPropForm::on_copyInput_checkBox_clicked(bool checked)
{
	if (cur_pipelineObj == NULL)
		return;
	if (reflectionFilter_propData == NULL)
		return;
	reflectionFilter_propData->flag_copyInput =checked;
	emit _parentWin->sig_python_reflectionCopyInput(cur_pipelineObj);
	emit sig_enable_applyBtn();
}
