#ifndef PIPELINEOBJPLOTFORM_H
#define PIPELINEOBJPLOTFORM_H
#include <vtkAutoInit.h> 
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
//VTK_MODULE_INIT(vtkRenderingContextOpenGL2);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
VTK_MODULE_INIT(vtkRenderingGL2PSOpenGL2);
//VTK_MODULE_INIT(vtkIOExportOpenGL2);
//vtkRender
#include "qobject.h"
#include <QWidget>
#include "global.h"
#include <QDir>
#include <qvector.h>
#include <qmenu.h>
#include <qfiledialog.h>
#include <qsettings.h>
#include <QTextCodec>
#include "vtkPlotPoints.h"
#include "vtkPlot.h"
#include "vtkSmartPointer.h"
#include "vtkContextView.h"
#include "vtkChartXY.h"
#include "vtkFloatArray.h"
#include "vtkFloatArray.h"
#include "vtkTable.h"
#include "vtkContextView.h"
#include "vtkRenderer.h"
#include "vtkContextScene.h"
#include "vtkAxis.h"
#include "vtkTextProperty.h"
#include "vtkBorderRepresentation.h"
#include "vtkProperty2D.h"
#include "vtkRenderWindow.h"
#include "vtkPen.h"
#include "vtkInteractorStyleSwitch.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleUser.h"
#include "vtkInteractorStyle.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkWindowToImageFilter.h"
#include "vtkPNGWriter.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkInteractorStyleTrackballActor.h"
#include "vtkInteractorStyleUnicam.h"
#include "vtkInteractorStyleFlight.h"
#include "vtkInteractorStyleRubberBandZoom.h"
#include "vtkInteractorStyleJoystickCamera.h"
#include "vtkInteractorStyleJoystickActor.h"
#include "vtkImageWriter.h"
#include "vtkLegendScaleActor.h"
#include "vtkChartLegend.h"
#include "vtkBMPWriter.h"
#include "vtkJPEGWriter.h"
#include "vtkLegendBoxActor.h"
#include "vtkSmartPointer.h"
#include "vtkTextWidget.h"
#include "vtkTextActor.h"
#include "vtkWindowToImageFilter.h"
#include "vtkTextRepresentation.h"
#include "global.h"
#include "qtimer.h"
#include "qdir.h"
#include "qdatetime.h"
#include "vtkGL2PSExporter.h"
#include "vtkRenderLargeImage.h"
#include "vtkgl2ps/gl2ps.h"
#include "vtkSelection.h"
#include "vtkSelectionNode.h"
#include "vtkImageResize.h"
#include "vtkCollection.h"
#include "vtkRendererCollection.h"
#include "vtkCamera.h"
#include "disvtkPOVExporter.h"
#include "PipelineObject.h"
//#include "GraphicsAnalyseLib_global.h"
//#include "savepicturedialog.h"
#include "vtkSmartPointer.h"
#include "vtkCamera.h"
#include "vtkOrientationMarkerWidget.h"
#include "vtkSmartPointer.h"
#include "vtkAxesActor.h"
#include "FilterSlice.h"
#include "FilterClip.h"
#include "FilterContour.h"
#include "FilterVector.h"
#include "FilterReflection.h"
#include "FilterCalculator.h"
#include "FilterSmooth.h"
#include "FilterStreamLine.h"
#include "vtkAniThread.h"
#include "vtkScalarBarRepresentation.h"
#include "vtkAssembly.h"
#include "vtkOBJImporter.h"
#include "vtkOBJReader.h"
#include <vtkCellPicker.h>
#include <vtkPointPicker.h>
#include <vtkCommand.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkObjectFactory.h>
#include <vtkSphereSource.h>
#include <vtkPropPicker.h>
#include <vtkVertex.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkAreaPicker.h>
#include <vtkExtractGeometry.h>
#include <vtkPlanes.h>
#include <vtkAbstractPicker.h>
#include <vtkExtractPolyDataGeometry.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkIdList.h>
#include <vtkGenericCell.h>
#include "PickAreaInteractorStyle.h"
#include "vtkKdTreePointLocator.h"
#include "qtoolbar.h"
#include <QMutex>
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkVolume.h"
#include "updateCgnsThread.h"
#include "rubberZoomInteractorStyle.h"
#include "actorInteractorStyle.h"
#include "actorMoveInteractorStyle.h"
#include "myInteractorStyle.h"
#include "LoadActionFile.h"//20180516
#include "exportfiledialog.h"
#include "exportFileThread.h"
#include <vtkVersionMacros.h>
#if(VTK_MAJOR_VERSION < 9)
#include <QVTKWidget.h>
#else
#include <QVTKOpenGLNativeWidget.h>
#endif
class vtkAVIWriter;

