#include "Post2DWidget.h"
#include "ui_Post2DWidget.h"
#include "CurveAnalyse/curve_mainwindow.h"
#include "settings/busAPI.h"

namespace Post
{
	Post2DWindow::Post2DWindow(GUI::MainWindow* mw, int proID)
		:_ui(new Ui::Post2DWidget), ModuleBase::GraphWindowBase(mw, proID, ModuleBase::Post2D)
	{
		_ui->setupUi(this);
		_curveMainWindow = new curve_MainWindow;
		_ui->layout->addWidget(_curveMainWindow->get_curve_plotWidget());
//		_curveMainWindow->get_curve_plotWidget()->show();
	}
	Post2DWindow::~Post2DWindow()
	{
		if (_curveMainWindow) _curveMainWindow->close();
		if (_ui != nullptr) delete _ui;
	}
	void Post2DWindow::replot()
	{
		_curveMainWindow->script_update_replot();
	}
	bool Post2DWindow::openFile(QString file)
	{
		return _curveMainWindow->script_openFile(file);
	}
	void Post2DWindow::addCurve(QString tep_filename, int tep_column_index)
	{
		_curveMainWindow->script_addCurve(tep_filename, tep_column_index);
	}
	void Post2DWindow::addCurve(QVector<double> data_x, QVector<double> data_y, QString tep_filename)
	{
		_curveMainWindow->script_addCurve(data_x, data_y, tep_filename);
	}
	void Post2DWindow::addCurve(QVector<double> data_y, QString fileName)
	{
		_curveMainWindow->script_addCurve(data_y, fileName);
	}
	void Post2DWindow::delCurve(QString fileName, int colum_index)
	{
		_curveMainWindow->script_delCurve(fileName, colum_index);
	}
	void Post2DWindow::delAllCurves()
	{
		_curveMainWindow->script_delAllCurves();
	}
	void Post2DWindow::setTitle(QString title)
	{
		_curveMainWindow->script_Plot_title(title);
	}
	void Post2DWindow::setTitleFont(int font)
	{
		if (font < 0 || font>2) return;
		_curveMainWindow->script_Plot_titleFontType((plotProp_fontType)font);
	}
	void Post2DWindow::setTitleFontSize(int size)
	{
		_curveMainWindow->script_Plot_titleFontSize(size);
	}
	void Post2DWindow::setTitleFontColor(QColor color)
	{
		_curveMainWindow->script_Plot_titleFontColor(color);
	}
	void Post2DWindow::setBackGround(QColor c)
	{
		_curveMainWindow->script_Plot_backgroundColor(c);
	}
	void Post2DWindow::setPlotGrid(bool isgrid)
	{
		_curveMainWindow->script_Plot_grid(isgrid);
	}
	void Post2DWindow::setLegendPosition(int p)
	{
		if (p < 0 || p>4) return;
		_curveMainWindow->script_Plot_legend((plotProp_legendPostion)p);
	}
	void Post2DWindow::setAxisNum(int num)
	{
		_curveMainWindow->script_Plot_numAxis(num);
	}
	void Post2DWindow::setAxisName(QString axis, QString name)
	{
		_curveMainWindow->script_Plot_axisName(axis, name);
	}
	void Post2DWindow::setAxisFontColor(QString axis, QColor color)
	{
		_curveMainWindow->script_Plot_axisFontColor(axis, color);
	}
	void Post2DWindow::setAxisFontSize(QString axis, int size)
	{
		_curveMainWindow->script_Plot_axisFontSize(axis, size);
	}
	void Post2DWindow::setAxisFont(QString axis, int font)
	{
		if (font < 0 || font>2) return;
		_curveMainWindow->script_Plot_axisFontType(axis, (plotProp_fontType)font);
	}
	void Post2DWindow::setCurveColor(QString fileName, int colIndex, QColor color)
	{
		_curveMainWindow->script_Curve_color(fileName, colIndex, color);
	}
	void Post2DWindow::setCurveStyle(QString fileName, int colIndex, Qt::PenStyle style)
	{
		_curveMainWindow->script_Curve_lineType(fileName, colIndex, style);
	}
	void Post2DWindow::setCurveWidth(QString fileName, int colindex, int width)
	{
		_curveMainWindow->script_Curve_lineWidth(fileName, colindex, width);
	}
	void Post2DWindow::setCurveShowPoint(QString fileName, int index, bool show)
	{
		_curveMainWindow->script_Curve_showPoint(fileName, index, show);
	}
	void Post2DWindow::setCurveName(QString fileName, int colindex, QString name)
	{
		_curveMainWindow->script_Curve_name(fileName, colindex, name);
	}
	void Post2DWindow::setCurveAxisIndex(QString fileName, int colindex, int axisIndex)
	{
		_curveMainWindow->script_Curve_axisIndex(fileName, colindex, axisIndex);
	}
// 	QWidget* Post2DWindow::getPropWidget()
// 	{
// 		return _curveMainWindow->get_column_dataWidget();
// 	}
// 	QWidget* Post2DWindow::getTreeWidget()
// 	{
// 		return _curveMainWindow->get_curve_treeWidget();
// 	}
	void Post2DWindow::updateCurve(QVector<double> data_x, QVector<double> data_y, QString tep_filename)
	{
		_curveMainWindow->script_updateCurveData(data_x, data_y, tep_filename);
	}
	void Post2DWindow::updateCurve(QVector<double> data_y, QString tep_filename)
	{
		_curveMainWindow->script_updateCurveData(data_y, tep_filename);
	}
	void Post2DWindow::setBackGround(QColor color1, QColor color2)
	{
		//TODO
	}
	void Post2DWindow::saveImage(QString fileName, int width, int heigh, bool showDlg)
	{
		_curveMainWindow->script_saveImage(showDlg, width, heigh, fileName);
	}
	void Post2DWindow::setAxisRange(QString axis, double range[2])
	{
		_curveMainWindow->script_Plot_axisrange(axis, range);
	}
	QStringList Post2DWindow::getColumnNameList(QString fileName)
	{
		return _curveMainWindow->script_getFileColumnNameList(fileName);
	}
	void Post2DWindow::reTranslate()
	{
		_ui->retranslateUi(this);
		QString lang = Setting::BusAPI::instance()->getLanguage();
		_curveMainWindow->ChangeLanguage(lang);
	}
	QWidget* Post2DWindow::getTreeWidget()
	{
		QDockWidget* dw = _curveMainWindow->get_curve_treeWidget();
		QWidget* old = dw->titleBarWidget();
		QWidget* empty = new QWidget;
		dw->setTitleBarWidget(empty);
		delete old;
		return dw;
	}
	QWidget* Post2DWindow::getPropWidget()
	{
		QDockWidget* dw = _curveMainWindow->get_column_dataWidget();
		QWidget* old = dw->titleBarWidget();
		QWidget* empty = new QWidget;
		dw->setTitleBarWidget(empty);
		delete old;
		return dw;
	}
	curve_line_data Post2DWindow::getCurveProp(QString tepFileName, int colIndex)
	{
		return _curveMainWindow->script_get_CurveProp(tepFileName, colIndex);
	}

	bool Post2DWindow::startAnimate()
	{
		return _curveMainWindow->script_start_animate();
	}

	bool Post2DWindow::stopAnimate()
	{
		return _curveMainWindow->script_stop_animate();
	}

	void Post2DWindow::setAniPointColor(QString fileName, int colum, QColor color)
	{
		_curveMainWindow->script_AniPoint_color(fileName, colum, color);
	}

	void Post2DWindow::setAniPointType(QString fileName, int colum, aniPointType type)
	{
		_curveMainWindow->script_AniPoint_type(fileName, colum, type);
	}



}
