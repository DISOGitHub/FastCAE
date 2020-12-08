#include "pickcurvedockwidget.h"

pickCurveDockWidget::pickCurveDockWidget(QWidget *parent) : QDockWidget(parent)
{
	setStyleSheet("QWidget{background-color: #eaf2f8;}""QDockWidget::title{border-radius: 6px; background-image: url(:/images/project_title.png);}");
    this->setWindowTitle(tr("pickCurve"));
	curve_widget = new curve_MainWindow;
	setWidget(curve_widget->get_curve_plotWidget());
	curve_name = "";
}

pickCurveDockWidget::~pickCurveDockWidget()
{
	if (curve_widget != NULL)
	{
		delete curve_widget;
		curve_widget = NULL;
	}
}

void pickCurveDockWidget::slot_draw_3d_curve(QString tep_curve_name,QVector<double> data)
{
	if (data.count() < 1)
		return;
	if (curve_name != "")
		slot_del_3d_curve();
	QString fileName = tep_curve_name;
	//curve_widget->script_Plot_backgroundColor(QColor(0,192,255));
	curve_widget->script_addCurve(data, fileName);
	curve_widget->script_Curve_color(fileName, 0, QColor(255, 0, 0));
	//curve_widget->script_Curve_axisIndex(fileName, 0, 0);
	curve_widget->script_Curve_name(fileName, 0, tep_curve_name);
	curve_widget->script_Plot_title(tep_curve_name);
	curve_widget->script_Plot_legend(plotProp_legendPostion::none);
	curve_widget->script_Plot_numAxis(1);
	curve_widget->script_Plot_axisName("x", "x");
	curve_widget->script_Plot_axisName("y", "y");
	curve_widget->script_Curve_lineType(fileName, 0, Qt::PenStyle::SolidLine);
	curve_widget->script_Curve_lineWidth(fileName, 0, 2);
	curve_widget->script_Curve_showPoint(fileName, 0, true);

	curve_widget->script_update_replot();
	setDiaplayCurveName(tep_curve_name);
}

void pickCurveDockWidget::slot_update_3d_curve(int tep_file_index,int tep_column_index, double val_x, double val_y)
{
	curve_widget->script_updateCurveData(0,0, val_x, val_y);
}

void pickCurveDockWidget::slot_del_3d_curve()
{
	curve_widget->script_delAllCurves(); //curve_widget->script_delCurve(curve_name, 0);
}

void pickCurveDockWidget::setDiaplayCurveName(QString tep_name)
{
	curve_name = tep_name;
}

void pickCurveDockWidget::reTranslate()
{
	this->setWindowTitle(tr("pickCurve"));
	curve_widget->reTranslate();
}