//#include "PipelineActor.h"
extern QList<PipelineObject*> g_ani_objList;
extern QMutex g_aniThread_mutex;
extern bool flag_render;
namespace Ui {
class PipelineObjPlotForm;
}

class GRAPHICSANALYSESHARED_EXPORT PipelineObjPlotForm : public QWidget
{
    Q_OBJECT

public:
	explicit PipelineObjPlotForm(QWidget *parent = 0,QToolBar* toolBar=NULL);
    ~PipelineObjPlotForm();
	bool flag_script_running;
	void setCameraPara_SelfAdaptatioin(bool flag);
	void setScriptRunning(bool flag) { flag_script_running = flag; };
	bool getScriptRunning() { return flag_script_running; };
	void func_saveRemarkPosition();
	MyInteractorStyle* get_mouse_style() { return mouse_style; };
	void set_saveImage_prop_data(saveImage_prop tep_prop);
	void del_obj_scalarBarWidget(PipelineObject *pipeObj);	
	//void set_flag_saveImage(bool val);//bool flag_saveImage;
	void set_saveImage_filename(QString tep_name);
	int max_ani_frames;
	void set_cur_ani_frame_xh(int val);//int cur_ani_frame_xh;
	void set_cur_ani_dis_xh(int val) { cur_ani_dis_xh = val; };
	int get_cur_ani_frame_xh() { return cur_ani_frame_xh; };
	int get_cur_ani_dis_xh() { return cur_ani_dis_xh; };
	saveAni_prop *aniSetting_data;
	void slot_changeCurFrame(int arg1);
	Camera_Para getCameraPara();
	void setCameraPara_Position(double pos[3]);
	void setCameraPara_FocalPoint(double focalPoint[3]);
	void setCameraPara_ClippingRange(double clippingRange[2]);
	void setCameraPara_ViewUp(double viewup[3]);
	void setCameraPara_ViewAngle(double angle);
	void setCameraPara_ParalellScale(double scale);
	void setCameraPara_Zoom(double zoom);
	void setCameraPara_SelfAdaptation(bool flag);
	void setCameraPara_Reset();
	void setViewDirection(QString val);
	void setViewValueDirection(int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3);
	void resetView();
	bool load_objFile(NumericSrcObject* NumericSrcObj,QString obj_fileName);
	void set_pickPointMode();
	void set_pickCellMode();
	void set_pickCurvePointMode();
	void set_pickCurveCellMode();
	void set_pickAreaMode(int flag, bool flag_viewInteraction);//2point 3cell
	void del_pipelineObj_pickPlot(PipelineObject* pipeObj);
	void update_consoleWidget(QString str);
	void set_consoleWidget(consoleCmdDockWidget *tep_wid);
	QWidget *get_vtkWidget();
	void clear_pickPlot();
	void draw_PickPoint(PipelineObject * tep_pipelineObj, vtkIdType pick_Id);
	void draw_PickCell(PipelineObject * tep_pipelineObj, vtkIdType pick_Id);
	void draw_PickArea(PipelineObject * tep_pipelineObj, int flag_cell);
	bool set_pickData_area(PipelineObject * tep_pipelineObj, int flag_pickMode, vtkIdList* tep_plist, vtkIdList* tep_clist);
	bool set_pickData_point_cell(PipelineObject * tep_pipelineObj, int flag_pickMode, vtkIdType pick_id);
	bool set_pickData_curve(PipelineObject * tep_pipelineObj, int flag_pickMode, vtkIdType pick_id);
	void disconnect_pick();
	void clear_pickCurveData();
	bool updatePipelineObjeDataSet(PipelineObject* tep_pipeObj, QString tep_filename);
	void func_set_rubberZoom(bool flag_viewInteraction);
	void func_set_actorOperate(ActorOperateType type,bool flag_viewInteraction);
	void func_viewInteraction(bool checked);
	virtual void reTranslate();
	void func_exportFile(PipelineObject* tep_pipeObj, QString tep_filename);
	void func_set_num_maxDisplace(int num) { num_maxDisplace = num; };
	void func_set_disActors(QList<PipelineObject*> mPipelineObjs);
	renderview_global_prop *m_renderViewPara;
	void generate_scalarBarWidget_calculatorResult(NumericSrcObject *tep_obj, QString calResultName);
public slots:
	void slot_update_glyphVector(PipelineObject *tep_pipeObj);
	void slot_updatePickData(struct pick_pipelineObj_propData tep_pickData);

