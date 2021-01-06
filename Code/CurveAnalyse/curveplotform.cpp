#include "curveplotform.h"
#include "ui_curveplotform.h"
#include "curveaxispropdialog.h"
#include "curvetitlepropdialog.h"
#include "qfile.h"
#include "qtextstream.h"

CurvePlotForm::CurvePlotForm(QWidget *parent) :
    QWidget(parent), ui(new Ui::CurvePlotForm)
{
    ui->setupUi(this);
	ani_thread = NULL;
	exportCurveThread = NULL;
	aniPoint_para.axis_indexList.clear();
	aniPoint_para.aniPoint_CurveNameList.clear();
	aniPoint_para.aniPoint_IdList.clear();
	aniPoint_para.aniPoint_CoorList.clear();
	aniPoint_para.aniPoint_ColorList.clear();
	aniPoint_para.aniPoint_TypeList.clear();
	cur_ani_frame_xh = 0;
	this->setCursor(QCursor(Qt::ArrowCursor));
	setContentsMargins(0, 0, 0, 0);
	curve_data_ptr = NULL;
	title_item = NULL;
	pCustomPlot = new QCustomPlot(this);
	pCustomPlot->xAxis->setRange(1e-12, 1e-10);
	pCustomPlot->xAxis2->setRange(1e-12, 1e-10);
	pCustomPlot->yAxis->setRange(1e-12, 1e-10);
	pCustomPlot->yAxis2->setRange(1e-12, 1e-10);
	pCustomPlot->xAxis2->setRangeReversed(true);
	pCustomPlot->yAxis2->setRangeReversed(true);
	pCustomPlot->xAxis2->grid()->setVisible(false);
	pCustomPlot->yAxis2->grid()->setVisible(false);
	connect(pCustomPlot, SIGNAL(axisDoubleClick(QCPAxis *, QCPAxis::SelectablePart, QMouseEvent *)), this, SLOT(slot_axisDoubleClick(QCPAxis *, QCPAxis::SelectablePart, QMouseEvent *)));
	//connect(pCustomPlot, SIGNAL(legendDoubleClick(QCPLegend *, QCPAbstractLegendItem *, QMouseEvent *)), this, SLOT(slot_legendDoubleClick(QCPLegend *, QCPAbstractLegendItem *, QMouseEvent *)));
	connect(pCustomPlot, SIGNAL(plottableDoubleClick(QCPAbstractPlottable *, int, QMouseEvent *)), this, SLOT(slot_plottableDoubleClick(QCPAbstractPlottable *, int, QMouseEvent *)));
	connect(pCustomPlot, SIGNAL(myPlotDoubleClick(QMouseEvent *)), this, SLOT(slot_plot_mouseDoubleClick(QMouseEvent *)));
	connect(pCustomPlot, SIGNAL(myTitleDoubleClick(QCPTextElement*, QMouseEvent *)), this, SLOT(slot_myTitleDoubleClick(QCPTextElement*, QMouseEvent *)));
	connect(pCustomPlot, SIGNAL(mousePress(QMouseEvent *)), this, SLOT(slot_plot_mousePress(QMouseEvent *)));
	//void itemDoubleClick(QCPAbstractItem *item, QMouseEvent *event);
	pCustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);// | QCP::iSelectPlottables | QCP::iSelectAxes | QCP::iSelectItems);
	QVBoxLayout* layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(pCustomPlot);
	setLayout(layout);
	cur_selected_curve = NULL;
}

CurvePlotForm::~CurvePlotForm()
{
	if (ani_thread != NULL)
	{
		ani_thread->stop();
	}
	delete ani_thread;
	ani_thread = NULL;
	while (flag_finish)
		break;
	delete ui;
}

void CurvePlotForm::slot_myTitleDoubleClick(QCPTextElement* item,QMouseEvent *event)
{
	if ((item == NULL) || (curve_data_ptr == NULL))
		return;
	curveTitlePropDialog* dlg = new curveTitlePropDialog;
	dlg->curve_data_ptr = curve_data_ptr;
	dlg->initForm();
	connect(dlg, SIGNAL(sig_setTitle()), this, SLOT(slot_setTitle()));
	dlg->exec();
}

void CurvePlotForm::slot_axisDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part, QMouseEvent *event)
{
	if  (curve_data_ptr == NULL)
		return;
	if (part == QCPAxis::SelectablePart::spAxisLabel)
	{
		curveAxisPropDialog* dlg = new curveAxisPropDialog;
		dlg->curve_data_ptr = curve_data_ptr;
		if (axis == pCustomPlot->xAxis)
			dlg->setAixsId("X");
		else if (axis == pCustomPlot->xAxis2)
			dlg->setAixsId("X2");
		else if (axis == pCustomPlot->yAxis)
			dlg->setAixsId("Y");
		else if (axis == pCustomPlot->yAxis2)
			dlg->setAixsId("Y2");
		dlg->initForm();
		connect(dlg, SIGNAL(sig_setAxisName(QString)), this, SLOT(slot_setAxisName(QString)));
		connect(dlg, SIGNAL(sig_setAxisRange(QString)), this, SLOT(slot_setAxisRange(QString)));
		connect(dlg, SIGNAL(sig_xAxis_dataSource_change(QString)), this, SLOT(slot_xAxis_dataSource_change(QString)));
		dlg->exec();
	}
}

void CurvePlotForm::slot_legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item, QMouseEvent *event)
{

}

