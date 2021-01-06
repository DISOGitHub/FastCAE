#ifndef GRAPH_MAINWINDOW_H
#define GRAPH_MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QBrush>
#include <QSpinBox>
#include <QApplication>
#include "qstring.h"
#include <QLinearGradient>
#include <QTranslator>
#include <QDesktopServices>
#include "qmessagebox.h"
#include "PipelineObjTreeDockWidget.h"
#include "PipelineObjPropDockWidget.h"
#include "consolecmddockwidget.h"
#include "scalarbareditdockwidget.h"
#include "pickcurvedockwidget.h"
#include "PipelineObjPlotForm.h"
#include "global.h"
#include "qcheckbox.h"
#include "qpushbutton.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "graphicsanalyse_global.h"
#include "NumericSrcObject.h"
#include "PipelineObject.h"
#include "ReadDataIO.h"
#include "HeuProjectDB.h"
#include "FilterSlice.h"
#include "FilterClip.h"
#include "FilterContour.h"
#include "FilterVector.h"
#include "FilterReflection.h"
#include "FilterSmooth.h"
#include "FilterStreamLine.h"
#include "FilterCalculator.h"
#include "slicefilterpropform.h"
#include "saveanidialog.h"
#include "qdialog.h"
#include "qtoolbar.h"
#include "qcombobox.h"
#include "saveimagedialog.h"
#include "translator.h"
#include "qprocess.h"
#include "signalhandlerPy.h"
extern QList<PipelineObject*> g_ani_objList;
namespace Ui {
class graph_MainWindow;
}

class SignalHandlerPy; 

