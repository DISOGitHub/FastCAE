#include "streamlinefilterpropform.h"
#include "ui_streamlinefilterpropform.h"
#include "pipelineobjpropwidget.h"
#include "FilterStreamLine.h"
streamLineFilterPropForm::streamLineFilterPropForm(PipelineObjPropWidget *parent) :
QWidget(parent), _parentWin(parent),
    ui(new Ui::streamLineFilterPropForm)
{
    ui->setupUi(this);
}

streamLineFilterPropForm::~streamLineFilterPropForm()
{
    delete ui;
}

void streamLineFilterPropForm::updateForm()
{
	QString tep_str;
	QIcon icon3 = QIcon::fromTheme("val_on_point.png", QIcon(":/images/val_on_point.png"));
	ui->vector_comboBox->blockSignals(true);
	ui->vector_comboBox->clear();
	//ui->vector_comboBox->addItem(icon3, "None", "val_on_point.png");	
	for (int i = 0; i < streamLineFilter_propData->vector_list.count(); i++)
	{
		tep_str = streamLineFilter_propData->vector_list.at(i);
		if (tep_str.indexOf("_cell000") >= 0)
		{
			QIcon icon1 = QIcon::fromTheme("val_on_cell.png", QIcon(":/images/val_on_cell.png"));
			ui->vector_comboBox->addItem(icon1, tep_str.left(tep_str.length() - 8), "val_on_cell.png");
		}
		else
		{
			QIcon icon3 = QIcon::fromTheme("val_on_point.png", QIcon(":/images/val_on_point.png"));
			ui->vector_comboBox->addItem(icon3, tep_str, "val_on_point.png");
		}
	}
	ui->vector_comboBox->blockSignals(false);
	tep_str = streamLineFilter_propData->vectorColName;
	if (tep_str.indexOf("_cell000") >= 0)
		tep_str = tep_str.left(tep_str.length() - 8);
	ui->vector_comboBox->setCurrentText(tep_str);
	ui->interpolatorType_comboBox->setCurrentIndex(streamLineFilter_propData->interpolator_type);
	ui->surfaceLines_checkBox->setChecked(streamLineFilter_propData->surface_streamLines);
	ui->Inte_Direction->setCurrentIndex(streamLineFilter_propData->integration_data.direction);
	ui->inte_Type->setCurrentIndex(streamLineFilter_propData->integration_data.type);
	ui->inte_StepUnit->setCurrentIndex(streamLineFilter_propData->integration_data.stepUnit-1);
	ui->inte_InitStepLen->setText(tep_str.setNum(streamLineFilter_propData->integration_data.initStepLen,'g',6));
	ui->inte_MiniStepLen->setText(tep_str.setNum(streamLineFilter_propData->integration_data.miniStepLen, 'g', 6));
	ui->inte_MaxiStepLen->setText(tep_str.setNum(streamLineFilter_propData->integration_data.maxiStepLen, 'g', 6));

	ui->stream_MaxiSteps->setText(tep_str.setNum(streamLineFilter_propData->stream_data.maxiSteps));
	ui->stream_MaxiLen_horizontalSlider->setValue(streamLineFilter_propData->stream_data.maxiStreamLen / streamLineFilter_propData->stream_data.old_maxiStreamLen*100);
	ui->stream_MaxiLen_lineEdit->setText(tep_str.setNum(streamLineFilter_propData->stream_data.maxiStreamLen, 'g', 6));
	ui->stream_TerminalSpeed->setText(tep_str.setNum(streamLineFilter_propData->stream_data.terminalSpeed, 'g', 6));
	ui->stream_MaxiError->setText(tep_str.setNum(streamLineFilter_propData->stream_data.maxiError, 'g', 6));
	ui->seeds_Type->setCurrentIndex(streamLineFilter_propData->seeds_data.type);
	ui->seedP1_lineEdit->setText(tep_str.setNum(streamLineFilter_propData->seeds_data.mPoint.x, 'g', 6));
	ui->seedP2_lineEdit->setText(tep_str.setNum(streamLineFilter_propData->seeds_data.mPoint.y, 'g', 6));
	ui->seedP3_lineEdit->setText(tep_str.setNum(streamLineFilter_propData->seeds_data.mPoint.z, 'g', 6));
	ui->seedPoints_spinBox->setValue(streamLineFilter_propData->seeds_data.num_points);
	ui->seedRadius_lineEdit->setText(tep_str.setNum(streamLineFilter_propData->seeds_data.radius, 'g', 6));
	ui->vorticity_checkBox->setChecked(streamLineFilter_propData->vorticity);
    ui->reflection_checkBox->setChecked(cur_pipelineObj->m_reflectionPara.flag_refleciton);
	ui->reflection_groupBox->setVisible(cur_pipelineObj->m_reflectionPara.flag_refleciton);
    if (cur_pipelineObj->m_reflectionPara.flag_refleciton)
    {       
        ui->reflection_comboBox->setCurrentIndex((int)cur_pipelineObj->m_reflectionPara.flag_refleciton);
    }
}