void CurvePlotForm::slot_plottableDoubleClick(QCPAbstractPlottable *t1, int t2, QMouseEvent *t3)
{
	QCPCurve * tep_curve = dynamic_cast<QCPCurve*>(t1);
	if ((tep_curve == NULL)||(curve_data_ptr==NULL))
		return;
	QMap<QString, QCPCurve*>::Iterator it;
	QString tep_curve_id="";
	for (it = curve_data_ptr->curveList.begin(); it != curve_data_ptr->curveList.end(); it++)
	{
		if (it.value() == tep_curve)
		{
			tep_curve_id = it.key();
			break;
		}
	}
	if (tep_curve_id == "")
		return;
	QStringList tep_list = tep_curve_id.split("_");
	if (tep_list.count() != 3)
		return;
	bool ok;
	int tep_file_index = tep_list.at(1).toInt(&ok);
	int tep_column_index = tep_list.at(2).toInt(&ok);
	int tep_curve_index=-1;
	if (tep_file_index >= curve_data_ptr->curve_file_list.count())
		return;
	if (tep_column_index >= curve_data_ptr->curve_file_list.at(tep_file_index).file_prop.num_column)
		return;
	for (int i = 0; i < curve_data_ptr->curve_file_list.at(tep_file_index).line_data.count(); i++)
	{
		if (curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(i).yAxis_column == tep_column_index)
		{
			tep_curve_index = i;
			break;
		}
	}
	if (tep_curve_index == -1)
		return;
	cur_selected_curve = tep_curve;
	CurvePropForm* curve_PropForm = new CurvePropForm;
	curve_PropForm->curve_data_ptr = curve_data_ptr;
	curve_PropForm->initForm(1, tep_file_index,tep_curve_index );
	connect(curve_PropForm, SIGNAL(sig_update_curve_name(int,int)), this, SLOT(slot_update_curve_name(int,int)));
	connect(curve_PropForm, SIGNAL(sig_update_curve_pen(int, int)), this, SLOT(slot_update_curve_pen(int, int)));
	connect(curve_PropForm, SIGNAL(sig_update_curve_linePoint(int, int)), this, SLOT(slot_update_curve_linePoint(int, int)));
	connect(curve_PropForm, SIGNAL(sig_update_curve_axis(int, int)), this, SLOT(slot_update_curve_axis(int, int)));
	curve_PropForm->exec();
}

void CurvePlotForm::slot_plot_mousePress(QMouseEvent *tep_event)
{
	if (tep_event->button() == Qt::RightButton)
	{
		QMenu menu(pCustomPlot);
		QAction *savePlotAct = new QAction(tr("Save Chart"), this);
		menu.addAction(savePlotAct);
		connect(savePlotAct, SIGNAL(triggered()), this, SLOT(slot_savePlotAct()));

		QAction *exportPlotAct = new QAction(tr("Export Chart"), this);
		menu.addAction(exportPlotAct);
		connect(exportPlotAct, SIGNAL(triggered()), this, SLOT(slot_exportPlotAct()));
		menu.exec(QCursor::pos());
	}
}

void CurvePlotForm::slot_update_curve_name(int tep_file_index, int tep_curve_index)
{
	cur_selected_curve->setName(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).yAxis_name);
	pCustomPlot->replot();
	emit sig_update_tree_curve_name(tep_file_index, tep_curve_index, curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).yAxis_name);
}

void CurvePlotForm::slot_update_curve_pen(int tep_file_index, int tep_curve_index)
{
	QPen pen;
	pen.setColor(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).curve_color);
	pen.setWidth(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).curve_lineWidth);
	pen.setStyle(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).curve_lineType);
	cur_selected_curve->setPen(pen);
	pCustomPlot->replot();
}

void CurvePlotForm::slot_update_curve_linePoint(int tep_file_index, int tep_curve_index)
{
	if (curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).flag_showPoint)
		cur_selected_curve->setScatterStyle(QCPScatterStyle::ScatterShape::ssStar);
	else
		cur_selected_curve->setScatterStyle(QCPScatterStyle::ScatterShape::ssNone);
	pCustomPlot->replot();
	
}

void CurvePlotForm::slot_updateCurvePlot(int tep_file_index, int tep_curve_index)
{

}

void CurvePlotForm::slot_update_curve_axis(int tep_file_index, int tep_curve_index)//ÇÐ»»×ø±êÖá
{
	QString tep_curve_id = QString("curve_%1_%2").arg(tep_file_index).arg(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).yAxis_column);
	if (curve_data_ptr->curveList.contains(tep_curve_id))
	{
		QCPCurve* tep_curve = curve_data_ptr->curveList[tep_curve_id];
		tep_curve->data().data()->clear();
		tep_curve->removeFromLegend();
		pCustomPlot->replot();
	}
	slot_add_curve_plot(tep_file_index,tep_curve_index);
}

void CurvePlotForm::initPlot()
{
	pCustomPlot->clearItems();
	pCustomPlot->clearGraphs();
	func_setBackgroundColor();
	func_setTitle();
	func_setAxisName("x");
	func_setAxisName("y");
	func_setAxisName("x2");
	func_setAxisName("y2");
	func_setGrid();
	func_setLegend();
	pCustomPlot->replot();
}

void CurvePlotForm::func_setGrid()
{
	pCustomPlot->xAxis->grid()->setVisible(curve_data_ptr->glo_curve_data.flag_grid);
	pCustomPlot->yAxis->grid()->setVisible(curve_data_ptr->glo_curve_data.flag_grid);
}

void CurvePlotForm::slot_setGrid()
{
	func_setGrid();
	pCustomPlot->replot();
}

void CurvePlotForm::slot_setLegend()
{
	func_setLegend();
	pCustomPlot->replot();
}

