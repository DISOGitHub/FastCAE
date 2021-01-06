#include "scalarbareditform.h"
#include "ui_scalarbareditform.h"
#include "ctkVTKScalarsToColorsWidget.h"
#include "ctkVTKScalarsToColorsView.h"
#include "scalarrangedialog.h"
#include "vtkDiscretizableColorTransferFunction.h"
#include "qdebug.h"
#include "vtkScalarBarWidget.h"
#include "global.h"
#include "vtkAxis.h"
#include "qabstractitemview.h"
#include "qtextcodec.h"
#include "vtkTextProperty.h"
ScalarBarEditForm::ScalarBarEditForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScalarBarEditForm)
{
    ui->setupUi(this);
	cur_PipelineObj = NULL;
	ui->groupBox->setVisible(false);
	colorTransferFunction=NULL;
	opacityFunction=NULL;
	flag_init = false;
	double validBounds[4] = { VTK_DOUBLE_MIN, VTK_DOUBLE_MAX, 0., 1. };
	ui->ScalarOpacityWidget->view()->setValidBounds(validBounds);
	ui->ScalarColorWidget->view()->setValidBounds(validBounds);
	QObject::connect(ui->ScalarOpacityWidget->view(), SIGNAL(extentChanged()),this, SIGNAL(chartsExtentChanged()));
	
	QObject::connect(ui->ScalarColorWidget, SIGNAL(axesModified()), this, SLOT(onAxesModified()), Qt::QueuedConnection);
	QObject::connect(ui->ScalarColorWidget, SIGNAL(sig_udpate_scalarBarTable()), this, SLOT(slot_update_scalarBarTable()));
	QObject::connect(ui->ScalarOpacityWidget, SIGNAL(axesModified()), this, SLOT(onAxesModified()), Qt::QueuedConnection);
	QObject::connect(ui->ScalarOpacityWidget, SIGNAL(sig_udpate_scalarBarTable()), this, SLOT(slot_update_scalarBarTable()));
	connect(ui->zone_tableWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onItemClicked(QTableWidgetItem*)));
	ui->opacity_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->color_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tabWidget->tabBar()->setVisible(false);
}

ScalarBarEditForm::~ScalarBarEditForm()
{
    delete ui;
}

void ScalarBarEditForm::slot_update_scalarBarTable()
{
	//emit sig_update_plot_pipelineObj(cur_PipelineObj, colorTransferFunction, opacityFunction);
	int num1 = colorTransferFunction->GetSize();
	ui->color_tableWidget->setRowCount(num1);
	double *val = colorTransferFunction->GetDataPointer();
	QString tep;
	for (int i = 0; i < num1; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ui->color_tableWidget->setItem(i, j, new QTableWidgetItem(tep.setNum(val[i * 4 + j], 'g', 6)));
		}
	}
	int num2 = opacityFunction->GetSize();
	ui->opacity_tableWidget->setRowCount(num2);
	val = opacityFunction->GetDataPointer();
	for (int i = 0; i < num2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			ui->opacity_tableWidget->setItem(i, j, new QTableWidgetItem(tep.setNum(val[i * 2 + j], 'g', 6)));
		}
	}
	
	/*if (ui->enableOpacity_checkBox->isChecked())
		on_enableOpacity_checkBox_clicked(true);
	else
		on_enableOpacity_checkBox_clicked(false);*/
}

void ScalarBarEditForm::slot_update_zoneTable()
{
	if (colorTransferFunction == NULL)
		return;
	int num1 = colorTransferFunction->GetSize();
	int tep_num_row = num1 / 2;
	if (num1 % 2)
		tep_num_row++;
	ui->zone_tableWidget->setRowCount(tep_num_row);
	double *val = colorTransferFunction->GetDataPointer(); 
	QString tep;
	double tep_rgb[4];
	for (int i = 0; i < num1; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tep_rgb[j] = val[i * 4 + j];
		}
		int tep_col = 0;
		if (i % 2)
			tep_col = 2;
		ui->zone_tableWidget->setItem(i / 2, tep_col, new QTableWidgetItem(tep.setNum(i,10)));
		QPushButton* tep_color_btn = new QPushButton(" ");
		tep_color_btn->setStyleSheet(QString("border:2px groove gray;border-radius:10px;padding:2px 4px; background-color: rgb(%1,%2,%3)").arg(tep_rgb[1] * 255).arg(tep_rgb[2] * 255).arg(tep_rgb[3] * 255));
		connect(tep_color_btn, SIGNAL(clicked()), this, SLOT(OnZoneColorBtnClicked()));
		ui->zone_tableWidget->setCellWidget(i/2, tep_col+1, tep_color_btn);
	}
	ui->zone_tableWidget->resizeColumnsToContents();
}