class GRAPHICSANALYSESHARED_EXPORT graph_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	Ui::graph_MainWindow* getUi();
	explicit graph_MainWindow(QWidget *parent = 0);
	~graph_MainWindow();
	PipelineObjTreeDockWidget* get_pipelineObj_treeWidget(){ return pipelineObj_treeWidget; };//PipelineObjTreeDockWidget *pipelineObj_treeWidget;
	PipelineObjPropDockWidget* get_pipelineObj_propWidget(){ return pipelineObj_propWidget; };
	PipelineObjPlotForm* get_pipelineObj_plotWidget(){ return pipelineObj_plotWidget; };
	consoleCmdDockWidget* get_console_cmdWidget(){ return console_cmdWidget; };
	pickCurveDockWidget* get_pickCurveWidget(){ return pickCurve_Widget; };
	PipelineObject* get_cur_PipelineObj(){ return cur_PipelineObj; };
	scalarBarEditDockWidget* get_scalarBar_dockWidget() { return scalarBar_dockWidget; };
	Translator* GetTranslator();
	void initTranslate();
	virtual void reTranslate();
	void set_flag_commandLine(bool val) { flag_commandLine = val; };
	//script
	PipelineObject* script_getPipelineObj_byID(int id);
	void script_ChangeLanguage(QString lang);
	bool script_getScalarRange(PipelineObject* tep_obj, double val[2]);
	void script_rescaleScalarRange(PipelineObject* tep_obj, double min, double max);
	//void script_clearAllPipelineObjs();
	void script_updatePipelineObjeDataSet(PipelineObject* tep_pipeObj, QString tep_filename);
	bool script_openFile(QString args);
	void script_showPlotOnly();
	void script_applyClicked();
	vtkDataSet* script_get_pipelineObj_dataSet(PipelineObject* tep_piplnObj);
	void script_FilterClip(PipelineObject* tep_piplnObj);
	void script_FilterSlice(PipelineObject* tep_piplnObj);
	void script_FilterContour(PipelineObject* tep_piplnObj);
	void script_FilterVector(PipelineObject* tep_piplnObj);
	void script_FilterReflection(PipelineObject* tep_piplnObj);
	void script_FilterCalculator(PipelineObject* tep_piplnObj);
	void script_FilterSmooth(PipelineObject* tep_piplnObj);
	void script_FilterStreamLine(PipelineObject* tep_piplnObj);
	//void script_FilterClip(PipelineObject* tep_piplnObj, sliceFilter_pipelineObj_propData tep_plane_propData, bool flag_defaultPlaneProp);
	//void script_FilterSlice(PipelineObject* tep_piplnObj, sliceFilter_pipelineObj_propData tep_plane_propData, bool flag_defaultPlaneProp);
	//void script_FilterContour(PipelineObject* tep_piplnObj, contourFilter_pipelineObj_propData tep_contour_propData, bool flag_defaultContourProp);
	//void script_FilterVector(PipelineObject* tep_piplnObj, vectorFilter_pipelineObj_propData tep_vector_propData, bool flag_defaultVectorProp);

	//properties view
	void script_Properties_view_backgroundType(int val);
	void script_Properties_view_backgroundColor(QColor color_val);
	void script_Properties_view_background2Color(QColor color_val);
	void script_Properties_view_axesVisible(bool val);
	void script_Properties_view_remarkVisible(bool val);
	void script_Properties_view_remarkFontColor(QColor color_val);
	void script_Properties_view_remarkText(QString text);
	void script_Properties_view_remarkPosition1(double *pos);
	void script_Properties_view_remarkPosition2(double *pos);
	void script_Properties_view_cameraParallel(bool val);
	void script_Properties_view_interaction(bool val);
	void script_Properties_renderView();
	//dataSource zone(tecplot cgns)
	void script_Properties_DataSource_DisplayGridOrZone(PipelineObject* tep_piplnObj, int val);
	void script_Properties_dataSourceZone(PipelineObject* tep_piplnObj, QString val);
	vtkFloatArray* script_Properties_getColumnData(PipelineObject* tep_piplnObj, QString col);
	//plane properties
	void script_Properties_planeOrigin(PipelineObject* tep_piplnObj, double val[3]);
	void script_Properties_planeNormal(PipelineObject* tep_piplnObj, double val[3]);
	void script_Properties_planeVisible(PipelineObject* tep_piplnObj, bool val);
	void script_Properties_insideOut(PipelineObject* tep_piplnObj, bool val);
	//plane properties
	//glyph properties
	void script_Properties_vector_GlyphVector(PipelineObject* tep_piplnObj, QString val);
	void script_Properties_vector_scalar(PipelineObject* tep_piplnObj, QString val);
	void script_Properties_vector_normal(PipelineObject* tep_piplnObj, QString val);
	void script_Properties_vector_numPoints(PipelineObject* tep_piplnObj, int val);
	void script_Properties_vector_glyph_type(PipelineObject* tep_piplnObj, int val);
	void script_Properties_vector_glyph_tipRes(PipelineObject* tep_piplnObj, int val);
	void script_Properties_vector_glyph_tipRad(PipelineObject* tep_piplnObj, double val);
	void script_Properties_vector_glyph_tipLen(PipelineObject* tep_piplnObj, double val);
	void script_Properties_vector_glyph_shaftRes(PipelineObject* tep_piplnObj, int val);
	void script_Properties_vector_glyph_shaftRad(PipelineObject* tep_piplnObj, double val);
	//glyph properties
	//streamline
	void script_Properties_streamline_vector(PipelineObject* tep_piplnObj, QString val);
	void script_Properties_streamline_integration_direction(PipelineObject* tep_piplnObj, IntegrationDirection val);
	void script_Properties_streamline_integration_type(PipelineObject* tep_piplnObj, IntegratorType val);
	void script_Properties_streamline_integration_stepUnit(PipelineObject* tep_piplnObj, IntegrationStepUnit val);
	void script_Properties_streamline_integration_initStepLen(PipelineObject* tep_piplnObj, double val);
	void script_Properties_streamline_integration_miniStepLen(PipelineObject* tep_piplnObj, double val);
	void script_Properties_streamline_integration_maxiStepLen(PipelineObject* tep_piplnObj, double val);
	void script_Properties_streamline_stream_maxiSteps(PipelineObject* tep_piplnObj, int val);
	void script_Properties_streamline_stream_maxiStreamLen(PipelineObject* tep_piplnObj, double val);
	void script_Properties_streamline_stream_terminalSpeed(PipelineObject* tep_piplnObj, double val);
	void script_Properties_streamline_stream_maxiError(PipelineObject* tep_piplnObj, double val);
	void script_Properties_streamline_seeds_type(PipelineObject* tep_piplnObj, SpeedType val);
	void script_Properties_streamline_seeds_mPoint(PipelineObject* tep_piplnObj, double val[3]);
	void script_Properties_streamline_seeds_num_points(PipelineObject* tep_piplnObj, int val);
	void script_Properties_streamline_seeds_radius(PipelineObject* tep_piplnObj, double val);
	void script_Properties_streamline_vorticity(PipelineObject* tep_piplnObj, bool val);
	void script_Properties_streamline_interpolatorType(PipelineObject* tep_piplnObj, InterpolatorType val);
	void script_Properties_streamline_surface_streamLines(PipelineObject* tep_piplnObj, bool val);
	void script_Properties_streamline_reflection(PipelineObject* tep_piplnObj, bool val);
	void script_Properties_streamline_reflectionAxes(PipelineObject* tep_piplnObj, ReflectionPlane val);
	//streamline
	//contour properties
	void script_Properties_contourColumn(PipelineObject* tep_piplnObj, QString val);
	void script_Properties_contourValue(PipelineObject* tep_piplnObj, double val);
	void script_Properties_contour_reflection(PipelineObject* tep_piplnObj, bool val);
	void script_Properties_contour_reflectionAxes(PipelineObject* tep_piplnObj, ReflectionPlane val);
	//contour properties
	//reflection properties
	void script_Properties_reflectionAxes(PipelineObject* tep_piplnObj, ReflectionPlane val);
	void script_Properties_reflectionCopyInput(PipelineObject* tep_piplnObj, bool val);
	//reflection public
	//calculator public
	void script_Properties_calculatorFormula(PipelineObject* tep_piplnObj, QString val);
	void script_Properties_calculatorResultName(PipelineObject* tep_piplnObj, QString val);
	//calculator public
	//smooth properties
	void script_Properties_smooth(PipelineObject* tep_piplnObj, int type, double coef);//0cellToPoint,1mesh
	//smooth properties
	//mooringLine properties
	void script_Properties_mooringLine_type(PipelineObject* tep_piplnObj, int val);
	void script_Properties_mooringLine_tipRes(PipelineObject* tep_piplnObj, int val);
	void script_Properties_mooringLine_tipRad(PipelineObject* tep_piplnObj, double val);
	void script_Properties_mooringLine_tipLen(PipelineObject* tep_piplnObj, double val);
	void script_Properties_mooringLine_shaftRes(PipelineObject* tep_piplnObj, int val);
	void script_Properties_mooringLine_shaftRad(PipelineObject* tep_piplnObj, double val);
	void script_Properties_mooringLine_displaySeaBottom(PipelineObject* tep_piplnObj, bool val);
	void script_Properties_mooringLine_colorSeaBottom(PipelineObject* tep_piplnObj, int val[3]);
	//mooringLine properties
	//properties aniDisplace
	void script_Properties_aniDisplace_fileName(PipelineObject* tep_piplnObj, QString val);
	void script_Properties_aniDisplace_startPosition(PipelineObject* tep_piplnObj, int val);
	void script_Properties_aniDisplace_interval(PipelineObject* tep_piplnObj, int val);
	void script_Properties_aniDisplace_zoomFactor(PipelineObject* tep_piplnObj, double val);
	void script_Properties_aniDisplace_gravityCenter(PipelineObject* tep_piplnObj, double val[3]);
	//properties aniDisplace
	void script_Properties_Opacity(PipelineObject* tep_piplnObj, double val);
	void script_Properties_colorColumn(PipelineObject* tep_piplnObj, QString args);
	void script_Properties_colorColumn_component(PipelineObject* tep_piplnObj, QString args);
	void script_Properties_lineWidth(PipelineObject* tep_piplnObj, int val);
	void script_Properties_pointSize(PipelineObject* tep_piplnObj, int val);
	void script_Properties_translate(PipelineObject* tep_piplnObj, double val[3]);
	void script_Properties_origin(PipelineObject* tep_piplnObj, double val[3]);
	void script_Properties_scale(PipelineObject* tep_piplnObj, double val[3]);
	void script_Properties_orientation(PipelineObject* tep_piplnObj, double val[3]);
	void script_Properties_representation(PipelineObject* tep_piplnObj, ERepresentationType val);
	void script_Properties_specular(PipelineObject* tep_piplnObj, double val);
	void script_Properties_diffuse(PipelineObject* tep_piplnObj, double val);
	void script_Properties_ambient(PipelineObject* tep_piplnObj, double val);
	void script_Properties_specularPower(PipelineObject* tep_piplnObj, int val);
	void script_Properties_specularColor(PipelineObject* tep_piplnObj, int val[3]);
	void script_Properties_solidColor(PipelineObject* tep_piplnObj, int val[3]);
	void script_Properties_edgeColor(PipelineObject* tep_piplnObj, int val[3]);
	void script_Properties_interpolation(PipelineObject* tep_piplnObj, EInterpolationType val);
	void script_Properties_Flag_scalarBar(PipelineObject* tep_piplnObj, bool val);
	void script_Properties_LookupTableMap(PipelineObject* tep_piplnObj, QString val);
	void script_Properties_LookupOpacityMap(PipelineObject* tep_piplnObj, QString val);
	void script_Properties_EnableOpacityMap(PipelineObject* tep_piplnObj, bool val);
	void script_Properties_visible(PipelineObject* tep_piplnObj, bool val);
	void script_Properties_show_scalarBars(PipelineObject* tep_piplnObj);
	//slot_update_scalarBar_title(PipelineObject*, QString, QString);
	void script_Properties_show_cubeAxes(PipelineObject* tep_piplnObj);
	void script_Properties_scalarBarTitle(PipelineObject* tep_piplnObj, QString colName, QString m_title);
	void script_Properties_scalarBarFontSize(PipelineObject* tep_piplnObj, QString colName, int m_fontSize);
	void script_Properties_scalarBarNumLables(PipelineObject* tep_piplnObj, QString colName, int m_numLables);
	void script_Properties_scalarBarPosition(PipelineObject* tep_piplnObj, QString colName,int tep_orietation, double pos[4]);
	//proerties public
	//get pipelineobject list
	QList<PipelineObject*> script_get_pipelineObject_list();
	QToolBar* script_get_filterToolBar();//获取过滤器工具栏
	QToolBar* script_get_AniToolBar();//获取动画工具栏
	QToolBar* script_get_viewDirectionToolBar();//获取显示方向设置工具栏
	QToolBar* script_get_actorToolBar();//获取单个三维图形移动、旋转工具栏
	QToolBar* script_get_savePngAviToolBar();//获取保存动画工具栏
	QToolBar* script_get_pipelineObjPropToolBar() { return objPropToolBar; };//获取节点属性工具栏
	QToolBar* script_get_mousePickToolBar() { return mousePickToolBar; };//获取拾取工具栏
	//get pipelineobject list

	Camera_Para script_getCameraPara();
	void script_Camera_Position(double pos[3]);
	void script_Camera_FocalPoint(double focalPoint[3]);
	void script_Camera_ClippingRange(double clippingRange[2]);
	void script_Camera_ViewUp(double viewup[3]);
	void script_Camera_ViewAngle(double angle);
	void script_Camera_ParalellScale(double scale);
	void script_Camera_Zoom(double zoom);
	void script_Camera_Reset();
	void script_Camera_SelfAdaptation(bool flag);
	void RunScript(QString fileName);
	void SetDosScriptFile(QString fileName);

	//set view direction
	void script_resetView();
	void script_setViewDirection(QString val);
	void script_setViewValueDirection(int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3);

	void script_Save_Animate(QString aviFileName);
	void script_saveImage(bool flag_dlg, int wid, int hei, QString fileName);
	void script_delPipelineObj(PipelineObject* pipeObj);
	void script_update_pipelineObj_propForm(PipelineObject* pipeObj);
	void script_projectTreeItemClicked(PipelineObject* tep_pipelineObj);
	bool script_get_pipelineObj_visible_status(PipelineObject* tep_piplnObj);
	int script_get_pipelineObj_ID(PipelineObject* tep_piplnObj);//get pipelineObject ID,-1 not exist

	bool script_start_animate();//启动停止动画
	bool script_stop_animate();
	//pick
	void script_draw_PickPoint(PipelineObject * tep_pipelineObj, vtkIdType pick_Id);
	void script_draw_PickCell(PipelineObject * tep_pipelineObj, vtkIdType pick_Id);
	void script_draw_PickArea(PipelineObject * tep_pipelineObj, int flag_cell, vtkIdList* pick_plist, vtkIdList* pick_clist);
	void script_draw_PickPointCurve(PipelineObject * tep_pipelineObj, vtkIdType pick_Id);
	void script_draw_PickCellCurve(PipelineObject * tep_pipelineObj, vtkIdType pick_Id);
	
	void script_setExitSoftWare(int ms);
	void script_setPlotSize(int* val);
	//script

	QAction *helpAct;
	QAction *aboutAct;
	QAction *VRAct;
	//QAction *newAct;
	QAction *saveAniAct;
	QAction *saveImageAct;
	QAction *openAct;

	//script
	//bool flag_script_running;
	QAction *editScriptAct;
	QAction *runScriptAct{}; // libaojun
	//script

	QAction *contourAct;
	QAction *clipAct;
	QAction *sliceAct;
	QAction *glyphAct;
	QAction *reflectionAct;
	QAction *calculateAct;
	QAction *smoothAct;
	QAction *streamLineAct;
	//QAction *saveAsAct;
	QAction *exitAct;
	QAction *actionChinese;
	QAction *actionEnglish;
	QAction *menuAct;
	QAction *statusAct;
	QAction *toolAct;
	QAction *resetViewAct;
	QAction *XPlusAct;
	QAction *YPlusAct;
	QAction *ZPlusAct;
	QAction *XMinusAct;
	QAction *YMinusAct;
	QAction *ZMinusAct;
	QAction *rubberZoomAct;
	QAction *viewInteractionAct;

	QAction *pickCellAct;
	QAction *pickPointAct;
	QAction *pickAreaCellAct;
	QAction *pickAreaPointAct;
	QAction *pickCurvePointAct;
	QAction *pickCurveCellAct;
	QAction *pickClearAct;

	QAction *cameraRedoAct;
	QAction *cameraUndoAct;
	QAction *actorMoveAct;
	QAction *actorRotateAct;
	QAction *actorRotateXAct;
	QAction *actorRotateYAct;
	QAction *actorRotateZAct;

	QAction *objProp_scalarBarAct;
	QAction *objProp_solidColorAct;
	QComboBox *objProp_columnCmb;
	QComboBox *objProp_presentCmb;
	QComboBox *objProp_GlyphVector;
	QAction *cgnsAniTimesAct;
	QAction* exportAct;