void CurvePlotForm::func_setLegend()
{
	if (curve_data_ptr->glo_curve_data.flag_legend == plotProp_legendPostion::none)
		pCustomPlot->legend->setVisible(false);
	else
	{
		pCustomPlot->legend->setVisible(true);
		QFont legendFont = font();  // start out with MainWindow's font..
		legendFont.setPointSize(12); // and make a bit smaller for legend
		pCustomPlot->legend->setFont(legendFont);
		pCustomPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 125)));
		pCustomPlot->legend->setSelectableParts(QCPLegend::spLegendBox);
		if (curve_data_ptr->glo_curve_data.flag_legend == plotProp_legendPostion::right_top)
		{
			pCustomPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
			//connect(pCustomPlot, SIGNAL(plottableDoubleClick(QCPAbstractPlottable*, int, QMouseEvent*)), this, SLOT(editCurve(QCPAbstractPlottable*)));
		}
		else if (curve_data_ptr->glo_curve_data.flag_legend == plotProp_legendPostion::rignt_bottom)
		{
			pCustomPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom | Qt::AlignRight);
		}
		else if (curve_data_ptr->glo_curve_data.flag_legend == plotProp_legendPostion::left_top)
		{
			pCustomPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignLeft);
		}
		else if (curve_data_ptr->glo_curve_data.flag_legend == plotProp_legendPostion::left_bottom)
		{
			pCustomPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom | Qt::AlignLeft);
		}
		//pCustomPlot->axisRect()->setRangeDrag(Qt::Orientation::Horizontal | Qt::Orientation::Vertical);
		//pCustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom /*| QCP::iSelectPlottables*/);
	}
}

void CurvePlotForm::func_setBackgroundColor()
{
	pCustomPlot->setBackground(curve_data_ptr->glo_curve_data.backColor);
}

void CurvePlotForm::slot_setBackgroundColor()
{
	func_setBackgroundColor();
	pCustomPlot->replot();
}

void CurvePlotForm::slot_plot_mouseDoubleClick(QMouseEvent* tep_event)
{
	CurvePropForm* curve_PropForm = new CurvePropForm;
	curve_PropForm->curve_data_ptr = curve_data_ptr;
	curve_PropForm->initForm(0, -1, -1);
	connect(curve_PropForm, SIGNAL(sig_setBackgroundColor()), this, SLOT(slot_setBackgroundColor()));
	connect(curve_PropForm, SIGNAL(sig_setGrid()), this, SLOT(slot_setGrid()));
	connect(curve_PropForm, SIGNAL(sig_setLegend()), this, SLOT(slot_setLegend()));
	connect(curve_PropForm, SIGNAL(sig_setAxisNum()), this, SLOT(slot_setAxisNum()));
	//connect(curve_PropForm, SIGNAL(sig_xAxis_dataSource_change()), this, SLOT(sig_xAxis_dataSource_change()));
	curve_PropForm->exec();
}

void CurvePlotForm::func_setAxisName(QString axisId)
{
	global_plot_axis_prop* axisPropPtr = NULL;
	QCPAxis* tep_axis = NULL;
	if (axisId.toLower() == "x")
	{
		axisPropPtr = &(curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[0]);
		tep_axis = pCustomPlot->xAxis;
	}
	else if (axisId.toLower() == "x2")
	{
		axisPropPtr = &(curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[1]);
		tep_axis = pCustomPlot->xAxis2;
	}
	else if (axisId.toLower() == "y")
	{
		axisPropPtr = &(curve_data_ptr->glo_curve_data.glo_yAxis_prop_data[0]);
		tep_axis = pCustomPlot->yAxis;
	}
	else if (axisId.toLower() == "y2")
	{
		axisPropPtr = &(curve_data_ptr->glo_curve_data.glo_yAxis_prop_data[1]);
		tep_axis = pCustomPlot->yAxis2;
	}
	if (tep_axis != NULL)
	{
		QFont font;
		int fontId;
		if (axisPropPtr->axis_font.font_type == plotProp_fontType::Song)
			fontId = QFontDatabase::addApplicationFont(QCoreApplication::applicationDirPath() + "/font/simfang.ttf");
		else if (axisPropPtr->axis_font.font_type == plotProp_fontType::Hei)
			fontId = QFontDatabase::addApplicationFont(QCoreApplication::applicationDirPath() + "/font/simhei.ttf");
		else
			fontId = QFontDatabase::addApplicationFont(QCoreApplication::applicationDirPath() + "/font/arial.ttf");
		if (fontId >= 0)
		{
			QString font_typeStr = QFontDatabase::applicationFontFamilies(fontId).at(0);
			font.setFamily(font_typeStr);
		}
		font.setBold(true);
		font.setPointSize(axisPropPtr->axis_font.font_size);
		tep_axis->setLabel(axisPropPtr->axis_name);
		tep_axis->setLabelFont(font);
		tep_axis->setLabelColor(axisPropPtr->axis_font.font_color);
		
	}
}

void CurvePlotForm::slot_setAxisName(QString axisId)
{
	func_setAxisName(axisId);
	pCustomPlot->replot();
}

void CurvePlotForm::func_setTitle()
{
	if (curve_data_ptr->glo_curve_data.title == "")
	{
		curve_data_ptr->glo_curve_data.title = " ";
		//return;
	}
	QFont font; int fontId;
	if (curve_data_ptr->glo_curve_data.title_font.font_type == plotProp_fontType::Song)
		fontId = QFontDatabase::addApplicationFont(QCoreApplication::applicationDirPath() + "/font/simfang.ttf");
	else if (curve_data_ptr->glo_curve_data.title_font.font_type == plotProp_fontType::Hei)
		fontId = QFontDatabase::addApplicationFont(QCoreApplication::applicationDirPath() + "/font/simhei.ttf");
	else
		fontId = QFontDatabase::addApplicationFont(QCoreApplication::applicationDirPath() + "/font/arial.ttf");
	if (fontId >= 0)
	{
		QString font_typeStr = QFontDatabase::applicationFontFamilies(fontId).at(0);
		font.setFamily(font_typeStr);
	}
	font.setPointSize(curve_data_ptr->glo_curve_data.title_font.font_size);
	font.setBold(true);
	if (curve_data_ptr->glo_curve_data.title == tr("curve_title"))
	{
		QTextCodec *codec = QTextCodec::codecForName("gbk");
		curve_data_ptr->glo_curve_data.title = codec->toUnicode(QString(tr("curve_title")).toStdString().data());
	}
	if (title_item == NULL)
	{
		title_item = new QCPTextElement(pCustomPlot, curve_data_ptr->glo_curve_data.title, font);
		pCustomPlot->plotLayout()->insertRow(0);
		pCustomPlot->plotLayout()->addElement(0, 0, title_item);
	}
	else
	{
		title_item->setText(curve_data_ptr->glo_curve_data.title);
		title_item->setFont(font);
	}
	title_item->setTextColor(curve_data_ptr->glo_curve_data.title_font.font_color);
}

