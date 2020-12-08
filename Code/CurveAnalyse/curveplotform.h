#ifndef CURVEPLOTFORM_H
#define CURVEPLOTFORM_H
#include <QWidget>
#include <QDir>
#include <qvector.h>
#include <qmenu.h>
#include <qfiledialog.h>
#include <qsettings.h>
#include <QTextCodec>
#include "curve_struct.h"
#include "qtimer.h"
#include "qdir.h"
#include "qdatetime.h"
#include "readdatafileprop.h"
#include "curve_saveimagedialog.h"
#include "qcustomplot.h"
#include "curvepropform.h"
#include "aniPointThread.h"
#include "exportCurveThread.h"
#include "exportcurvedialog.h"
namespace Ui {
class CurvePlotForm;
}
extern QMutex g_aniThread_mutex;
extern curve_aniPoint_struct aniPoint_para;
class CurvePlotForm : public QWidget
{
    Q_OBJECT

public:
    explicit CurvePlotForm(QWidget *parent = 0);
    ~CurvePlotForm();
    curve_ini_data *curve_data_ptr;
	void initPlot();
	void func_saveImage(bool flag_dlg, int wid, int hei, QString fileName);
	void func_clear_plot();
	void func_refreshPlotForm();
	virtual void reTranslate();
	void delAniPointCurves();
	void func_zoomAct(QString axis);
public slots:
	void slot_runAni();
	void slot_stopAni();
	void slot_finish_aniThread(bool flag);
	void slot_addAniPoint(int tep_frame,int tep_start_frame,int tep_max_frames);
	void slot_updateAniPoint(int tep_frame, int tep_start_frame, int tep_max_frames);
	//property
	void slot_replot();
	void slot_setAxisName(QString axisId);
	void slot_setAxisRange(QString axisId);//
	void slot_xAxis_dataSource_change(QString axisId);

	void slot_setTitle();
	void slot_setLegend();
	void slot_setGrid();
	void slot_setBackgroundColor();
	void slot_setAxisNum();
	//curve
	void slot_curve_selected_changed(bool selected);
	void slot_axisDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part, QMouseEvent *event);
	void slot_myTitleDoubleClick(QCPTextElement* item, QMouseEvent *event);
	void slot_legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item, QMouseEvent *event);
	void slot_plottableDoubleClick(QCPAbstractPlottable *, int, QMouseEvent *);
	void slot_update_curve_name(int tep_file_index,int tep_curve_index);
	void slot_update_curve_pen(int tep_file_index, int tep_curve_index);
	void slot_update_curve_linePoint(int tep_file_index, int tep_curve_index);
	void slot_update_curve_axis(int tep_file_index, int tep_curve_index);
	void slot_updateCurvePlot(int tep_file_index, int tep_curve_index);
	
	void slot_add_curve_plot(int tep_file_index, int tep_curve_index);
	void slot_updateCurveData(int tep_file_index,int tep_column_index, double val_x, double val_y);//for 3D curve 
	void slot_updateCurveData(QVector<double> data_x, QVector<double> data_y, QString tep_filename);
	void slot_updateCurveData(QVector<double> data_y, QString tep_filename);
	void slot_plot_mouseDoubleClick(QMouseEvent* tep_event);
	void slot_plot_mousePress(QMouseEvent *tep_event);
	void slot_del_curve_plot(int tep_file_index, int tep_column_index);
	void slot_savePlotAct();
	void slot_exportPlotAct();
	void slot_saveImage();
	void slot_update_curve_plot();
	
private:
	Ui::CurvePlotForm *ui;
	bool flag_finish;
	QTextCodec *codec;
	QCustomPlot* pCustomPlot;
	QCPTextElement* title_item;
	QCPCurve* cur_selected_curve;
	curve_saveImage_prop saveImage_prop_data;
	void set_saveImage_prop_data(curve_saveImage_prop tep_prop);
	void func_setTitle();
	void func_setAxisName(QString axisId);
	void func_setGrid();
	void func_setLegend();
	void func_setBackgroundColor();
	void func_setAxisRange(QString axisId);
	void func_setAxisNum();
	int func_numberOfCurves(QString axisId);
	aniPointThread* ani_thread;
	int cur_ani_frame_xh;
	int max_ani_frames;
	QVector<QCPCurve*> aniPointCurveList;
	ExportCurveThread* exportCurveThread;
	
signals:
	void sig_update_tree_curve_name(int,int,QString);
	void sig_finish_aniThread(bool);
	void sig_updateZoomBar(bool);
	void sig_CAE_curve_frame(int tep_frame, int tep_start_frame, int tep_max_frames);
};

#endif // CURVEPLOTFORM_H