	void slot_cameraUndo();
	void slot_cameraRedo();
	void slot_cameraDo(int pos);
	void slot_restoreInteractor();
	void slot_update_cgns_pipelineObj(PipelineObject* pipeObj);
	void slot_finish_saveAvi();
	void slot_runAni(); //void slot_runAni(QList<PipelineObject*>);
	void slot_stopAni();
	void slot_stop_update_cgns_pipelineObj();
	void slot_change_global_view_prop();//renderview_global_prop tep_prop
	void slot_init_sliceFilter_plotForm(FilterSlice *pipeObj);
	void slot_init_clipFilter_plotForm(FilterClip *pipeObj);
	void slot_refreshView(QList<PipelineObject*> mPipelineObjs);
	void slot_refreshViewAni(int tep_cur_frame_xh);
	void slot_saveImage();
	void slot_saveAnimate();
	void slot_delPipelineObj(PipelineObject* pipeObj);
	void slot_update_remark(bool flag);
	void slot_update_remark_fontColor();
	void slot_update_remark_text();
	//SliceFilter
	void slot_update_planeOrigin(PipelineObject *);
	void slot_update_planeNormal(PipelineObject *);
	void slot_update_planeVis(PipelineObject *);
	//void slot_update_insideOut(PipelineObject *);
	//SliceFilter

	//modify from private slot to public slot for script
	void slot_update_opacity(PipelineObject * tep_pipelineObj);
	void slot_update_color(PipelineObject * tep_pipelineObj);
	void slot_update_lineWidth(PipelineObject * tep_pipelineObj);
	void slot_update_pointSize(PipelineObject * tep_pipelineObj);
	void slot_update_translate(PipelineObject * tep_pipelineObj);
	void slot_update_origin(PipelineObject * tep_pipelineObj);
	void slot_update_scale(PipelineObject * tep_pipelineObj);
	void slot_update_orientation(PipelineObject * tep_pipelineObj);
	void slot_update_representation(PipelineObject * tep_pipelineObj);
	void slot_update_specular(PipelineObject *);
	void slot_update_diffuse(PipelineObject *);
	void slot_update_ambient(PipelineObject *);
	void slot_update_specularPower(PipelineObject *);
	void slot_update_specularColor(PipelineObject *);
	void slot_update_solidColor(PipelineObject*);
	void slot_update_edgeColor(PipelineObject*);
	void slot_update_interpolation(PipelineObject *);
	void slot_update_scalarBar(PipelineObject *tep_pipelineObj);
	void slot_update_cubeAxes(PipelineObject *tep_pipelineObj);
	void slot_update_actor_visible_plotForm(PipelineObject* pipeObj,bool flag_change);
	//modify from private slot to public slot for script

	void slot_update_plot_pipelineObj(PipelineObject*, vtkColorTransferFunction*, vtkPiecewiseFunction*);
	void slot_volume_scalarBarEdit(PipelineObject* tep_pipeObj, vtkSmartPointer<vtkLookupTable> tep_lookupTable);

	void slot_update_camera_parallel(bool checked);
private slots:
	void slot_ImageAndAnimate();
	void slot_update_UndoReDoButton(int);
	void slot_update_cgns_plot(PipelineObject*, int);
	void slot_offPlaneWidget(PipelineObject *pipeObj);
	void slot_finish_cgnsThread(PipelineObject*);
	//SliceFilter
	/*void slot_update_planeOrigin(PipelineObject *);
	void slot_update_planeNormal(PipelineObject *);
	void slot_update_planeVis(PipelineObject *);*/
	void slot_update_planeCameraNormal(PipelineObject *);
	//SliceFilter

	//void slot_change_global_view_prop(renderview_global_prop tep_prop);	
	void slot_update_axes_visible(bool checked);

	void slot_refreshView_fromMainWin(QList<PipelineObject*>);


	void slot_cellPicker();
	void slot_pointPicker();
	void slot_curvePointPicker();
	void slot_curveCellPicker();
	void slot_refreshPicker();