void ScalarBarEditForm::OnZoneColorBtnClicked()
{
	QPushButton *senderObj = qobject_cast<QPushButton*>(sender());
	if (senderObj == nullptr)
	{
		return;
	}
	QModelIndex idx = ui->zone_tableWidget->indexAt(QPoint(senderObj->frameGeometry().x(), senderObj->frameGeometry().y()));
	int row = idx.row();
	int col = idx.column();
	if (!(col % 2))
		return;
	double annotation;;
	if (col == 1)
		annotation = row * 2;
	else
		annotation = row * 2 + 1;
	QPushButton* tep_btn = (QPushButton*)ui->zone_tableWidget->cellWidget(row, col);
	double tep_rgb[3];
	colorTransferFunction->GetColor(annotation, tep_rgb);
	QColor init_color0 = QColor(tep_rgb[0] * 255, tep_rgb[1] * 255, tep_rgb[2] * 255);
	QColor color = QColorDialog::getColor(init_color0, this,tr("color dialog"));
	if (!color.isValid())
		return;
	int r = color.red();
	int g = color.green();
	int b = color.blue();
	tep_btn->setStyleSheet(QString("border:2px groove gray;border-radius:10px;padding:2px 4px; background-color: rgb(%1,%2,%3)").arg(r).arg(g).arg(b));
	colorTransferFunction->AddRGBPoint(annotation, r / 255.0, g / 255.0, b / 255.0);
}

void ScalarBarEditForm::computeRange(double* range)
{
	if ((colorTransferFunction == NULL) || (opacityFunction == NULL))
	{
		range[0] = 0.;
		range[1] = 1.;
		return;
	}
	range[0] = VTK_DOUBLE_MAX;
	range[1] = VTK_DOUBLE_MIN;

	double colorRange[2] = { 0., 1. };
	colorTransferFunction->GetRange(colorRange);
	range[0] = qMin(range[0], colorRange[0]);
	range[1] = qMax(range[1], colorRange[1]);

	double opacityRange[2] = { 0., 1. };
	opacityFunction->GetRange(opacityRange);
	range[0] = qMin(range[0], opacityRange[0]);
	range[1] = qMax(range[1], opacityRange[1]);
}

void ScalarBarEditForm::updateRange()
{
	double range[2];
	computeRange(range);

	double chartBounds[8];
	ui->ScalarOpacityWidget->view()->chartBounds(chartBounds);
	chartBounds[2] = range[0];
	chartBounds[3] = range[1];
	ui->ScalarOpacityWidget->view()->setChartUserBounds(chartBounds);
	ui->ScalarOpacityWidget->view()->update();

	ui->ScalarColorWidget->view()->chartBounds(chartBounds);
	chartBounds[2] = range[0];
	chartBounds[3] = range[1];
	ui->ScalarColorWidget->view()->setChartUserBounds(chartBounds);
	ui->ScalarColorWidget->view()->update();
}

void ScalarBarEditForm::chartsBounds(double bounds[4])const
{
	double chartBounds[4];
	ui->ScalarOpacityWidget->view()->chartBounds(chartBounds);
	memcpy(bounds, chartBounds, 4 * sizeof(double));
	ui->ScalarColorWidget->view()->chartBounds(chartBounds);
	bounds[0] = qMin(bounds[0], chartBounds[0]);
	bounds[1] = qMax(bounds[1], chartBounds[1]);
	bounds[2] = qMin(bounds[2], chartBounds[2]);
	bounds[3] = qMax(bounds[3], chartBounds[3]);
}

void ScalarBarEditForm::chartsExtent(double extent[4])const
{
	double chartExtent[8];
	ui->ScalarOpacityWidget->view()->chartExtent(chartExtent);
	memcpy(extent, chartExtent, 4 * sizeof(double));
	ui->ScalarColorWidget->view()->chartExtent(chartExtent);
	extent[0] = qMin(extent[0], chartExtent[0]);
	extent[1] = qMax(extent[1], chartExtent[1]);
	extent[2] = qMin(extent[2], chartExtent[2]);
	extent[3] = qMax(extent[3], chartExtent[3]);
}

void ScalarBarEditForm::moveAllPoints(double xOffset, double yOffset,bool dontMoveFirstAndLast)
{
	ui->ScalarOpacityWidget->view()->moveAllPoints(xOffset, yOffset, dontMoveFirstAndLast);
	ui->ScalarColorWidget->view()->moveAllPoints(xOffset, yOffset, dontMoveFirstAndLast);
}