void streamLineFilterPropForm::on_vector_comboBox_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	streamLineFilter_propData->vectorColName = ui->vector_comboBox->currentText();
	if (ui->vector_comboBox->itemData(index).toString().indexOf("val_on_cell.png") >= 0)
		streamLineFilter_propData->vectorColName += "_cell000";
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_streamline_vector(cur_pipelineObj, streamLineFilter_propData->vectorColName);
}

void streamLineFilterPropForm::on_Inte_Direction_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	streamLineFilter_propData->integration_data.direction = (IntegrationDirection)index;
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_streamline_integration_direction(cur_pipelineObj, (IntegrationDirection)index);
}

void streamLineFilterPropForm::on_inte_Type_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	streamLineFilter_propData->integration_data.type = (IntegratorType)index;
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_streamline_integration_type(cur_pipelineObj, streamLineFilter_propData->integration_data.type);
}

void streamLineFilterPropForm::on_inte_StepUnit_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	streamLineFilter_propData->integration_data.stepUnit = (IntegrationStepUnit)(index+1);
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_streamline_integration_stepUnit(cur_pipelineObj, streamLineFilter_propData->integration_data.stepUnit);
}

void streamLineFilterPropForm::on_stream_MaxiLen_horizontalSlider_sliderMoved(int position)
{
	QString str;
	streamLineFilter_propData->stream_data.maxiStreamLen = streamLineFilter_propData->stream_data.old_maxiStreamLen / 100.0*position;
	ui->stream_MaxiLen_lineEdit->setText(str.setNum(streamLineFilter_propData->stream_data.maxiStreamLen,'g',6));
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_streamline_stream_maxiStreamLen(cur_pipelineObj, streamLineFilter_propData->stream_data.maxiStreamLen);

}


void streamLineFilterPropForm::on_seedP1_lineEdit_editingFinished()
{
	bool ok;
	streamLineFilter_propData->seeds_data.mPoint.x = ui->seedP1_lineEdit->text().trimmed().toDouble(&ok);
	double val[3];
	val[0] = streamLineFilter_propData->seeds_data.mPoint.x;
	val[1] = streamLineFilter_propData->seeds_data.mPoint.y;
	val[2] = streamLineFilter_propData->seeds_data.mPoint.z;
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_streamline_seeds_mPoint(cur_pipelineObj, val);

}

void streamLineFilterPropForm::on_seedP2_lineEdit_editingFinished()
{
	bool ok;
	streamLineFilter_propData->seeds_data.mPoint.y = ui->seedP2_lineEdit->text().trimmed().toDouble(&ok);
	double val[3];
	val[0] = streamLineFilter_propData->seeds_data.mPoint.x;
	val[1] = streamLineFilter_propData->seeds_data.mPoint.y;
	val[2] = streamLineFilter_propData->seeds_data.mPoint.z;
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_streamline_seeds_mPoint(cur_pipelineObj, val);
}

void streamLineFilterPropForm::on_seedP3_lineEdit_editingFinished()
{
	bool ok;
	streamLineFilter_propData->seeds_data.mPoint.z = ui->seedP3_lineEdit->text().trimmed().toDouble(&ok);
	double val[3];
	val[0] = streamLineFilter_propData->seeds_data.mPoint.x;
	val[1] = streamLineFilter_propData->seeds_data.mPoint.y;
	val[2] = streamLineFilter_propData->seeds_data.mPoint.z;
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_streamline_seeds_mPoint(cur_pipelineObj, val);
}

void streamLineFilterPropForm::on_seedPoints_spinBox_valueChanged(int arg1)
{
	streamLineFilter_propData->seeds_data.num_points = arg1;
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_streamline_seeds_num_points(cur_pipelineObj, arg1);
}

void streamLineFilterPropForm::on_seedRadius_lineEdit_editingFinished()
{
	bool ok;
	streamLineFilter_propData->seeds_data.radius = ui->seedRadius_lineEdit->text().trimmed().toDouble(&ok);
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_streamline_seeds_radius(cur_pipelineObj, streamLineFilter_propData->seeds_data.radius);
}

void streamLineFilterPropForm::on_seeds_Type_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	streamLineFilter_propData->seeds_data.type = (SpeedType)index;
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_streamline_seeds_type(cur_pipelineObj, streamLineFilter_propData->seeds_data.type);
}

