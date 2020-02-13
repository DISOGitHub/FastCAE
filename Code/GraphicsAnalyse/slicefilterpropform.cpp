#include "slicefilterpropform.h"
#include "ui_slicefilterpropform.h"
sliceFilterPropForm::sliceFilterPropForm(PipelineObjPropWidget *parent) :
QWidget(parent), _parentWin(parent),
    ui(new Ui::sliceFilterPropForm)
{
    ui->setupUi(this);
	plane_propData = NULL;
	cur_pipelineObj = NULL;
}

sliceFilterPropForm::~sliceFilterPropForm()
{
    delete ui;
}

void sliceFilterPropForm::updateForm()
{
	QString tep_str;
	ui->checkBox->setChecked(plane_propData->flag_plane);
	ui->m_planeOrigin_lineEdit1->setText(tep_str.setNum(plane_propData->m_plane_origin.x, 'g', 3));
	ui->m_planeOrigin_lineEdit2->setText(tep_str.setNum(plane_propData->m_plane_origin.y, 'g', 3));
	ui->m_planeOrigin_lineEdit3->setText(tep_str.setNum(plane_propData->m_plane_origin.z, 'g', 3));
	ui->m_planeNormal_lineEdit1->setText(tep_str.setNum(plane_propData->m_plane_normal.x, 'g', 3));
	ui->m_planeNormal_lineEdit2->setText(tep_str.setNum(plane_propData->m_plane_normal.y, 'g', 3));
	ui->m_planeNormal_lineEdit3->setText(tep_str.setNum(plane_propData->m_plane_normal.z, 'g', 3));
	if (cur_pipelineObj != NULL)
	{
		if (cur_pipelineObj->GetObjectType() == dClip_DataSource)
		{
			ui->insideOut_checkBox->setVisible(true);
			ui->insideOut_checkBox->setChecked(plane_propData->flag_insideOut);
		}
		else
			ui->insideOut_checkBox->setVisible(false);
	}
}

void sliceFilterPropForm::on_m_planeOrigin_lineEdit1_editingFinished()
{
	if (cur_pipelineObj == NULL)
		return;
	if (plane_propData == NULL)
		return;
	bool ok;
	origins[0] = ui->m_planeOrigin_lineEdit1->text().toDouble(&ok);
	plane_propData->m_plane_origin.x = origins[0];
	emit sig_update_planeOrigin(cur_pipelineObj);
	emit _parentWin->sig_python_planeOrigin(cur_pipelineObj);
	emit sig_enable_applyBtn();
}

void sliceFilterPropForm::on_m_planeOrigin_lineEdit2_editingFinished()
{
	if (cur_pipelineObj == NULL)
		return;
	if (plane_propData == NULL)
		return;
	bool ok;
	origins[1] = ui->m_planeOrigin_lineEdit2->text().toDouble(&ok);
	plane_propData->m_plane_origin.y = origins[1];
	emit sig_update_planeOrigin(cur_pipelineObj);
	emit _parentWin->sig_python_planeOrigin(cur_pipelineObj);
	emit sig_enable_applyBtn();
}

void sliceFilterPropForm::on_m_planeOrigin_lineEdit3_editingFinished()
{
	if (cur_pipelineObj == NULL)
		return;
	if (plane_propData == NULL)
		return;
	bool ok;
	origins[2] = ui->m_planeOrigin_lineEdit3->text().toDouble(&ok);
	plane_propData->m_plane_origin.z = origins[2];
	emit sig_update_planeOrigin(cur_pipelineObj);
	emit _parentWin->sig_python_planeOrigin(cur_pipelineObj);
	emit sig_enable_applyBtn();
}

void sliceFilterPropForm::on_m_planeNormal_lineEdit1_editingFinished()
{
	if (cur_pipelineObj == NULL)
		return;
	if (plane_propData == NULL)
		return;
	bool ok;
	normals[0] = ui->m_planeNormal_lineEdit1->text().toDouble(&ok);
	plane_propData->m_plane_normal.x = normals[0];
	emit sig_update_planeNormal(cur_pipelineObj);
	emit _parentWin->sig_python_planeNormal(cur_pipelineObj);
	emit sig_enable_applyBtn();
}

void sliceFilterPropForm::on_m_planeNormal_lineEdit2_editingFinished()
{
	if (cur_pipelineObj == NULL)
		return;
	if (plane_propData == NULL)
		return;
	bool ok;
	normals[1] = ui->m_planeNormal_lineEdit2->text().toDouble(&ok);
	plane_propData->m_plane_normal.y = normals[1];
	emit sig_update_planeNormal(cur_pipelineObj);
	emit _parentWin->sig_python_planeNormal(cur_pipelineObj);
	emit sig_enable_applyBtn();
}