public slots:
	void script_exitSoftWare();
	void script_ReturnToOldSize();
	void script_clearAllPipelineObjs();
	bool slot_LoadDataSource(QString tep_file, QStringList fileList, int tep_file_xh);
	void slot_animate();
	void slot_load_scalarBarEditForm(PipelineObject* tep_pipeObj);
    void slot_setChinese();
    void slot_setEnglish();
	void ChangeLanguage(QString lang);
	void slot_update_scalarBar_title(PipelineObject* tep_pipeObj, QString colName, QString m_title);
	void slot_update_scalarBar_fontSize(PipelineObject* tep_pipeObj, QString colName, int m_fontSize);
	void slot_update_scalarBar_numLables(PipelineObject* tep_pipeObj, QString colName, int m_numLables);
private:
    Ui::graph_MainWindow *ui;
	struct exitSoftWareScript_struct exitSoftWareScript_para;
	QString winID;
	QString dos_scriptFileName;
	SignalHandlerPy* _signalHandler{}; 
	QProcess *VRProcess;
	Translator* _translator;
	PipelineObjTreeDockWidget *pipelineObj_treeWidget;
	PipelineObjPropDockWidget *pipelineObj_propWidget;
	PipelineObjPlotForm *pipelineObj_plotWidget;
	consoleCmdDockWidget *console_cmdWidget;
	pickCurveDockWidget *pickCurve_Widget;
	scalarBarEditDockWidget *scalarBar_dockWidget;
	HeuProjectDB mProjectDB;
	PipelineObject* cur_PipelineObj;
	void addNewPipelineObjectForMulBlock(NumericSrcObject* tep_obj);
	void do_clipFilter(PipelineObject *pipelineobj);
	void do_sliceFilter(PipelineObject *pipelineobj);
	void do_contourFilter(PipelineObject *pipelineobj);
	void do_glyphFilter(PipelineObject *pipelineobj);
	void do_reflectionFilter(PipelineObject *pipelineobj);
	void do_calculatorFilter(PipelineObject *pipelineobj);
	void do_smoothFilter(PipelineObject *pipelineobj);
	void do_streamLineFilter(PipelineObject *pipelineobj);
	void func_saveImage(bool flag_dlg, int wid, int hei, QString fileName);
	saveAni_prop aniSetting_data;
	int count_obj_id;
	void ani_update_all_form(bool flag_start);
	PipelineObjProp* GetPipelineObjProp(PipelineObject* pplnObj);
	void DisplayMessage(const QString& title, const QString& message, EMsgType type, bool onDlg);
    void creatActions();
    void creatDocWindows();
	void AddPipelineObject(PipelineObject* pipeObj);
    void initWindow();
	bool func_aniToolBar(int tep_cur_frame_xh);
	void setDockSize(QDockWidget* dock, int setWidth, int setHeight);
	QLabel* lab1;
	QLabel* lab2;
	QMenu* plotViewMenu;
	QMenu *langueMenu;
	

    QMenu *fileMenu;
    QMenu *filterMenu;
    QMenu *toolMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
	QMenu *VRMenu;

    QMdiArea *mdiArea;
    bool flag_DockWin;
   
    bool flag_newFile;

	QAction *runAct;
	QAction *firstAct;
	QAction *lastAct;
	QAction *nextAct;
	QAction *endAct;
	QLineEdit *cur_frame_lineEdit;
	QSpinBox *cur_frame_spinBox;
	QLabel *cur_frame_num;
	QTime cur_ani_time;
	QSpinBox *ani_fileInterval_spinBox;
	QLabel* combine_model_lab;
	QComboBox* combine_model_obj;
	//QLabel *lab_of;

    QAction *openAviPath_action;
    QAction *applyAvi_action;
    QCheckBox *flag_saveAvi_checkBox;
    QLabel *flag_saveAvi_label;

	QToolBar *fileToolBar;
	QToolBar *filterToolBar;
	QToolBar *viewDirectionToolBar;
	QToolBar *actorToolBar;
	QToolBar *save_pngAviToolBar;
	QToolBar *AniToolBar;
	QToolBar *VRToolBar;
	QToolBar *exitToolBar;
	QToolBar *objPropToolBar;
	QToolBar *mousePickToolBar;
	bool flag_commandLine;
	int script_MainWinWid, script_MainWinHei;
	bool flag_state;
