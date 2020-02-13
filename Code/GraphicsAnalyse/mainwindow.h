#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include "slicefilterpropform.h"
#include "saveanidialog.h"
extern QList<PipelineObject*> glo_ani_objList;

namespace Ui {
class MainWindow;
}

class GRAPHICSANALYSESHARED_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent=0);
    ~MainWindow();
    PipelineObjTreeDockWidget *pipelineObj_treeWidget;
    PipelineObjPropDockWidget *pipelineObj_propWidget;
    PipelineObjPlotForm *pipelineObj_plotWidget;
    consoleCmdDockWidget *console_cmdWidget;
    QTranslator *pre_translator;
	QString cur_ini_filename;
	void DisplayMessage(const QString& title, const QString& message, EMsgType type, bool onDlg);
public slots:
    void slot_setChinese();
    void slot_setEnglish();
private:
	saveAni_prop aniSetting_data;
	int count_obj_id;
	void ani_update_all_form(bool flag_start);
	PipelineObjProp* GetPipelineObjProp(PipelineObject* pplnObj);
    Ui::MainWindow *ui;
	PipelineObject* cur_PipelineObj;
	HeuProjectDB mProjectDB;
	
    void creatActions();
    void creatDocWindows();
	void AddPipelineObject(PipelineObject* pipeObj);
    void initWindow();
	//QAction *newAct;
	QAction *saveAniAct;
    QAction *openAct;
	QAction *runScriptAct{}; // libaojun
    QAction *contourAct;
    QAction *clipAct;
    QAction *sliceAct;
    QAction *glyphAct;
    //QAction *saveAsAct;
    QAction *exitAct;
    QAction *addCurveAct;
    QAction *delCurveAct;
    QAction *language_cn;
    QAction *language_en;
    QAction *menuAct;
    QAction *statusAct;
    QAction *toolAct;

    QMenu *fileMenu;
    QMenu *filterMenu;
    QMenu *toolMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

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
	//QLabel *lab_of;

    QAction *openAviPath_action;
    QAction *applyAvi_action;
    QCheckBox *flag_saveAvi_checkBox;
    QLabel *flag_saveAvi_label;

	QToolBar *fileToolBar;
	QToolBar *filterToolBar;
	QToolBar *AniToolBar;
	QToolBar *exitToolBar;
	
private slots:
	void slot_runAni_fromAniSetting(saveAni_prop);
	void slot_changeCurFrame_spinBox(int tep_val);
	//void slot_changeCurFrame_lineEdit(QString str_frameNo);
	//void slot_changeCurFrame_lineEdit(int tep_val);
	void slot_update_countAniFrame(int,int);
	//void slot_build_planePlot_connect(PipelineObject*);
	void slot_update_pipelineObjs();//void slot_update_pipelineObjs(QList<PipelineObject*> tep_pipelineObjs);
//	void slot_animate();
	void slot_animateFirst();
	void slot_animateLast();
	void slot_animateEnd();
	void slot_animateNext();
	void slot_update_runtool_runing(bool);
	void slot_update_aniToolBar(int);
	void slot_update_filterToolBar(bool, PipelineObject*);
	void slot_refreshView_fromPropWidget();
	void slot_delPipelineObj(PipelineObject* pipeObj);
    void slot_showHelpPDF();
	void slot_showToolBar();
	void slot_showMenuBar();
	void slot_showStatusBar();
    void slot_showAboutDialog();
    void newFile();
    void openFile();
	void runScript();  //libaojun
	void saveAniSetting();
    void slot_contourFilter();
	void slot_clipFilter();
	void slot_sliceFilter();
	void slot_glyphFilter();
    void exitSoftware();
public slots:  //libaojun for fake script
	void slot_LoadDataSource(QString tep_file,QStringList fileList,int tep_file_xh);
	void slot_animate();
public:
	void applyClicked(){ pipelineObj_propWidget->applyClicked(); }
	void RunScript(QString filename);

signals:
	void sig_refreshView_fromMainWin(QList<PipelineObject*>);
	//void sig_update_curve_propForm(curve_line_data);
    void sig_update_curveTree();
	void sig_update_tree_curve_info(int);
    void sig_update_propForm(int,int);
    void sig_saveToPng();
    void sig_start_animate_plot(bool);
    void sig_stop_animate_plot();
	void sig_runAni(); //void sig_runAni(QList<PipelineObject*>);
	void sig_stopAni();
};
#endif // MAINWINDOW_H