	void slot_finish_aniThread(bool flag);
	void slot_movePosition(double* pos);
private:
	pov_prop pov_propData;
	bool flag_selfAdaptation;
	int num_maxDisplace;
	QString mFontPath;
	ExportFileThread* exportThread;
	//LoadActionFile loadDispFile;//20180516
	void func_combine_obj_model(PipelineObject* tep_pipelineObj);
	bool flag_cur_viewInteraction;
	rubberZoomInteractorStyle* rubberZoom_style;
	actorInteractorStyle* actorROperate_style;
	actorMoveInteractorStyle* actorMOperate_style;
	PickAreaInteractorStyle* areaPick_style;
	//use for pick
	struct pick_pipelineObj_propData pick_Data;
	QList<PipelineObject*> cur_mPipelineObjs;
	vtkSmartPointer<vtkEventQtSlotConnect> vtk_mousePicker_connector;
	//use for pick
	saveImage_prop saveImage_prop_data;
	void init_saveImage_prop_data();
	void savePicutreForAvi(QString img_name);
	void savePovForAvi(QString pov_name);
	QString savePng_filename;
	void generate_scalarBarWidget(NumericSrcObject *tep_obj);
	void generate_scalarBarWidget_GlyphVector(NumericSrcObject *tep_obj);
	PipelineObject* findParentPipelineObj(QString tep_filename, QList<PipelineObject*> tep_mPipelineObjs);
	void func_resetDisAni(QList<PipelineObject*> tep_mPipelineObjs);
	bool func_setPipeObjDisplaceData(PipelineObject *tep_pipelineObj, int tep_cur_frame_xh);
	
	double scalarBar_max[2];
	double scalarBar_min[2];
	QMutex plot_mutex;
	Ui::PipelineObjPlotForm *ui;

	vtkRenderWindow* renderWindow{};
	vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
	MyInteractorStyle* mouse_style; //vtkInteractorStyleTrackballCamera* mouse_style;
	void AddActors(PipelineObject* pipeObject);
	vtkSmartPointer<vtkCamera> cur_camera;
	vtkSmartPointer<vtkOrientationMarkerWidget> m_orientationMarkerWidget;
	vtkSmartPointer<vtkAxesActor>  m_orientationAxesActor;
	vtkSmartPointer<vtkTextWidget> m_remarkWidget;
	vtkSmartPointer<vtkTextActor>  m_remarkActor;
	vtkSmartPointer<vtkTextRepresentation> m_remarkPresentation;
	int count_scalarBar;
	QVector<vtkSmartPointer<vtkScalarBarWidget>> scalarBar_list;
	vtkAniThread *ani_thread;
	updateCgnsThread *cgns_thread;
	vtkAVIWriter* avi_writer;
	bool flag_start_aniSave;
	bool flag_refresh_forAni;
	//bool flag_saveImage;
	int cur_ani_frame_xh;
	int cur_ani_dis_xh;
	consoleCmdDockWidget *consoleWidget_ptr;
	bool func_cellPicker();
	bool func_pointPicker();
	QToolBar* aniToolBarPtr;

	template<class TPipelineObject> void func_addActors(PipelineObject* pipeObject)
	{
		TPipelineObject *tep_pipelineObj = (TPipelineObject*)pipeObject;
		if (!pipeObject->flag_exist_actors)
		{
			tep_pipelineObj->generate_actors();
			if ((pipeObject->model_ptr != NULL) && (tep_pipelineObj->GetObjectType() == dNumeric_DataSource))
			{
				renderer->AddViewProp(tep_pipelineObj->model_ptr);
			}
			else
			{
				for (int i = 0; i < tep_pipelineObj->mVecActors.count(); i++)
				{
					renderer->AddViewProp(tep_pipelineObj->mVecActors[i]);
				}
			}
		}
		else
		{
			tep_pipelineObj->generate_actors();
		}
	}
	bool func_displaceAniFinish();
	void func_update_scalarBarPosition(QList<PipelineObject*> mPipelineObjs);
signals:
	void sig_update_UndoReDoButton(int);

	void sig_cgnsAniTimesIcon_status(bool);
	void sig_cgns_animnate_times(QString);

	void sig_finish_aviFile(QString);
	void sig_finish_aniThread(bool);
	void sig_addInfoToConsole(QString infoStr);
	void sig_treeItem_disable(PipelineObject*);
	void sig_update_runtool_runing(bool);
	void sig_update_countAniFrame(int,int);
	//script
	void sig_script_user_custom_refresh_pipelineTree();
	void sig_script_refresh_pickData(struct pick_pipelineObj_propData pick_Data);
	//script

	//draw curve
	void sig_draw_3d_curve(QString,QVector<double>);
	void sig_update_3d_curve(int,int,double,double );
	void sig_pickCellAct_status(bool);
	void sig_pickPointAct_status(bool);
	void sig_pickAreaCellAct_status(bool);
	void sig_pickAreaPointAct_status(bool);
	void sig_pickCurvePointAct_status(bool);
	void sig_pickCurveCellAct_status(bool);

	void sig_script_update_currentObj(PipelineObject*);

	void sig_saveImage();
	void sig_saveAnimate();
	void sig_exitSoftWare();

	//void sig_updateCalculatorScalars(PipelineObject*);
};

#endif // PIPELINEOBJPLOTFORM_H