void sliceFilterPropForm::on_m_planeNormal_lineEdit3_editingFinished()
{
	if (cur_pipelineObj == NULL)
		return;
	if (plane_propData == NULL)
		return;
	bool ok;
	normals[2] = ui->m_planeNormal_lineEdit3->text().toDouble(&ok);
	plane_propData->m_plane_normal.z = normals[2];
	emit sig_update_planeNormal(cur_pipelineObj);
	emit _parentWin->sig_python_planeNormal(cur_pipelineObj);
	emit sig_enable_applyBtn();
}

void sliceFilterPropForm::on_checkBox_clicked(bool checked)
{
	if (cur_pipelineObj == NULL)
		return;
	if (plane_propData == NULL)
		return;
	plane_propData->flag_plane = checked;
	emit sig_update_planeVis(cur_pipelineObj);
	emit _parentWin->sig_python_planeVis(cur_pipelineObj);
	emit sig_enable_applyBtn();
}

void sliceFilterPropForm::on_xNormal_btn_clicked()
{
	if (cur_pipelineObj == NULL)
		return;
	if (plane_propData == NULL)
		return;
	ui->m_planeNormal_lineEdit1->setText("1");
	ui->m_planeNormal_lineEdit2->setText("0");
	ui->m_planeNormal_lineEdit3->setText("0");
	plane_propData->m_plane_normal.x = 1;
	plane_propData->m_plane_normal.y = 0;
	plane_propData->m_plane_normal.z = 0;
	emit sig_update_planeNormal(cur_pipelineObj);
	emit _parentWin->sig_python_planeNormal(cur_pipelineObj);
	emit sig_enable_applyBtn();
}

void sliceFilterPropForm::on_yNormal_btn_clicked()
{
	if (cur_pipelineObj == NULL)
		return;
	if (plane_propData == NULL)
		return;
	ui->m_planeNormal_lineEdit1->setText("0");
	ui->m_planeNormal_lineEdit2->setText("1");
	ui->m_planeNormal_lineEdit3->setText("0");
	plane_propData->m_plane_normal.x = 0;
	plane_propData->m_plane_normal.y = 1;
	plane_propData->m_plane_normal.z = 0;
	emit sig_update_planeNormal(cur_pipelineObj);
	emit _parentWin->sig_python_planeNormal(cur_pipelineObj);
	emit sig_enable_applyBtn();
}

void sliceFilterPropForm::on_zNormal_btn_clicked()
{
	if (cur_pipelineObj == NULL)
		return;
	if (plane_propData == NULL)
		return;
	ui->m_planeNormal_lineEdit1->setText("0");
	ui->m_planeNormal_lineEdit2->setText("0");
	ui->m_planeNormal_lineEdit3->setText("1");
	plane_propData->m_plane_normal.x = 0;
	plane_propData->m_plane_normal.y = 0;
	plane_propData->m_plane_normal.z = 1;
	emit sig_update_planeNormal(cur_pipelineObj);
	emit _parentWin->sig_python_planeNormal(cur_pipelineObj);
	emit sig_enable_applyBtn();
}

void sliceFilterPropForm::slot_update_planeProp_widget(double* origins_val, double* normals_val)
{
	if (cur_pipelineObj == NULL)//20171024 add
		return;
	QString str;
	ui->m_planeOrigin_lineEdit1->setText(str.setNum(origins_val[0], 'g', 6));
	ui->m_planeOrigin_lineEdit2->setText(str.setNum(origins_val[1], 'g', 6));
	ui->m_planeOrigin_lineEdit3->setText(str.setNum(origins_val[2], 'g', 6));
	ui->m_planeNormal_lineEdit1->setText(str.setNum(normals_val[0], 'g', 6));
	ui->m_planeNormal_lineEdit2->setText(str.setNum(normals_val[1], 'g', 6));
	ui->m_planeNormal_lineEdit3->setText(str.setNum(normals_val[2], 'g', 6));
	plane_propData->m_plane_origin.x = origins_val[0];
	plane_propData->m_plane_origin.y = origins_val[1];
	plane_propData->m_plane_origin.z = origins_val[2];
	plane_propData->m_plane_normal.x = normals_val[0];
	plane_propData->m_plane_normal.y = normals_val[1];
	plane_propData->m_plane_normal.z = normals_val[2];
	emit sig_enable_applyBtn();
}

void sliceFilterPropForm::on_cameraNormal_btn_clicked()
{
	emit sig_update_planeCameraNormal(cur_pipelineObj);
	emit _parentWin->sig_python_planeNormal(cur_pipelineObj);
	emit sig_enable_applyBtn();
}

void sliceFilterPropForm::on_insideOut_checkBox_clicked(bool checked)
{
	if (cur_pipelineObj == NULL)
		return;
	if (plane_propData == NULL)
		return;
	plane_propData->flag_insideOut = checked;
	emit _parentWin->sig_python_insideOut(cur_pipelineObj);
	emit sig_enable_applyBtn();
}

void sliceFilterPropForm::reTranslate()
{
	ui->retranslateUi(this);
}