void CurvePlotForm::slot_setTitle()
{
	func_setTitle();
	pCustomPlot->replot();
}

int CurvePlotForm::func_numberOfCurves(QString axisId)
{
	int num = 0;
	for (int i = 0; i < curve_data_ptr->curve_file_list.count(); i++)
	{
		num+=curve_data_ptr->curve_file_list.at(i).line_data.count();
	}
	return num;
}

void CurvePlotForm::slot_add_curve_plot(int tep_file_index, int tep_curve_index)
{
	QCPCurve* tep_curve = NULL;
	int axis_index = curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).axis_index;
	if ((axis_index != 0) && (axis_index != 1))
		return;
	if (axis_index == 0)
	{
		if (func_numberOfCurves("x") == 1)
		{
			pCustomPlot->xAxis->setRange(1e-12, 1e-10);
			pCustomPlot->yAxis->setRange(1e-12, 1e-10);
		}
	}
	else
	{
		if (func_numberOfCurves("x2") == 1)
		{
			pCustomPlot->xAxis2->setRange(1e-12, 1e-10);
			pCustomPlot->yAxis2->setRange(1e-12, 1e-10);
		}
	}
	if (axis_index == 0)
		tep_curve = new QCPCurve(pCustomPlot->xAxis, pCustomPlot->yAxis);
	else if (axis_index == 1)
		tep_curve = new QCPCurve(pCustomPlot->xAxis2, pCustomPlot->yAxis2);
	QString tep_curve_id = QString("curve_%1_%2").arg(tep_file_index).arg(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).yAxis_column);
	curve_data_ptr->curveList.insert(tep_curve_id, tep_curve);
	//connect(tep_curve, SIGNAL(selectionChanged(bool)), this, SLOT(slot_curve_selected_changed(bool)));
	
	int num_points, tep_column_index;
	QVector<double> data_x, data_y;
	tep_column_index = curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).yAxis_column;
	if (curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[axis_index].axis_dataSource == plotProp_dataSource::Index)
	{
		num_points = curve_data_ptr->curve_file_list.at(tep_file_index).file_prop.num_point;
		
		data_x.resize(num_points);
		data_y.resize(num_points);
		for (int i = 0; i < num_points; i++)
		{
			data_x[i] = i*1.0;
			data_y[i] = curve_data_ptr->curve_file_list.at(tep_file_index).file_prop.column_val_data[tep_column_index][i];
			//data[i] = QCPCurveData(i, i*1.0, curve_data_ptr->curve_file_list.at(tep_file_index).file_prop.column_val_data[tep_column_index][i]);
		}
	}
	else if (curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[axis_index].axis_dataSource == plotProp_dataSource::File)
	{
		int tep_xfile_index= curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[axis_index].axis_fileIndex;
		int tep_xcolumn_index= curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[axis_index].axis_fileColumn;
		num_points = curve_data_ptr->curve_file_list.at(tep_xfile_index).file_prop.num_point;
		if (curve_data_ptr->curve_file_list.at(tep_file_index).file_prop.num_point < num_points)
			num_points = curve_data_ptr->curve_file_list.at(tep_file_index).file_prop.num_point;
		data_x.resize(num_points);
		data_y.resize(num_points);
		for (int i = 0; i < num_points; i++)
		{
			data_x[i] = curve_data_ptr->curve_file_list.at(tep_xfile_index).file_prop.column_val_data[tep_xcolumn_index][i];
			data_y[i] = curve_data_ptr->curve_file_list.at(tep_file_index).file_prop.column_val_data[tep_column_index][i];
		}
	}
	QPen pen;
	pen.setColor(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).curve_color);
	pen.setWidth(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).curve_lineWidth);
	pen.setStyle(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).curve_lineType);
	tep_curve->setPen(pen);
	tep_curve->setName(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).yAxis_name);
	tep_curve->setData(data_x, data_y);
	//tep_curve->rescaleAxes(true);
	if (curve_data_ptr->curveList.count() == 1)
		tep_curve->rescaleAxes();
	else
		tep_curve->rescaleAxes(true);
	if (curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(tep_curve_index).flag_showPoint)
		tep_curve->setScatterStyle(QCPScatterStyle::ScatterShape::ssStar);
	else
		tep_curve->setScatterStyle(QCPScatterStyle::ScatterShape::ssNone);
	tep_curve->setSelectable(QCP::SelectionType::stNone);
	pCustomPlot->replot();
	emit sig_updateZoomBar(true);
}

void CurvePlotForm::slot_updateCurveData(int tep_file_index,int tep_column_index,double val_x,double val_y)
{
	QString tep_curve_id = QString("curve_%1_%2").arg(tep_file_index).arg(tep_column_index);
	QMap<QString, QCPCurve*>::Iterator it = curve_data_ptr->curveList.begin();
	while (it != curve_data_ptr->curveList.end())
	{
		QString tep_key = it.key();
		QCPCurve* tep_curve = it.value();
		if ((tep_curve != NULL) && (tep_key==tep_curve_id))
		{
			tep_curve->addData(val_x, val_y);
			tep_curve->rescaleAxes(true);
			pCustomPlot->replot();
			break;
		}
		it++;
	}
}