void ScalarBarEditForm::spreadAllPoints(double factor, bool dontSpreadFirstAndLast)
{
	ui->ScalarOpacityWidget->view()->spreadAllPoints(factor, dontSpreadFirstAndLast);
	ui->ScalarColorWidget->view()->spreadAllPoints(factor, dontSpreadFirstAndLast);
}

void ScalarBarEditForm::onAxesModified()
{
	ctkVTKScalarsToColorsWidget* senderWidget =
		qobject_cast<ctkVTKScalarsToColorsWidget*>(this->sender());

	double xRange[2] = { 0., 0. };
	senderWidget->xRange(xRange);
	if (senderWidget != ui->ScalarOpacityWidget)
	{
		bool wasBlocking = ui->ScalarOpacityWidget->blockSignals(true);
		ui->ScalarOpacityWidget->setXRange(xRange[0], xRange[1]);
		ui->ScalarOpacityWidget->blockSignals(wasBlocking);
	}
	if (senderWidget != ui->ScalarColorWidget)
	{
		bool wasBlocking = ui->ScalarColorWidget->blockSignals(true);
		ui->ScalarColorWidget->setXRange(xRange[0], xRange[1]);
		ui->ScalarColorWidget->blockSignals(wasBlocking);
	}
}

void ScalarBarEditForm::enable_components(bool flag)
{
	ui->groupBox->setEnabled(flag);
	ui->ScalarColorWidget->setEnabled(flag);
	ui->ScalarOpacityWidget->setEnabled(flag);
	ui->scalarRange_customBtn->setEnabled(flag);
	ui->scalarRange_dataBtn->setEnabled(flag);
	ui->scalarRange_tableBtn->setEnabled(flag);
}

void ScalarBarEditForm::clearAll()
{
	ui->colName_lineEdit->setText(" ");
	ui->tabWidget->setCurrentIndex(2);
	cur_PipelineObj = NULL;
}