void streamLineFilterPropForm::on_stream_MaxiError_editingFinished()
{
	bool ok;
	streamLineFilter_propData->stream_data.maxiError = ui->stream_MaxiError->text().trimmed().toDouble(&ok);
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_streamline_stream_maxiError(cur_pipelineObj, streamLineFilter_propData->stream_data.maxiError);
}

void streamLineFilterPropForm::on_stream_TerminalSpeed_editingFinished()
{
	bool ok;
	streamLineFilter_propData->stream_data.terminalSpeed = ui->stream_TerminalSpeed->text().trimmed().toDouble(&ok);
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_streamline_stream_terminalSpeed(cur_pipelineObj, streamLineFilter_propData->stream_data.terminalSpeed);
}

void streamLineFilterPropForm::on_stream_MaxiSteps_editingFinished()
{
	bool ok;
	streamLineFilter_propData->stream_data.maxiSteps = ui->stream_MaxiSteps->text().trimmed().toInt(&ok);
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_streamline_stream_maxiSteps(cur_pipelineObj, streamLineFilter_propData->stream_data.maxiSteps);
}

void streamLineFilterPropForm::on_inte_MaxiStepLen_editingFinished()
{
	bool ok;
	streamLineFilter_propData->integration_data.maxiStepLen = ui->stream_MaxiSteps->text().trimmed().toDouble(&ok);
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_streamline_integration_maxiStepLen(cur_pipelineObj, streamLineFilter_propData->integration_data.maxiStepLen);
}

void streamLineFilterPropForm::on_inte_MiniStepLen_editingFinished()
{
	bool ok;
	streamLineFilter_propData->integration_data.miniStepLen = ui->inte_MiniStepLen->text().trimmed().toDouble(&ok);
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_streamline_integration_miniStepLen(cur_pipelineObj, streamLineFilter_propData->integration_data.miniStepLen);
}

void streamLineFilterPropForm::on_inte_InitStepLen_editingFinished()
{
	bool ok;
	streamLineFilter_propData->integration_data.initStepLen = ui->inte_InitStepLen->text().trimmed().toDouble(&ok);
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_streamline_integration_initStepLen(cur_pipelineObj, streamLineFilter_propData->integration_data.initStepLen);
}

void streamLineFilterPropForm::on_interpolatorType_comboBox_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	streamLineFilter_propData->interpolator_type = (InterpolatorType)index;
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_streamline_interpolatorType(cur_pipelineObj, streamLineFilter_propData->interpolator_type);

}

void streamLineFilterPropForm::on_surfaceLines_checkBox_clicked(bool checked)
{
	streamLineFilter_propData->surface_streamLines = checked;
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_streamline_surface_streamLines(cur_pipelineObj, streamLineFilter_propData->surface_streamLines);

}

void streamLineFilterPropForm::on_vorticity_checkBox_clicked(bool checked)
{
	streamLineFilter_propData->vorticity = checked;
	emit sig_enable_applyBtn();
	emit _parentWin->sig_python_streamline_vorticity(cur_pipelineObj, streamLineFilter_propData->vorticity);

}

void streamLineFilterPropForm::reTranslate()
{
	ui->retranslateUi(this);
}

void streamLineFilterPropForm::on_reflection_checkBox_clicked(bool checked)
{
    ui->reflection_groupBox->setVisible(checked);
    cur_pipelineObj->m_reflectionPara.flag_refleciton = checked;
	if (checked)
    {
        ui->reflection_comboBox->setCurrentIndex(cur_pipelineObj->m_reflectionPara.type);
    }
    int val = 0;
    if (checked)
        val = 1;
    emit _parentWin->sig_python_streamline_reflection(cur_pipelineObj,val);
    emit sig_enable_applyBtn();
}

void streamLineFilterPropForm::on_reflection_comboBox_currentIndexChanged(int index)
{
	if (index == -1)
		return;
    cur_pipelineObj->m_reflectionPara.type = (ReflectionPlane)index;
    emit _parentWin->sig_python_streamline_reflectionAxes(cur_pipelineObj, index);
    emit sig_enable_applyBtn();
}
void streamLineFilterPropForm::initSeedForm()
{
	QString tep_str;
	ui->seedP1_lineEdit->setText(tep_str.setNum(streamLineFilter_propData->seeds_data.mPoint.x, 'g', 6));
	ui->seedP2_lineEdit->setText(tep_str.setNum(streamLineFilter_propData->seeds_data.mPoint.y, 'g', 6));
	ui->seedP3_lineEdit->setText(tep_str.setNum(streamLineFilter_propData->seeds_data.mPoint.z, 'g', 6));
	ui->seedRadius_lineEdit->setText(tep_str.setNum(streamLineFilter_propData->seeds_data.radius, 'g', 6));
}