void CurvePlotForm::slot_updateCurveData(QVector<double> data_x, QVector<double> data_y, QString tep_filename)
{
	int num_files = curve_data_ptr->curve_file_list.count();
	for (int i = 0; i < num_files; i++)
	{
		if (curve_data_ptr->curve_file_list.at(i).file_prop.Axis_filename == tep_filename)
		{
			QString tep_curve_id = QString("curve_%1_%2").arg(i).arg(1);
			QMap<QString, QCPCurve*>::Iterator it = curve_data_ptr->curveList.begin();
			while (it != curve_data_ptr->curveList.end())
			{
				QString tep_key = it.key();
				QCPCurve* tep_curve = it.value();
				if ((tep_curve != NULL) && (tep_key == tep_curve_id))
				{
					tep_curve->setData(data_x, data_y);
					tep_curve->rescaleAxes(true);
					pCustomPlot->replot();
					break;
				}
				it++;
			}
			break;
		}
	}
}

void CurvePlotForm::slot_updateCurveData(QVector<double> data_y, QString tep_filename)
{
	QVector<double> data_x;
	data_x.clear();
	int num_files = curve_data_ptr->curve_file_list.count();
	for (int i = 0; i < num_files; i++)
	{
		if (curve_data_ptr->curve_file_list.at(i).file_prop.Axis_filename == tep_filename)
		{
			QString tep_curve_id = QString("curve_%1_%2").arg(i).arg(0);
			QMap<QString, QCPCurve*>::Iterator it = curve_data_ptr->curveList.begin();
			while (it != curve_data_ptr->curveList.end())
			{
				QString tep_key = it.key();
				QCPCurve* tep_curve = it.value();
				if ((tep_curve != NULL) && (tep_key == tep_curve_id))
				{
					for (int k = 0; k < data_y.count(); k++)
						data_x.append(k*1.0);
					tep_curve->setData(data_x, data_y);
					tep_curve->rescaleAxes(true);
					pCustomPlot->replot();
					break;
				}
				it++;
			}
			break;
		}
	}
}

void CurvePlotForm::slot_del_curve_plot(int tep_file_index, int tep_column_index)
{
	QString tep_curve_id = QString("curve_%1_%2").arg(tep_file_index).arg(tep_column_index);
	if (curve_data_ptr->curveList.contains(tep_curve_id))
	{
		QCPCurve* tep_curve = curve_data_ptr->curveList[tep_curve_id];
		tep_curve->data().data()->clear();
		tep_curve->removeFromLegend();
		//del aniPoint
		for (int i = 0; i < aniPoint_para.aniPoint_IdList.count(); i++)
		{
			if (tep_curve_id == aniPoint_para.aniPoint_IdList.at(i).trimmed())
			{
				if (aniPointCurveList.count() > i)
				{
					tep_curve->data().data()->clear();
				}
			}
		}
		//del aniPoint
		pCustomPlot->replot();
	}
	curve_file_data tep_curve_file = curve_data_ptr->curve_file_list.at(tep_file_index);
	int tep_num_curve = tep_curve_file.line_data.count();
	for (int i = 0; i<tep_num_curve; i++)
	{
		if (tep_curve_file.line_data.at(i).yAxis_column == tep_column_index)
		{
			tep_curve_file.line_data.removeAt(i);
			break;
		}
	}
	curve_data_ptr->curve_file_list.replace(tep_file_index, tep_curve_file);
	curve_data_ptr->curveList.remove(tep_curve_id);
	if (curve_data_ptr->curveList.count() < 1)
		emit sig_updateZoomBar(false);
}

void CurvePlotForm::slot_curve_selected_changed(bool selected)
{
	QCPCurve * tep_curve = dynamic_cast<QCPCurve*>(sender());
	if (tep_curve == NULL)
		return;
	if (selected)
		tep_curve->setScatterStyle(QCPScatterStyle::ScatterShape::ssStar);
	else
		tep_curve->setScatterStyle(QCPScatterStyle::ScatterShape::ssNone);
}

void CurvePlotForm::func_clear_plot()
{
	QMap<QString, QCPCurve*>::Iterator it = curve_data_ptr->curveList.begin();
	while (it != curve_data_ptr->curveList.end())
	{
		QString tep_key = it.key();
		QCPCurve* tep_curve = it.value();
		if (tep_curve != NULL)
		{
			tep_curve->data().data()->clear();
			tep_curve->removeFromLegend();
		}
		it++;
	}
	pCustomPlot->replot();
	curve_data_ptr->curveList.clear();
}

void CurvePlotForm::set_saveImage_prop_data(curve_saveImage_prop tep_prop)
{
	saveImage_prop_data = tep_prop;
}

void CurvePlotForm::func_saveImage(bool flag_dlg, int wid, int hei, QString fileName)
{
	if (flag_dlg)
	{
		curve_saveImageDialog* dlg = new curve_saveImageDialog;
		if (dlg->exec() == QDialog::Accepted)
		{
			set_saveImage_prop_data(dlg->get_saveImage_prop_data());
			slot_saveImage();
		}
		delete dlg;
		dlg = NULL;
	}
	else
	{
		curve_saveImage_prop tep_data;
		tep_data.image_wid = wid;
		tep_data.image_hei = hei;
		tep_data.saveImage_fileName = fileName;set_saveImage_prop_data(tep_data);
		slot_saveImage();
	}
}