private slots:
    void slot_script_setPlotSize(int* val);
	void slot_update_UndoReDoButton(int);
    void slot_ani_fileInterval_spinBox(int);
	void slot_ani_combine_model_comboBox(int);
    void slot_cgnsAniTimesIcon_status(bool);
	//void slot_update_cgns_pipelineObj(PipelineObject* pipeObj);
	//add for user refresh custom tree
	void slot_script_user_custom_refresh_pipelineTree();
	//add for user refresh custom tree
	//pipeline object properties toolbar
	void slot_objProp_scalarBarAct();
	void slot_objProp_solidColorAct();
	void slot_cgnsAniTimesAct();
	void slot_objProp_exportAct();
	void slot_objProp_presentCmb_currentIndexChanged(int);
	void slot_objProp_columnCmb_currentIndexChanged(int);
	void slot_objProp_GlyphVector_currentIndexChanged(int);
	//pipeline object properties toolbar
	void slot_resetView();
	void slot_viewInteraction(bool checked);
	void slot_cameraUndo();
	void slot_cameraRedo();
	void slot_view_rubberZoom();
	void slot_actorMove();
	void slot_actorRotate();
	void slot_actorRotateX();
	void slot_actorRotateY();
	void slot_actorRotateZ();
	void slot_view_XPlus();
	void slot_view_YPlus();
	void slot_view_ZPlus();
	void slot_view_XMinus();
	void slot_view_YMinus();
	void slot_view_ZMinus();
	void slot_saveImage();
	void slot_runAni_fromAniSetting();
	void slot_changeCurFrame_spinBox(int tep_val);
	void slot_update_countAniFrame(int,int);
	void slot_update_pipelineObjs();
	void slot_animateFirst();
	void slot_animateLast();
	void slot_animateEnd();
	void slot_animateNext();
	void slot_finish_aniThread(bool);
	void slot_update_aniToolBar(int);
	void slot_update_filterToolBar(bool, PipelineObject*);
	void slot_update_pipelineObj_toolBar(PipelineObject* tep_pipeObj);
	void slot_update_toolBar_objProp_column(int tep_index);
	void slot_update_toolBar_objProp_column_GlyphVector(bool flag);
	void slot_update_toolBar_objProp_column_GlyphVectorXYZ(int index);
	void slot_update_toolBar_objProp_present(int tep_index);
	void slot_update_toolBar_objProp_column_Calculator();
	void slot_refreshView_fromPropWidget();
	void slot_delPipelineObj(PipelineObject* pipeObj);
    void slot_showHelpPDF();
	void slot_showToolBar();
	void slot_showMenuBar();
	void slot_showStatusBar();
    void slot_showAboutDialog();
	void slot_showVR();
    void newFile();
    void openFile();
	void editScript();
	void runScript();  //libaojun
	void saveAniSetting();
	//void slot_saveImage();
	PipelineObject* slot_adjust_addFilter();
    void slot_contourFilter();
	void slot_clipFilter();
	void slot_sliceFilter();
	void slot_glyphFilter();
	void slot_reflectionFilter();
	void slot_calculatorFilter();
	void slot_smoothFilter();
	void slot_streamLineFilter();
    void exitSoftware();

	void slot_pickPointAct(bool checked);
	void slot_pickCellAct(bool checked);
	void slot_pickAreaCellAct(bool checked);
	void slot_pickCurvePointAct(bool checked);
	void slot_pickCurveCellAct(bool checked);
	void slot_pickAreaPointAct(bool checked);
	void slot_pickClearAct();

	void slot_pickCellAct_status(bool status);
	void slot_pickPointAct_status(bool status);
	void slot_pickAreaCellAct_status(bool status);
	void slot_pickAreaPointAct_status(bool status);
	void slot_pickCurvePointAct_status(bool status);
	void slot_pickCurveCellAct_status(bool status);

	void slot_script_clearAllPipelineObjs(int);
	