void ScalarBarEditForm::updateForm(PipelineObject* tep_PipelineObj)
{
	cur_PipelineObj = tep_PipelineObj;
	if (cur_PipelineObj == NULL)
	{
		return;
	}
	if (tep_PipelineObj->mVecActors.count() < 1)
	{
		ui->tabWidget->setCurrentIndex(2);
		return;
	}
	QString colName = tep_PipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
	//QString m_title;
	PipelineObject* tep_obj = NULL;
	if (tep_PipelineObj->GetObjectType() == dNumeric_DataSource)
		tep_obj = tep_PipelineObj;
	else
		tep_obj = tep_PipelineObj->GetObjParent();
	/*if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains(colName))
	{
		scalarBarTitle_PropData tep_titlePara = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap[colName];
		m_title = tep_titlePara.title;
	}
	else
	{
		m_title = colName;
	}
	if (m_title.indexOf("_cell000") >= 0)
		m_title = m_title.left(m_title.length() - 8);
	ui->colName_lineEdit->setText(m_title);*/
	if (colName == "SolidColor")
	{
		ui->tabWidget->setCurrentIndex(2);
		ui->spinBox_numLabels->setEnabled(false);
	}
	else if (colName == "vtkBlockColors")
	{
		ui->tabWidget->setCurrentIndex(1);
		/*if (tep_PipelineObj->GetObjectType()==dNumeric_DataSource)
			colorTransferFunction = tep_PipelineObj->getLookupTable(tep_PipelineObj->mBaseGridBak, tep_PipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle);
		else*/
		colorTransferFunction = tep_obj->getLookupTable(tep_obj->mBaseGridBak, tep_PipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle);
		slot_update_zoneTable();
		ui->spinBox_numLabels->setEnabled(false);
	}
	else
	{
		ui->tabWidget->setCurrentIndex(0);
		ui->spinBox_numLabels->setEnabled(true);
		scalarBarTitle_PropData tep_titleProp;
		if ((tep_PipelineObj->GetObjectType() == dCalculator_DataSource) && (colName == tep_PipelineObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
		{
			if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains("CalculatorResult"))
			{
				tep_titleProp = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap["CalculatorResult"];
			}
			else
			{
				tep_titleProp.num_lable = scalarBar_NumLables;
				tep_titleProp.size = scalarBar_FontSize;
				tep_titleProp.title = colName;
			}
		}
		else
		{
			if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains(colName))
			{
				tep_titleProp = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap[colName];
			}
			else
			{
				tep_titleProp.num_lable = scalarBar_NumLables;
				tep_titleProp.size = scalarBar_FontSize;
				tep_titleProp.title = colName;
			}
		}
		QString m_title = tep_titleProp.title;
		/*if (m_title.indexOf("_cell000") >= 0)
			m_title = m_title.left(m_title.length() - 8);*/

		ui->spinBox->setValue(tep_titleProp.size);
		ui->spinBox_numLabels->setValue(tep_titleProp.num_lable);
		ui->colName_lineEdit->setText(m_title);
		bool flag = false;
		double tep_range[2];
		if (tep_PipelineObj->GetObjectType() == dVector_DataSource)
		{
			if (tep_PipelineObj->get_column_range(tep_PipelineObj->mBaseGrid, colName, tep_range))
			{
				if (tep_range[0] != tep_range[1])
					flag = true;
			}
		}
		else if ((tep_PipelineObj->GetObjectType() == dCalculator_DataSource)
			&&(tep_PipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle==tep_PipelineObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
		{
			if (tep_PipelineObj->get_column_range(tep_PipelineObj->mPipelineDataSet, colName, tep_range))
			{
				if (tep_range[0] != tep_range[1])
					flag = true;
			}
		}
		else
		{
			if (tep_obj->get_column_range(tep_PipelineObj->mBaseGrid, colName, tep_range))
			{
				if (tep_range[0] != tep_range[1])
					flag = true;
			}
		}
		if ((tep_PipelineObj->GetObjectType() == dCalculator_DataSource) && (colName == tep_PipelineObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
		{
			colorTransferFunction = tep_obj->getLookupTable(tep_obj->mBaseGridBak, "CalculatorResult");
			opacityFunction = tep_obj->getLookupOpacity(tep_obj->mBaseGridBak, "CalculatorResult");
		}
		else
		{
			colorTransferFunction = tep_obj->getLookupTable(tep_obj->mBaseGridBak, tep_PipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle);
			opacityFunction = tep_obj->getLookupOpacity(tep_obj->mBaseGridBak, tep_PipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle);
		}
		if ((colorTransferFunction == NULL) || (opacityFunction == NULL))
			return;
		enable_components(flag);
		if (!flag)
			return;
		/*if (cur_PipelineObj == tep_PipelineObj)
			return;
		cur_PipelineObj = tep_PipelineObj;*/
		
		cur_actor = cur_PipelineObj->mVecActors[0];

		if (!flag_init)
		{
			ui->ScalarOpacityWidget->view()->addCompositeFunction(0, 0, true, true);
			vtkCompositeControlPointsItem* composite = vtkCompositeControlPointsItem::SafeDownCast(ui->ScalarOpacityWidget->view()->opacityFunctionPlots()[1]);
			composite->SetColorFill(true);
			composite->SetPointsFunction(vtkCompositeControlPointsItem::OpacityPointsFunction);
			ui->ScalarColorWidget->view()->addColorTransferFunction(0, true);
			flag_init = true;
		}
		ui->ScalarOpacityWidget->view()->setOpacityFunctionToPlots(opacityFunction);
		ui->ScalarOpacityWidget->view()->setColorTransferFunctionToPlots(colorTransferFunction);
		ui->ScalarColorWidget->view()->setColorTransferFunctionToPlots(colorTransferFunction);
		//ui->ScalarColorWidget->view()->setOpacityFunctionToPlots(opacityFunction);

		vtkSmartPointer<vtkEventQtSlotConnect> tep_connector = vtkSmartPointer<vtkEventQtSlotConnect>::New();;
		tep_connector->Disconnect(0, vtkCommand::EndInteractionEvent, this, SLOT(updateRange()));
		tep_connector->Connect(opacityFunction, vtkCommand::EndInteractionEvent, this, SLOT(updateRange()), 0, 0., Qt::QueuedConnection);
		tep_connector->Connect(opacityFunction, vtkCommand::EndEvent, this, SLOT(updateRange()), 0, 0., Qt::QueuedConnection);
		tep_connector->Connect(colorTransferFunction, vtkCommand::EndInteractionEvent, this, SLOT(updateRange()), 0, 0., Qt::QueuedConnection);
		tep_connector->Connect(colorTransferFunction, vtkCommand::EndEvent, this, SLOT(updateRange()), 0, 0., Qt::QueuedConnection);

		updateRange();
		slot_update_scalarBarTable();
		this->resize(this->size().width()*0.99, this->size().height());
		/////////////////////////////////////////////////////////////
		if ((tep_PipelineObj->GetObjectType() == dCalculator_DataSource) && (colName == tep_PipelineObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
		{
			if (tep_obj->scalarBar_widgetMap.contains("CalculatorResult"))
			{
				vtkScalarBarWidget* tep_scalarWidget = tep_obj->scalarBar_widgetMap["CalculatorResult"];
				if (tep_scalarWidget != NULL)
				{
					ui->spinBox_numLabels->setValue(tep_scalarWidget->GetScalarBarActor()->GetNumberOfLabels());
				}
			}
		}
		else
		{
			if (tep_obj->scalarBar_widgetMap.contains(colName))
			{
				vtkScalarBarWidget* tep_scalarWidget = tep_obj->scalarBar_widgetMap[colName];
				if (tep_scalarWidget != NULL)
				{
					ui->spinBox_numLabels->setValue(tep_scalarWidget->GetScalarBarActor()->GetNumberOfLabels());
				}
			}
		}
		////////////////////////////////////////////////////////////////////////
	}
	if (cur_PipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible)
	{
		PipelineObject* tep_obj = NULL;
		if (cur_PipelineObj->GetObjectType() == dNumeric_DataSource)
			tep_obj = cur_PipelineObj;
		else
			tep_obj = cur_PipelineObj->GetObjParent();
		if ((tep_PipelineObj->GetObjectType() == dCalculator_DataSource) && (colName == tep_PipelineObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
		{
			if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.contains("CalculatorResult"))
			{
				bool tep_val = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap["CalculatorResult"];
				ui->enableOpacity_checkBox->setChecked(tep_val);
			}
			else
				ui->enableOpacity_checkBox->setChecked(false);
		}
		else
		{
			if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.contains(colName))
			{
				bool tep_val = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap[colName];
				ui->enableOpacity_checkBox->setChecked(tep_val);
			}
			else
				ui->enableOpacity_checkBox->setChecked(false);
		}
	}
}

void ScalarBarEditForm::on_scalarRange_dataBtn_clicked()
{
	bool flag = false;
	double tep_val[2];
	QString tep = cur_PipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
	if (tep.indexOf("_cell000") >= 0)
	{
		tep = tep.left(tep.length() - 8);
		vtkDataArray* tep_arr=cur_PipelineObj->mBaseGrid->GetCellData()->GetArray(tep.toStdString().data());
		if (tep_arr != NULL)
		{
			flag = true;
			tep_arr->GetRange(tep_val);
			//colorTransferFunction->AdjustRange(tep_arr->GetRange());
			//opacityFunction->AdjustRange(tep_arr->GetRange());
		}
	}
	else
	{
		vtkDataArray* tep_arr = cur_PipelineObj->mBaseGrid->GetPointData()->GetArray(tep.toStdString().data());
		if (tep_arr != NULL)
		{
			flag = true;
			tep_arr->GetRange(tep_val);
			//colorTransferFunction->AdjustRange(tep_arr->GetRange());
			//opacityFunction->AdjustRange(tep_arr->GetRange());
		}
	}
	if (flag)
	{
		slot_update_scalarRange(tep_val);
	}
}

void ScalarBarEditForm::on_scalarRange_customBtn_clicked()
{
	scalarRangeDialog* dlg = new scalarRangeDialog;
	connect(dlg, SIGNAL(sig_update_scalarRange(double*)), this, SLOT(slot_update_scalarRange(double*)));
	dlg->updateForm(colorTransferFunction->GetRange());
	dlg->exec();
	delete dlg;
	dlg = NULL;
}

void ScalarBarEditForm::on_scalarRange_tableBtn_clicked()
{
	if (ui->groupBox->isVisible())
		ui->groupBox->setVisible(false);
	else
	{
		ui->groupBox->setVisible(true);
		slot_update_scalarBarTable();
	}
}

void ScalarBarEditForm::modify_scalarBarTable(double val[2])
{
	if (val[1] <= val[0])
		return;
	double new_range = val[1] - val[0];
	double cur_range[2];
	colorTransferFunction->GetRange(cur_range);
	if (cur_range[1] <= cur_range[0])
		return;
	double old_range = cur_range[1] - cur_range[0];

	double *val1 = colorTransferFunction->GetDataPointer();
	colorTransForm_val color_val;
	QVector<colorTransForm_val> color_val_list; color_val_list.clear();
	int num1 = colorTransferFunction->GetSize();
	for (int i = 0; i < num1; i++)
	{
		color_val.val = val1[i * 4];
		color_val.r = val1[i * 4+1];
		color_val.g = val1[i * 4 + 2];
		color_val.b = val1[i * 4 + 3];
		color_val_list.append(color_val);
	}
	colorTransferFunction->RemoveAllPoints();
	for (int i = 0; i < num1; i++)
	{
		color_val = color_val_list.at(i);
		color_val.val = (color_val.val - cur_range[0])*new_range / old_range + val[0];
		colorTransferFunction->AddRGBPoint(color_val.val, color_val.r, color_val.g, color_val.b);
	}
	
	colorTransferFunction->Build();
	int num2 = opacityFunction->GetSize();
	double* val2 = opacityFunction->GetDataPointer();
	opacityTransForm_val opa_val;
	QVector<opacityTransForm_val> opa_val_list; opa_val_list.clear();
	for (int i = 0; i < num2; i++)
	{
		opa_val.val = val2[i * 2];
		opa_val.opa = val2[i * 2 + 1];
		opa_val_list.append(opa_val);
	}
	opacityFunction->RemoveAllPoints();
	for (int i = 0; i < num2; i++)
	{
		opa_val = opa_val_list.at(i);
		opa_val.val = (opa_val.val - cur_range[0])*new_range / old_range + val[0];
		opacityFunction->AddPoint(opa_val.val, opa_val.opa);
	}
	PipelineObject* tep_obj = NULL;
	if (cur_PipelineObj->GetObjectType() == dNumeric_DataSource)
		tep_obj = cur_PipelineObj;
	else
		tep_obj = cur_PipelineObj->GetObjParent();
	if (tep_obj->scalarBar_widgetMap.contains(cur_PipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle))
	{
		emit sig_update_scalarBar(cur_PipelineObj);
		//vtkSmartPointer<vtkScalarBarWidget> tep_scalarWidget = cur_PipelineObj->scalarBar_widgetMap[cur_PipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle];
		//cur_PipelineObj->scalarBar_widgetMap[cur_PipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle]->Render();
	}
}
void ScalarBarEditForm::slot_update_scalarRange(double val[2])
{
	modify_scalarBarTable(val);
	slot_update_scalarBarTable();
	ui->ScalarColorWidget->setXRange(val[0], val[1]);
	ui->ScalarOpacityWidget->setXRange(val[0], val[1]);
	updateRange();
	this->resize(this->size().width()*0.99, this->size().height());
}

void ScalarBarEditForm::on_enableOpacity_checkBox_clicked(bool checked)
{
	vtkSmartPointer<vtkLookupTable> new_lookupTable=NULL;
	if ((cur_PipelineObj == NULL) || (cur_actor==NULL))
		return;
	QString tep_colName = cur_PipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
	vtkScalarBarWidget* tep_scalarWidget = NULL;
	PipelineObject* tep_obj = NULL;
	if (cur_PipelineObj->GetObjectType() == dNumeric_DataSource)
		tep_obj = cur_PipelineObj;
	else
		tep_obj = cur_PipelineObj->GetObjParent();
	if ((cur_PipelineObj->GetObjectType() == dCalculator_DataSource)
		&& (tep_colName == cur_PipelineObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
	{
		if (tep_obj->scalarBar_widgetMap.contains("CalculatorResult"))
		{
			tep_scalarWidget = tep_obj->scalarBar_widgetMap["CalculatorResult"];
		}
	}
	else
	{
		if (tep_obj->scalarBar_widgetMap.contains(tep_colName))
		{
			tep_scalarWidget = tep_obj->scalarBar_widgetMap[tep_colName];
		}
	}
	
	if (checked)
	{
		new_lookupTable = tep_obj->MakeLUTFromCTF(colorTransferFunction, opacityFunction,256);
		cur_actor->GetMapper()->SetScalarRange(new_lookupTable->GetRange());
		cur_actor->GetMapper()->SetLookupTable(new_lookupTable);
		if (tep_scalarWidget != NULL)
		{
			tep_scalarWidget->GetScalarBarActor()->SetLookupTable(new_lookupTable);
			tep_scalarWidget->GetScalarBarActor()->UseOpacityOn();
		}
	}
	else
	{
		cur_actor->GetMapper()->SetLookupTable(colorTransferFunction);
		if (tep_scalarWidget != NULL)
		{
			tep_scalarWidget->GetScalarBarActor()->SetLookupTable(colorTransferFunction);
			tep_scalarWidget->GetScalarBarActor()->UseOpacityOff();
		}
	}
	if ((cur_PipelineObj->GetObjectType() == dCalculator_DataSource)
		&& (tep_colName == cur_PipelineObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
	{
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.contains("CalculatorResult"))
		{
			tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.insert("CalculatorResult", checked);
		}
	}
	else
	{
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.contains(tep_colName))
		{
			tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.insert(tep_colName, checked);
		}
	}
	emit sig_volume_scalarBarEdit(cur_PipelineObj,new_lookupTable);
}

void ScalarBarEditForm::on_spinBox_numLabels_valueChanged(int arg1)
{
	vtkSmartPointer<vtkLookupTable> new_lookupTable = NULL;
	if ((cur_PipelineObj == NULL) || (cur_actor == NULL))
		return;
	QString tep_colName = cur_PipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
	vtkScalarBarWidget* tep_scalarWidget = NULL;
	PipelineObject* tep_obj=NULL;
	if (cur_PipelineObj->GetObjectType() == dNumeric_DataSource)
		tep_obj = cur_PipelineObj;
	else
		tep_obj = cur_PipelineObj->GetObjParent();

    scalarBarTitle_PropData tep_titlePara;
	if ((cur_PipelineObj->GetObjectType() == dCalculator_DataSource)
		&& (tep_colName == cur_PipelineObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
	{
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains("CalculatorResult"))
		{
			tep_titlePara = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap["CalculatorResult"];
		}
		else
		{
			tep_titlePara.title = ui->colName_lineEdit->text();
			tep_titlePara.size = ui->spinBox->value();
		}
		tep_titlePara.num_lable = arg1;
		tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.insert("CalculatorResult", tep_titlePara);
	}
	else
	{
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains(tep_colName))
		{
			tep_titlePara = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap[tep_colName];
		}
		else
		{
			tep_titlePara.title = ui->colName_lineEdit->text();
			tep_titlePara.size = ui->spinBox->value();
		}
		tep_titlePara.num_lable = arg1;
		tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.insert(tep_colName, tep_titlePara);
	}
    
	if ((cur_PipelineObj->GetObjectType() == dCalculator_DataSource)
		&& (tep_colName == cur_PipelineObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
	{
		if (tep_obj->scalarBar_widgetMap.contains("CalculatorResult"))
		{
			tep_scalarWidget = tep_obj->scalarBar_widgetMap["CalculatorResult"];
			if (tep_scalarWidget != NULL)
			{
				tep_scalarWidget->GetScalarBarActor()->SetNumberOfLabels(arg1);
				tep_scalarWidget->Render();
			}
			emit sig_update_scalarBar_numLables(cur_PipelineObj, tep_colName, arg1);
		}
	}
	else
	{
		if (tep_obj->scalarBar_widgetMap.contains(tep_colName))
		{
			tep_scalarWidget = tep_obj->scalarBar_widgetMap[tep_colName];
			if (tep_scalarWidget != NULL)
			{
				tep_scalarWidget->GetScalarBarActor()->SetNumberOfLabels(arg1);
				tep_scalarWidget->Render();
			}
			emit sig_update_scalarBar_numLables(cur_PipelineObj, tep_colName, arg1);
		}
	}
}

void ScalarBarEditForm::reTranslate()
{
	ui->retranslateUi(this);
}
void ScalarBarEditForm::on_colName_lineEdit_editingFinished()
{
	vtkSmartPointer<vtkLookupTable> new_lookupTable = NULL;
	if ((cur_PipelineObj == NULL) || (cur_actor == NULL))
		return;
	PipelineObject* tep_obj = NULL;
	if (cur_PipelineObj->GetObjectType() == dNumeric_DataSource)
		tep_obj = cur_PipelineObj;
	else
		tep_obj = cur_PipelineObj->GetObjParent();

	QString tep_colName = cur_PipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
    scalarBarTitle_PropData tep_titlePara;
	if ((cur_PipelineObj->GetObjectType() == dCalculator_DataSource)
		&& (tep_colName == cur_PipelineObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
	{
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains("CalculatorResult"))
		{
			tep_titlePara = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap["CalculatorResult"];
		}
		else
		{
			tep_titlePara.size = ui->spinBox->value();
			tep_titlePara.num_lable = ui->spinBox_numLabels->value();
		}
	}
	else
	{
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains(tep_colName))
		{
			tep_titlePara = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap[tep_colName];
		}
		else
		{
			tep_titlePara.size = ui->spinBox->value();
			tep_titlePara.num_lable = ui->spinBox_numLabels->value();
		}
	}
	tep_titlePara.title = ui->colName_lineEdit->text();
	if ((cur_PipelineObj->GetObjectType() == dCalculator_DataSource)
		&& (tep_colName == cur_PipelineObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
	{
		tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.insert("CalculatorResult", tep_titlePara);
	}
	else
	{
		tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.insert(tep_colName, tep_titlePara);
	}
    //QString m_title = ui->colName_lineEdit->text();
	vtkScalarBarWidget* tep_scalarWidget = NULL;
	if ((cur_PipelineObj->GetObjectType() == dCalculator_DataSource)
		&& (tep_colName == cur_PipelineObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
	{
		if (tep_obj->scalarBar_widgetMap.contains("CalculatorResult"))
		{
			tep_scalarWidget = tep_obj->scalarBar_widgetMap["CalculatorResult"];
			if (tep_scalarWidget != NULL)
			{
				tep_scalarWidget->GetScalarBarActor()->SetTitle(tep_titlePara.title.toStdString().c_str());
				tep_scalarWidget->Render();
			}
			emit sig_update_scalarBar_title(cur_PipelineObj, tep_colName, tep_titlePara.title);
		}
	}
	else
	{
		if (tep_obj->scalarBar_widgetMap.contains(tep_colName))
		{
			tep_scalarWidget = tep_obj->scalarBar_widgetMap[tep_colName];
			if (tep_scalarWidget != NULL)
			{
				tep_scalarWidget->GetScalarBarActor()->SetTitle(tep_titlePara.title.toStdString().c_str());
				tep_scalarWidget->Render();
			}
			emit sig_update_scalarBar_title(cur_PipelineObj, tep_colName, tep_titlePara.title);
		}
	}
}

void ScalarBarEditForm::on_spinBox_valueChanged(int arg1)
{
    vtkSmartPointer<vtkLookupTable> new_lookupTable = NULL;
    if ((cur_PipelineObj == NULL) || (cur_actor == NULL))
        return;
    QString tep_colName = cur_PipelineObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
    vtkScalarBarWidget* tep_scalarWidget = NULL;
    PipelineObject* tep_obj=NULL;
    if (cur_PipelineObj->GetObjectType() == dNumeric_DataSource)
        tep_obj = cur_PipelineObj;
    else
        tep_obj = cur_PipelineObj->GetObjParent();
    /*if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains(tep_colName))
    {
        scalarBarTitle_PropData tep_titlePara = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap[tep_colName];
        tep_titlePara.size=arg1;
        tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.insert(tep_titlePara);
    }*/
    scalarBarTitle_PropData tep_titlePara;
	if ((cur_PipelineObj->GetObjectType() == dCalculator_DataSource)
		&& (tep_colName == cur_PipelineObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
	{
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains("CalculatorResult"))
		{
			tep_titlePara = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap["CalculatorResult"];
		}
		else
		{
			tep_titlePara.title = ui->colName_lineEdit->text();
			tep_titlePara.num_lable = ui->spinBox_numLabels->value();
		}
	}
	else
	{
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.contains(tep_colName))
		{
			tep_titlePara = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap[tep_colName];
		}
		else
		{
			tep_titlePara.title = ui->colName_lineEdit->text();
			tep_titlePara.num_lable = ui->spinBox_numLabels->value();
		}
	}
    tep_titlePara.size= arg1;
	if ((cur_PipelineObj->GetObjectType() == dCalculator_DataSource)
		&& (tep_colName == cur_PipelineObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
	{
		tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.insert("CalculatorResult", tep_titlePara);
	}
	else
	{
		tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_titleMap.insert(tep_colName, tep_titlePara);
	}
	if ((cur_PipelineObj->GetObjectType() == dCalculator_DataSource)
		&& (tep_colName == cur_PipelineObj->mPipeLineObjProp.calculatorFilter_propData.resultName))
	{
		if (tep_obj->scalarBar_widgetMap.contains("CalculatorResult"))
		{
			tep_scalarWidget = tep_obj->scalarBar_widgetMap["CalculatorResult"];
			if (tep_scalarWidget != NULL)
			{
				tep_scalarWidget->GetScalarBarActor()->GetTitleTextProperty()->SetFontSize(arg1);
				tep_scalarWidget->Render();
			}
			emit sig_update_scalarBar_fontSize(cur_PipelineObj, tep_colName, arg1);
		}
	}
	else
	{
		if (tep_obj->scalarBar_widgetMap.contains(tep_colName))
		{
			tep_scalarWidget = tep_obj->scalarBar_widgetMap[tep_colName];
			if (tep_scalarWidget != NULL)
			{
				tep_scalarWidget->GetScalarBarActor()->GetTitleTextProperty()->SetFontSize(arg1);
				tep_scalarWidget->Render();
			}
			emit sig_update_scalarBar_fontSize(cur_PipelineObj, tep_colName, arg1);
		}
	}
}