void CurvePlotForm::slot_saveImage()
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QFileInfo tep_info(saveImage_prop_data.saveImage_fileName);
	if (saveImage_prop_data.saveImage_fileName != "")
	{
		QString tep_path = tep_info.absolutePath();
		QDir tep_dir(tep_path);
		if (!tep_dir.exists())
			tep_dir.mkpath(tep_path);
	}
	if(saveImage_prop_data.saveImage_fileName != "")
	{
		if ((saveImage_prop_data.image_hei<0) || (saveImage_prop_data.image_wid<0))
			pCustomPlot->savePng(saveImage_prop_data.saveImage_fileName);
		else
			pCustomPlot->savePng(saveImage_prop_data.saveImage_fileName, saveImage_prop_data.image_wid, saveImage_prop_data.image_hei);
	}
}

void CurvePlotForm::func_setAxisRange(QString axisId)
{
	if (axisId.trimmed().toLower() == "x")
	{
		double step = 0.0;
		if (curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[0].axis_range[0] == curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[0].axis_range[1])
		{
			if (curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[0].axis_range[0] == 0.0)
				step = 0.5;
			else
				step = fabs(curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[0].axis_range[1] / 2.0);
		}		
		pCustomPlot->xAxis->setRange(curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[0].axis_range[0]-step, curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[0].axis_range[1]+step);
	}
	else if (axisId.trimmed().toLower() == "x2")
	{
		double step = 0.0;
		if (curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[1].axis_range[0] == curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[1].axis_range[1])
		{
			if (curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[1].axis_range[0] == 0.0)
				step = 0.5;
			else
				step = fabs(curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[1].axis_range[1] / 2.0);
		}
		pCustomPlot->xAxis2->setRange(curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[1].axis_range[0] - step, curve_data_ptr->glo_curve_data.glo_xAxis_prop_data[1].axis_range[1] + step);
	}
	else if (axisId.trimmed().toLower() == "y")
	{
		double step = 0.0;
		if (curve_data_ptr->glo_curve_data.glo_yAxis_prop_data[0].axis_range[0] == curve_data_ptr->glo_curve_data.glo_yAxis_prop_data[0].axis_range[1])
		{
			if (curve_data_ptr->glo_curve_data.glo_yAxis_prop_data[0].axis_range[0] == 0.0)
				step = 0.5;
			else
				step = fabs(curve_data_ptr->glo_curve_data.glo_yAxis_prop_data[0].axis_range[1] / 2.0);
		}
		pCustomPlot->yAxis->setRange(curve_data_ptr->glo_curve_data.glo_yAxis_prop_data[0].axis_range[0] - step, curve_data_ptr->glo_curve_data.glo_yAxis_prop_data[0].axis_range[1] + step);
	}
	else if (axisId.trimmed().toLower() == "y2")
	{
		double step = 0.0;
		if (curve_data_ptr->glo_curve_data.glo_yAxis_prop_data[1].axis_range[0] == curve_data_ptr->glo_curve_data.glo_yAxis_prop_data[1].axis_range[1])
		{
			if (curve_data_ptr->glo_curve_data.glo_yAxis_prop_data[1].axis_range[0] == 0.0)
				step = 0.5;
			else
				step = fabs(curve_data_ptr->glo_curve_data.glo_yAxis_prop_data[1].axis_range[1] / 2.0);
		}
		pCustomPlot->yAxis2->setRange(curve_data_ptr->glo_curve_data.glo_yAxis_prop_data[1].axis_range[0] - step, curve_data_ptr->glo_curve_data.glo_yAxis_prop_data[1].axis_range[1] + step);
	}
}

void CurvePlotForm::slot_setAxisRange(QString axisId)
{
	func_setAxisRange(axisId);
	pCustomPlot->replot();
}

void CurvePlotForm::slot_savePlotAct()
{
	func_saveImage(true, -1, -1, "");
}

void CurvePlotForm::slot_update_curve_plot()
{
	if (curve_data_ptr == NULL)
		return;
	func_setBackgroundColor();
	func_setTitle();
	func_setAxisName("x");
	func_setAxisName("y");
	func_setGrid();
	func_setLegend();
	for (int i = 0; i < curve_data_ptr->curve_file_list.count(); i++)
	{
		for (int j = 0; j < curve_data_ptr->curve_file_list.at(i).line_data.count(); j++)
			slot_add_curve_plot(i, j);
	}
	if (curve_data_ptr->glo_curve_data.num_axis == 2)
	{
		func_setAxisName("x2");
		func_setAxisName("y2");
	}
	pCustomPlot->replot();
}

void CurvePlotForm::slot_replot()
{
	//func_clear_plot();
	//slot_update_curve_plot();
	if (curve_data_ptr == NULL)
		return;
	func_setBackgroundColor();
	func_setTitle();
	func_setAxisName("x");
	func_setAxisName("y");
	if (curve_data_ptr->glo_curve_data.num_axis == 2)
	{
		func_setAxisName("x2");
		func_setAxisName("y2");
	}
	func_setGrid();
	func_setLegend();
	int tep_file_index, tep_column_index; bool ok;
	QMap<QString, QCPCurve*>::Iterator it = curve_data_ptr->curveList.begin();
	while (it != curve_data_ptr->curveList.end())
	{
		QString tep_key = it.key();
		QCPCurve* tep_curve = it.value();
		if (tep_curve != NULL)
		{
			QStringList tep_list = tep_key.split("_", QString::SkipEmptyParts);
			if (tep_list.count() != 3)
				continue;
			tep_file_index = tep_list.at(1).trimmed().toInt(&ok);
			tep_column_index = tep_list.at(2).trimmed().toInt(&ok);
			if (tep_file_index >= curve_data_ptr->curve_file_list.count())
				continue;
			if (tep_column_index >= curve_data_ptr->curve_file_list.at(tep_file_index).file_prop.num_column)
				continue;
			int num_lines = curve_data_ptr->curve_file_list.at(tep_file_index).line_data.count();
			for (int i = 0; i < num_lines;i++)
			{
				if (curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(i).yAxis_column == tep_column_index)
				{
					QPen pen;
					pen.setColor(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(i).curve_color);
					pen.setWidth(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(i).curve_lineWidth);
					pen.setStyle(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(i).curve_lineType);
					tep_curve->setPen(pen);
					tep_curve->setName(curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(i).yAxis_name);
					tep_curve->rescaleAxes(true);
					if (curve_data_ptr->curve_file_list.at(tep_file_index).line_data.at(i).flag_showPoint)
						tep_curve->setScatterStyle(QCPScatterStyle::ScatterShape::ssStar);
					else
						tep_curve->setScatterStyle(QCPScatterStyle::ScatterShape::ssNone);
					tep_curve->setSelectable(QCP::SelectionType::stNone);
				}
			}
		}
		it++;
	}
	pCustomPlot->replot();
}