protected:
	void resizeEvent(QResizeEvent * event);
signals:
	void sig_addInfoToConsole(QString);
	void sig_finish_aviFile(QString);
	void sig_refreshView_fromMainWin(QList<PipelineObject*>);
    void sig_update_propForm(int,int);
    void sig_saveToPng();
    void sig_start_animate_plot(bool);
    void sig_stop_animate_plot();
	void sig_runAni(); //void sig_runAni(QList<PipelineObject*>);
	void sig_stopAni();

	void sig_execScript(QString);
	//script
	void sig_script_user_custom_add_pipelineObject(PipelineObject* tep_pipelineObj);
	void sig_script_user_custom_del_pipelineObject(int pipelineObj_id);
	//add for user refresh custom tree
	void sig_script_user_custom_refresh_pipelineTree();
	void sig_script_countAniFrame(int);//发送当前动画刷新到第几帧的信号
	void sig_script_creatPickCurveWidget(QDockWidget*);//发送当前动画拾取曲线窗口
	void sig_script_creatScalarBarEditWidget(QDockWidget*);//发送lookupTable颜色编辑窗口
	//add for user refresh custom tree
	//script

	void sig_script_saveImage();
	void sig_RunScript(QString);
	//python
	void sig_python_openFile(QString);
	void sig_python_FilterSlice(PipelineObject *);
	void sig_python_FilterClip(PipelineObject *);
	void sig_python_FilterContour(PipelineObject *);
	void sig_python_FilterVector(PipelineObject *);
	void sig_python_FilterReflection(PipelineObject *);
	void sig_python_FilterCalculator(PipelineObject *);
	void sig_python_FilterSmooth(PipelineObject *);
	void sig_python_FilterStreamLine(PipelineObject *);

	void sig_python_Camera_Position(double pos[3]);
	void sig_python_Camera_FocalPoint(double focalPoint[3]);
	void sig_python_Camera_ClippingRange(double clippingRange[2]);
	void sig_python_Camera_ViewUp(double viewup[3]);
	void sig_python_Camera_ViewAngle(double angle);
	void sig_python_Camera_ParalellScale(double scale);
	void sig_python_Camera_Zoom(double zoom);
	void sig_python_Camera_Reset();

	void sig_python_scalarBar_title(PipelineObject*, QString, QString);
	void sig_python_scalarBar_fontSize(PipelineObject*, QString, int);
	void sig_python_scalarBar_numLables(PipelineObject*, QString, int);

	void sig_python_resetView();
	void sig_python_view_XPlus();
	void sig_python_view_YPlus();
	void sig_python_view_ZPlus();
	void sig_python_view_XMinus();
	void sig_python_view_YMinus();
	void sig_python_view_ZMinus();

	void sig_python_exitSoftWare();
	void sig_python_setPlotSize(int*);
	//python
};
#endif // MAINWINDOW_H
