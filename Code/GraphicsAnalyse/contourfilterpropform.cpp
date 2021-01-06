#include "contourfilterpropform.h"
#include "ui_contourfilterpropform.h"

contourFilterPropForm::contourFilterPropForm(PipelineObjPropWidget *parent) :
QWidget(parent), _parentWin(parent),
    ui(new Ui::contourFilterPropForm)
{
    ui->setupUi(this);
	contourFilter_propData = NULL;
	cur_pipelineObj = NULL;
}

contourFilterPropForm::~contourFilterPropForm()
{
    delete ui;
}

void contourFilterPropForm::updateForm()
{
	QString tep_str;
	ui->comboBox->blockSignals(true);
	ui->comboBox->clear();
	
	for (int i = 0; i < contourFilter_propData->contour_dataList.count(); i++)
	{
		tep_str = contourFilter_propData->contour_dataList.at(i).colName;
		QIcon icon3 = QIcon::fromTheme("val_on_point.png", QIcon(":/images/val_on_point.png"));
		ui->comboBox->addItem(icon3, tep_str, "val_on_point.png");
	}
	ui->comboBox->blockSignals(false);
	if (ui->comboBox->count() > 0)
	{
		int index = 0;
		QString tepStr;
		for (int i = 0; i < ui->comboBox->count(); i++)
		{
			tepStr = ui->comboBox->itemText(i);
			if (ui->comboBox->itemText(i) == contourFilter_propData->contour_colName)
			{
				index = i;
				break;
			}
		}
		ui->comboBox->setCurrentIndex(index);
		on_comboBox_currentIndexChanged(index);
		
	}
	ui->reflection_checkBox->setChecked(cur_pipelineObj->m_reflectionPara.flag_refleciton);
	ui->reflection_groupBox->setVisible(cur_pipelineObj->m_reflectionPara.flag_refleciton);
	if (cur_pipelineObj->m_reflectionPara.flag_refleciton)
	{		
		ui->reflection_comboBox->setCurrentIndex((int)cur_pipelineObj->m_reflectionPara.flag_refleciton);
	}
}

void contourFilterPropForm::on_val_spinbox_valueChanged(double arg1)
{
	if (cur_pipelineObj == NULL)
		return;
	if (contourFilter_propData == NULL)
		return;
	contour_PropData tep_data;
	for (int i = 0; i < contourFilter_propData->contour_dataList.count(); i++)
	{
		if (contourFilter_propData->contour_dataList.at(i).colName == contourFilter_propData->contour_colName)
		{
			tep_data = contourFilter_propData->contour_dataList.at(i);
			tep_data.val = arg1;
			contourFilter_propData->contour_dataList.replace(i, tep_data);
			break;
		}
	}
	emit _parentWin->sig_python_contourValue(cur_pipelineObj, arg1);
	emit sig_enable_applyBtn();
}

void contourFilterPropForm::on_comboBox_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	if (cur_pipelineObj == NULL)
		return;
	if (contourFilter_propData == NULL)
		return;
	contourFilter_propData->contour_colName = contourFilter_propData->contour_dataList.at(index).colName;
	ui->val_spinbox->setRange(contourFilter_propData->contour_dataList.at(index).min, contourFilter_propData->contour_dataList.at(index).max);
	ui->val_spinbox->setSingleStep((contourFilter_propData->contour_dataList.at(index).max-contourFilter_propData->contour_dataList.at(index).min)/100.0);
	ui->val_spinbox->setValue(contourFilter_propData->contour_dataList.at(index).val);

	/*contourFilter_propData->contour_colName = ui->comboBox->currentText().trimmed();
	if (ui->comboBox->itemData(index).toString().indexOf("val_on_cell.png") >= 0)
		contourFilter_propData->contour_colName += "_cell000";*/
	emit _parentWin->sig_python_contourColumn(cur_pipelineObj, contourFilter_propData->contour_colName);
	emit sig_enable_applyBtn();
}

void contourFilterPropForm::reTranslate()
{
	ui->retranslateUi(this);
}

void contourFilterPropForm::on_reflection_checkBox_clicked(bool checked)
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
	emit _parentWin->sig_python_contour_reflection(cur_pipelineObj,val);
	emit sig_enable_applyBtn();
}

void contourFilterPropForm::on_reflection_comboBox_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	cur_pipelineObj->m_reflectionPara.type = (ReflectionPlane)index;
	emit _parentWin->sig_python_contour_reflectionAxes(cur_pipelineObj, index);
	emit sig_enable_applyBtn();
}