void CurvePlotForm::func_refreshPlotForm()
{
	pCustomPlot->replot();
}

void CurvePlotForm::slot_xAxis_dataSource_change(QString axisId)
{
	if ((axisId.toLower() != "x") && (axisId.toLower() != "x2"))
		return;
	func_clear_plot();
	delAniPointCurves();
	slot_update_curve_plot();
}

void CurvePlotForm::func_setAxisNum()
{
	if (curve_data_ptr->glo_curve_data.num_axis == 1)
	{
		pCustomPlot->xAxis2->setVisible(false);
		pCustomPlot->yAxis2->setVisible(false);
		int num_files = curve_data_ptr->curve_file_list.count();
		for (int i = 0; i < num_files; i++)
		{
			int num_lines = curve_data_ptr->curve_file_list.at(i).line_data.count();
			for (int j = 0; j < num_lines; j++)
			{
				if (curve_data_ptr->curve_file_list.at(i).line_data.at(j).axis_index == 1)
				{
					QString tep_curve_id = QString("curve_%1_%2").arg(i).arg(curve_data_ptr->curve_file_list.at(i).line_data.at(j).yAxis_column);
					if (curve_data_ptr->curveList.contains(tep_curve_id))
					{
						curve_data_ptr->curveList[tep_curve_id]->setVisible(false);
						//pCustomPlot->removeGraph(curve_data_ptr->curveList[tep_curve_id]);
						//curve_data_ptr->curveList.remove(tep_curve_id);
					}
				}
			}
		}
	}
	else if (curve_data_ptr->glo_curve_data.num_axis == 2)
	{
		pCustomPlot->xAxis2->setVisible(true);
		pCustomPlot->yAxis2->setVisible(true);
		int num_files = curve_data_ptr->curve_file_list.count();
		for (int i = 0; i < num_files; i++)
		{
			int num_lines = curve_data_ptr->curve_file_list.at(i).line_data.count();
			for (int j = 0; j < num_lines; j++)
			{
				if (curve_data_ptr->curve_file_list.at(i).line_data.at(j).axis_index == 1)
				{
					QString tep_curve_id = QString("curve_%1_%2").arg(i).arg(curve_data_ptr->curve_file_list.at(i).line_data.at(j).yAxis_column);
					if (curve_data_ptr->curveList.contains(tep_curve_id))
					{
						curve_data_ptr->curveList[tep_curve_id]->setVisible(true);
					}
				}
			}
		}
	}
}

void CurvePlotForm::slot_setAxisNum()
{
	func_setAxisNum();
	pCustomPlot->replot();
}

void CurvePlotForm::reTranslate()
{
	ui->retranslateUi(this);
	slot_setTitle();
}
void CurvePlotForm::func_zoomAct(QString axis)
{
	QMap<QString, QCPCurve*>::Iterator it = curve_data_ptr->curveList.begin();
	while (it != curve_data_ptr->curveList.end())
	{
		QString tep_key = it.key();
		QCPCurve* tep_curve = it.value();
		if (tep_curve != NULL)
		{
			if (axis.trimmed().toLower() == "x")
			{
				pCustomPlot->axisRect()->setRangeZoomAxes(pCustomPlot->xAxis, 0);
				pCustomPlot->axisRect()->setRangeZoom(Qt::Horizontal);
			}
			else if (axis.trimmed().toLower() == "y")
			{
				pCustomPlot->axisRect()->setRangeZoomAxes(0, pCustomPlot->yAxis);
				pCustomPlot->axisRect()->setRangeZoom(Qt::Vertical);
			}
			else
			{
				pCustomPlot->axisRect()->setRangeZoomAxes(pCustomPlot->xAxis, pCustomPlot->yAxis);
				pCustomPlot->axisRect()->setRangeZoom(Qt::Vertical | Qt::Horizontal);
			}
		}
		it++;
	}
	pCustomPlot->replot();
}

