#ifndef CURVEMAINWINDOW_H
#define CURVEMAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QBrush>
#include <QApplication>
#include "qstring.h"
#include <QLinearGradient>
#include <QTranslator>
#include <QDesktopServices>
#include "qmessagebox.h"
#include "curvetreedockwidget.h"
#include "columnDataDockWidget.h"
#include "curveplotform.h"
#include "curve_struct.h"
#include "qcheckbox.h"
#include "qpushbutton.h"
#include "qlabel.h"
#include "curve_aboutdialog.h"
#include "curveanalyse_global.h"
#include "translator.h"

namespace Ui {
class curve_MainWindow;
}

class CURVEANALYSE_EXPORT curve_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	explicit curve_MainWindow(QWidget *parent = 0);
	~curve_MainWindow();
	CurveTreeDockWidget *get_curve_treeWidget() { return curve_treeWidget; };
	ColumnDataDockWidget *get_column_dataWidget() { return column_dataWidget; };
	CurvePlotForm *get_curve_plotWidget() { return curve_plotWidget; };

	//script function
	void RunScript(QString fileName);
	bool script_openFile(QString args);
	QStringList script_getFileColumnNameList(QString tep_filename);
	void script_addCurve(QString tep_filename,int tep_column_index);
	void script_updateCurveData(int tep_file_index, int tep_column_index, double val_x, double val_y);
	void script_addCurve(QVector<double> data_y, QString tep_filename);//用于三维拾取
	void script_addCurve(QVector<double> data_x, QVector<double> data_y,QString tep_filename);//不调用文件，直接用2组浮点数画曲线
	void script_updateCurveData(QVector<double> data_x, QVector<double> data_y, QString tep_filename);
	void script_updateCurveData(QVector<double> data_y, QString tep_filename);
	void script_refreshCurveForm();//用于设置曲线属性后使属性生效
	void script_update_replot();//用于删除所有曲线进行重绘
	//void script_update_curve_data(QString tep_curve_id, double val_x, double val_y);
	void script_delCurve(QString tep_filename, int tep_column_index);
	void script_delAllCurves();
	void script_Plot_title(QString name);
	void script_Plot_titleFontColor(QColor color);
	void script_Plot_titleFontSize(int fontSize);
	void script_Plot_titleFontType(plotProp_fontType fontType);
	void script_Plot_backgroundColor(QColor color);
	void script_Plot_grid(bool flag_grid);
	void script_Plot_legend(plotProp_legendPostion flag_legend);
	void script_Plot_numAxis(int num_axis);
	void script_Plot_axisName(QString axis, QString name);
	void script_Plot_axisFontColor(QString axis, QColor color);
	void script_Plot_axisFontSize(QString axis, int fontSize);
	void script_Plot_axisFontType(QString axis, plotProp_fontType fontType);
	void script_Plot_axisDataSource(QString axis, plotProp_dataSource dataSource);
	void script_Plot_axisFileName(QString axis, QString tep_filename);
	void script_Plot_axisFileColumn(QString axis, int tep_column_index);
	void script_Plot_axisrange(QString axis, double range[2]);
	curve_line_data script_get_CurveProp(QString tep_fileName, int tep_column_index);
	void script_Curve_color(QString tep_fileName,int tep_column_index,QColor color);
	void script_Curve_lineType(QString tep_fileName, int tep_column_index, Qt::PenStyle lineType);
	void script_Curve_lineWidth(QString tep_fileName, int tep_column_index, int lineWidth);
	void script_Curve_showPoint(QString tep_fileName, int tep_column_index, bool flag_point);
	void script_Curve_name(QString tep_fileName, int tep_column_index, QString name);
	void script_Curve_axisIndex(QString tep_fileName, int tep_column_index, int index);
	//void script_UpdateCurvePlot(QString tep_fileName, int tep_column_index);
	void script_AniPoint_color(QString tep_fileName, int tep_column_index, QColor color);/////////////////////////////
	void script_AniPoint_type(QString tep_fileName, int tep_column_index, aniPointType type);////////////////////////
	void script_AniPoint_del();////////////////////////
	QToolBar *script_getAniToolBar() { return aniPointBar; };

	void script_saveImage(bool flag_dlg, int wid, int hei, QString fileName);
	
	QToolBar* script_get_fileToolBar();
	QToolBar* script_get_helpToolBar();
	bool script_start_animate();///////////////////////////
	bool script_stop_animate();///////////////////////////////
	
	Translator* GetTranslator();
	void initTranslate();
	virtual void reTranslate();
public slots:
    void slot_setChinese();
    void slot_setEnglish();
	void ChangeLanguage(QString lang);
	
private:
	void ani_update_all_form(bool flag_start);
	curve_file_data func_create_file_fromXY(QVector<double> data_x, QVector<double> data_y, QString tep_filename);
	curve_file_data func_create_file_fromY(QVector<double> data_y, QString tep_filename);
    Ui::curve_MainWindow *ui;
	Translator* _translator;
	CurveTreeDockWidget *curve_treeWidget;
	ColumnDataDockWidget *column_dataWidget;
	CurvePlotForm *curve_plotWidget;
    void creatActions();
    void creatDocWindows();
	void initPlotData();
    void initWindow();

	//Script
	bool flag_script_running;
	QAction *editScriptAct;
	QAction *runScriptAct;
	void editScript(QString tep_filename);
	void editScript_GlobalProp(QString fileName);
	void editScript_Curve(QString fileName);
	void editScript_start(QString fileName);
	void editScript_end(QString fileName);

    QAction *openAct;
    QAction *savePngAct;
    QAction *exitAct;
	QAction *actionEnglish;
	QAction *actionChinese;
    QAction *menuAct;
    QAction *statusAct;
    QAction *toolAct;
	QAction *helpAct;
	QAction *aboutAct;

	QAction *zoomXAct;
	QAction *zoomYAct;

    QMenu *fileMenu;
    QMenu *toolMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
	QMenu *langueMenu;
	QMenu *zoomMenu;

    QMdiArea *mdiArea;
    bool flag_DockWin;
	curve_ini_data curve_data;

	QToolBar *fileToolBar;
	QToolBar *helpToolBar;
	QToolBar *exitToolBar;
	QToolBar *aniPointBar;
	QToolBar *zoomBar;
	QAction *aniPointAct;
	QAction *delAniPointAct;
private slots:
    void slot_showHelpPDF();
	void slot_showToolBar();
	void slot_showMenuBar();
	void slot_showStatusBar();
    void slot_showAboutDialog();
    void slot_newFile();
    void openFile();
    void slot_savePngFile();
    void slot_exitSoftware();
	void slot_editScript();
	void slot_runScript();
	void slot_animate();
	void slot_delAniPointAct();
	void slot_finish_aniThread(bool);
	void slot_zoomXAct(bool);
	void slot_zoomYAct(bool);
	void slot_updateZoomBar(bool);
signals:
    void sig_update_curveTree();
	void sig_CAE_curve_animate(int);//1开始0停止
};
#endif // CURVEMAINWINDOW_H