void CurvePlotForm::slot_runAni()
{
	int num_files = curve_data_ptr->curve_file_list.count();
	max_ani_frames = 0;
	for (int i = 0; i < num_files; i++)
	{
		curve_file_data tep_file_data = curve_data_ptr->curve_file_list.at(i);
		if (max_ani_frames < tep_file_data.file_prop.num_point)
			max_ani_frames = tep_file_data.file_prop.num_point;
	}

	if ((cur_ani_frame_xh + 1) == max_ani_frames)
		cur_ani_frame_xh = 0;

	if (max_ani_frames > 1)
	{
		if (ani_thread == NULL)
		{
			ani_thread = new aniPointThread;
			connect(ani_thread, SIGNAL(sig_finish_aniThread(bool)), this, SLOT(slot_finish_aniThread(bool)));
			connect(ani_thread, SIGNAL(sig_addAniPoint(int,int,int)), this, SLOT(slot_addAniPoint(int,int,int)));
			connect(ani_thread, SIGNAL(sig_updateAniPoint(int,int,int)), this, SLOT(slot_updateAniPoint(int,int,int)));
		}
		else
		{
			if (ani_thread->isRunning())
			{
				ani_thread->stop();
			}
		}
		ani_thread->initThread(curve_data_ptr,max_ani_frames, cur_ani_frame_xh);
		ani_thread->start();

	}
}
void CurvePlotForm::slot_stopAni()
{
	if (ani_thread != NULL)
	{
		if (ani_thread->isRunning())
		{
			ani_thread->stop();
		}
	}
}
void CurvePlotForm::delAniPointCurves()
{
	int num_curves = aniPointCurveList.count();
	for (int i = 0; i < num_curves; i++)
	{
		QCPCurve* tep_curve = aniPointCurveList.at(i);
		if (tep_curve != NULL)
		{
			tep_curve->data().data()->clear();
		}
	}
	pCustomPlot->replot();
	aniPointCurveList.clear();
}
void CurvePlotForm::slot_addAniPoint(int tep_frame, int tep_start_frame, int tep_max_frames)
{
	flag_finish = false;
	delAniPointCurves();
	cur_ani_frame_xh = tep_frame;
	/*QPen pen;
	pen.setColor(curve_data_ptr->glo_curve_data.backColor);
	QMap<QString, QCPGraph*>::Iterator it = curve_data_ptr->curveList.begin();
	while (it != curve_data_ptr->curveList.end())
	{
		QString tep_key = it.key();
		QCPGraph* tep_curve = it.value();
		if (tep_curve != NULL)
		{
			tep_curve->setPen(pen);
		}
		it++;
	}*/
	aniPointCurveList.clear();

	for (int i = 0; i < aniPoint_para.aniPoint_CoorList.count(); i++)
	{
		QVector<double> coor_x,coor_y;
		coor_x << aniPoint_para.aniPoint_CoorList.at(i).x();
		coor_y << aniPoint_para.aniPoint_CoorList.at(i).y();
		QPen drawPen;
		drawPen.setColor(aniPoint_para.aniPoint_ColorList.at(i));
		drawPen.setWidth(2);
		QCPCurve * curGraph;
		if (aniPoint_para.axis_indexList.at(i)==0)
			curGraph = new QCPCurve(pCustomPlot->xAxis, pCustomPlot->yAxis);
		else
			curGraph = new QCPCurve(pCustomPlot->xAxis2, pCustomPlot->yAxis2);
		//QCPCurve * curGraph = pCustomPlot->addGraph();
		curGraph->setPen(drawPen);
		curGraph->setLineStyle(QCPCurve::lsNone);
		curGraph->setScatterStyle(QCPScatterStyle::ScatterShape(aniPoint_para.aniPoint_TypeList.at(i) + 1));
		curGraph->setData(coor_x,coor_y);
		curGraph->setName(aniPoint_para.aniPoint_CurveNameList.at(i).toLocal8Bit().data());
		aniPointCurveList.append(curGraph);
	}
	pCustomPlot->replot();
	g_aniThread_mutex.unlock();
	emit sig_CAE_curve_frame(tep_frame, tep_start_frame, tep_max_frames);
	flag_finish = true;
}
void CurvePlotForm::slot_updateAniPoint(int tep_frame, int tep_start_frame, int tep_max_frames)
{
	flag_finish = false;
	cur_ani_frame_xh = tep_frame;
	for (int i = 0; i < aniPoint_para.aniPoint_CoorList.count(); i++)
	{
		QVector<double> coor_x, coor_y;
		coor_x << aniPoint_para.aniPoint_CoorList.at(i).x();
		coor_y << aniPoint_para.aniPoint_CoorList.at(i).y();
		QCPCurve *tep_graph = aniPointCurveList.at(i);
		tep_graph->setData(coor_x, coor_y);
	}
	pCustomPlot->replot();
	emit sig_CAE_curve_frame(tep_frame, tep_start_frame, tep_max_frames);
	g_aniThread_mutex.unlock();
	flag_finish = true;
}
void CurvePlotForm::slot_finish_aniThread(bool flag)
{
	emit sig_finish_aniThread(flag);
}

void CurvePlotForm::slot_exportPlotAct()
{
	if (curve_data_ptr->curveList.count() < 1)
	{
		QMessageBox box(QMessageBox::Warning, tr("warning"), tr("No curves!"));
		box.setStandardButtons(QMessageBox::Yes);
		box.setButtonText(QMessageBox::Yes, tr("ok"));
		box.exec();
		return;
	}
	QFileDialog* fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(tr("Choose Plot Export Directory"));
	fileDialog->setFileMode(QFileDialog::DirectoryOnly);
	QString dirName;
	if (fileDialog->exec() == QDialog::Accepted)
	{
		dirName = fileDialog->directory().absolutePath();
		if (dirName.isEmpty())
			return;
	}
	else
		return;
	ExportCurveDialog* exportFileDlg = new ExportCurveDialog;
	if (exportCurveThread == NULL)
	{
		exportCurveThread = new ExportCurveThread;
	}
	connect(exportCurveThread, SIGNAL(sig_changeExportText(QString)), exportFileDlg, SLOT(slot_changeExportText(QString)));
	connect(exportFileDlg, SIGNAL(sig_stopFileExport()), exportCurveThread, SLOT(stop()));
	//exportFileDlg->exportThreadPtr = exportCurveThread;
	exportCurveThread->initThread(curve_data_ptr, dirName);
	exportFileDlg->DisplayMovie(tr("file exporting ......"));
	exportCurveThread->start();
	exportFileDlg->exec();
	delete exportFileDlg;
}

