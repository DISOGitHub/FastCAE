#ifndef PIPELINEOBJPROPWIDGET_H
#define PIPELINEOBJPROPWIDGET_H

#include <QWidget>
#include "global.h"
#include "PipelineObject.h"
#include "PipelineObjProp.h"
#include "qboxlayout.h"
#include "scalarbareditdockwidget.h"
#include "datasourcepropform.h"
//#include "sizeDockWidget.h"
namespace Ui {
class PipelineObjPropWidget;
}
extern bool flag_render;
extern QString openDisDir;
class PipelineObjPropWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PipelineObjPropWidget(QWidget *parent = 0);
    ~PipelineObjPropWidget();
	Ui::PipelineObjPropWidget* getUi();
	QWidget *get_cur_addWidget(){ return cur_addWidget; };
	renderview_global_prop get_renderView_prop();
	void set_renderView_prop_backgroundType(int val);
	void set_renderView_prop_backgroundColor(QColor color);
	void set_renderView_prop_background2Color(QColor color2);
	void set_renderView_prop_axesVisible(bool val);
	void set_renderView_prop_remarkVisible(bool val);
	void set_renderView_prop_remarkFontColor(QColor color);
	void set_renderView_prop_remarkText(QString text);
	void set_renderView_prop_remarkPosition1(double* pos);
	void set_renderView_prop_remarkPosition2(double* pos);
	void set_renderView_prop_cameraParallel(bool val);
	void script_update_global_view_prop();//renderview_global_prop tep_prop
	void set_scalarBar_Form(scalarBarEditDockWidget* tep_form) { tep_scalarBar_Form = tep_form; };

	//pipeline object properties toolbar
	void toolBar_objProp_scalarBarAct();
	void toolBar_objProp_solidColorAct();
	void toolBar_objProp_presentCmb_currentIndexChanged(int tep_index);
	void toolBar_objProp_columnCmb_currentIndexChanged(int tep_index);
	void toolBar_objProp_GlyphVector_currentIndexChanged(int tep_index);
	void toolBar_objProp_exportAct();
	//pipeline object properties toolbar
	virtual void reTranslate();
	//void update_pipelineObjList_for_comModel(DataSourcePropForm *tep_widget, QList<PipelineObject*> tep_mPipelnObjs);
	renderview_global_prop cur_renderview_global_prop_data;

	void func_update_colorComboBox_glyphVector(bool flag);
	void func_update_colorComboBox_calculator();
public slots:
	void slot_update_buildIn_propForm();

	void on_BtnAccept_clicked();  // libaojun
	//void slot_update_sliceProp_widget(double* origins_val, double* normals_val);
private slots:
	
	//SliceFilter
	void slot_update_planeOrigin(PipelineObject *);
	void slot_update_planeNormal(PipelineObject *);
	void slot_update_planeCameraNormal(PipelineObject *);
	void slot_update_planeVis(PipelineObject *);
	//SliceFilter

	//contourFilter
	//void slot_update_contourMin(PipelineObject *);
	//void slot_update_contourMax(PipelineObject *);
	//void slot_update_contourColName(PipelineObject *);
	//void slot_update_contourColName(PipelineObject *);
	//contourFilter
	void slot_setFlagRender(bool flag);
	void slot_enable_applyBtn();
	void slot_update_pipelineObj_propForm(PipelineObject *tep_pipeObj);
		
    void slot_update_pipelineObj_colorPropWidget(QString item_name,bool vis);
    void on_BtnPropertiesButton_clicked();

    void on_BtnDisplayButton_clicked();

    void on_BtnViewButton_clicked();

    void on_comboBox_backgroundType_currentIndexChanged(int index);

    void on_color1_pushButton_clicked();

    void on_color2_pushButton_clicked();

   

    void on_m_sliderOpacity_sliderMoved(int position);

    void on_m_cmb_ColumnList_currentIndexChanged(int index);

    void on_m_edt_PointSize_editingFinished();

    void on_m_edt_linewidth_editingFinished();

    void on_m_edt_tranx_editingFinished();

    void on_m_edt_trany_editingFinished();

    void on_m_edt_tranz_editingFinished();

    void on_m_edt_scalex_editingFinished();
    void on_m_edt_scaley_editingFinished();
    void on_m_edt_scalez_editingFinished();

    void on_m_edt_orientx_editingFinished();
    void on_m_edt_orienty_editingFinished();
    void on_m_edt_orientz_editingFinished();

    void on_m_edt_originx_editingFinished();
    void on_m_edt_originy_editingFinished();
    void on_m_edt_originz_editingFinished();

    void on_m_cmb_Representation_currentIndexChanged(int index);

    void on_orient_checkBox_clicked(bool checked);

    void on_m_slider_specular_sliderMoved(int position);

    void on_m_silder_specularpower_sliderMoved(int position);

    void on_m_slider_ambient_sliderMoved(int position);

    void on_m_slider_diffuse_sliderMoved(int position);

    void on_m_cmb_interpolation_currentIndexChanged(int index);

    void on_btnspecularcolor_clicked();

    void on_scalarBar_showBtn_clicked();

    void on_colorEdit_Btn_clicked();

    void on_edgeColor_pushButton_clicked();

    void on_camera_parallel_checkBox_clicked(bool checked);

    void on_cubeAxes_checkBox_clicked(bool checked);
	

    void on_vectorColorXYZ_comboBox_currentIndexChanged(int index);

	
    void on_remark_checkBox_clicked(bool checked);

    void on_fontColor_pushButton_clicked();

    void on_remark_textEdit_textChanged();

private:
    Ui::PipelineObjPropWidget *ui;
    bool flag_propBtnExpand;
    bool flag_dispBtnExpand;
    bool flag_viewBtnpExpand;
    
	PipelineObjProp *pipelineObj_propPtr;
    PipelineObject *cur_pipelineObj;
	QWidget *cur_addWidget;
	QVBoxLayout *properties_layout;
	void add_propForm(PipelineObject *tep_pipeObj);
	scalarBarEditDockWidget* tep_scalarBar_Form;
	void func_initRenderView();
signals:
	void sig_change_global_view_prop();//void sig_change_global_view_prop(renderview_global_prop);
	void sig_refreshView_fromPropWidget();
	//base property for actor
    void sig_update_opacity(PipelineObject *);
	void sig_update_color(PipelineObject *);
	void sig_update_pointSize(PipelineObject *);
	void sig_update_lineWidth(PipelineObject *);
	void sig_update_translate(PipelineObject *);
	void sig_update_scale(PipelineObject *);
	void sig_update_origin(PipelineObject *);
	void sig_update_orientation(PipelineObject *);
	void sig_update_representation(PipelineObject *);
	void sig_update_axes_visible(bool);
	void sig_update_remark_visible(bool);
	void sig_update_remark_fontColor();
	void sig_update_remark_text();
	void sig_update_camera_parallel(bool);
	void sig_update_specular(PipelineObject *);
	void sig_update_diffuse(PipelineObject *);
	void sig_update_ambient(PipelineObject *);
	void sig_update_specularPower(PipelineObject *);
	void sig_update_specularColor(PipelineObject *);
	void sig_update_solidColor(PipelineObject *);
	void sig_update_interpolation(PipelineObject *);
	void sig_update_scalarBar(PipelineObject *);
	void sig_update_edgeColor(PipelineObject *);
	//SliceFilter
	void sig_update_planeOrigin(PipelineObject *);
	void sig_update_planeNormal(PipelineObject *);
	void sig_update_planeVis(PipelineObject *);
	void sig_update_planeCameraNormal(PipelineObject *);
	//SliceFilter

	//contourFilter
	//void sig_update_contourMin(PipelineObject *);
	//void sig_update_contourMax(PipelineObject *);
	//void sig_update_contourColName(PipelineObject *);
	//void sig_update_contourColValue(PipelineObject *);
	//contourFilter

	void sig_update_filterToolBar(bool, PipelineObject*);
	void sig_update_toolBar_objProp_column(int);
	void sig_update_toolBar_objProp_column_GlyphVector(bool);
	void sig_update_toolBar_objProp_column_GlyphVectorXYZ(int);
	void sig_update_toolBar_objProp_present(int);
	void sig_update_toolBar_objProp_column_Calculator();

	void sig_load_scalarBarEditForm(PipelineObject*);

	void sig_update_cubeAxes(PipelineObject *);

	void sig_update_glyphVector(PipelineObject *);

	void sig_setFlagRender(bool);
	//PYTHON
	void sig_python_applyClicked();
	void sig_python_opacity(PipelineObject *);
	void sig_python_color(PipelineObject *);
	void sig_python_color_component(PipelineObject *);
	void sig_python_pointSize(PipelineObject *);
	void sig_python_lineWidth(PipelineObject *);
	void sig_python_translate(PipelineObject *);
	void sig_python_scale(PipelineObject *);
	void sig_python_origin(PipelineObject *);
	void sig_python_orientation(PipelineObject *);
	void sig_python_representation(PipelineObject *);
	void sig_python_specular(PipelineObject *);
	void sig_python_diffuse(PipelineObject *);
	void sig_python_ambient(PipelineObject *);
	void sig_python_specularPower(PipelineObject *);
	void sig_python_specularColor(PipelineObject *);
	void sig_python_solidColor(PipelineObject *);
	void sig_python_interpolation(PipelineObject *);
	void sig_python_scalarBar(PipelineObject *);
	void sig_python_edgeColor(PipelineObject *);
	void sig_python_cubeAxes(PipelineObject *);
	void sig_python_axesVisible(bool);

	void sig_python_view_backgroundType(int val);
	void sig_python_view_backgroundColor(QColor color_val);
	void sig_python_view_background2Color(QColor color_val);
	void sig_python_view_axesVisible(bool val);
	void sig_python_view_cameraParallel(bool val);
	void sig_python_view_interaction(bool val);
	void sig_python_renderView();
	void sig_python_view_remarkVisible(bool);
	void sig_python_view_remarkFontColor(QColor);
	void sig_python_view_remarkText(QString);

	void sig_python_vector_scalar(PipelineObject* tep_piplnObj, QString val);
	void sig_python_vector_normal(PipelineObject* tep_piplnObj, QString val);
	void sig_python_vector_numPoints(PipelineObject* tep_piplnObj, int val);
	void sig_python_vector_glyph_type(PipelineObject* tep_piplnObj, int val);
	void sig_python_vector_glyph_tipRes(PipelineObject* tep_piplnObj, int val);
	void sig_python_vector_glyph_tipRad(PipelineObject* tep_piplnObj, double val);
	void sig_python_vector_glyph_tipLen(PipelineObject* tep_piplnObj, double val);
	void sig_python_vector_glyph_shaftRes(PipelineObject* tep_piplnObj, int val);
	void sig_python_vector_glyph_shaftRad(PipelineObject* tep_piplnObj, double val);

	void sig_python_planeOrigin(PipelineObject *);
	void sig_python_planeNormal(PipelineObject *);
	void sig_python_planeVis(PipelineObject *);
	void sig_python_insideOut(PipelineObject *);

	void sig_python_contourColumn(PipelineObject* tep_piplnObj, QString val);
	void sig_python_contourValue(PipelineObject* tep_piplnObj, double val);
	void sig_python_contour_reflection(PipelineObject* tep_piplnObj,int val);
	void sig_python_contour_reflectionAxes(PipelineObject* tep_piplnObj, int val);

	void sig_python_reflectionAxes(PipelineObject*);
	void sig_python_reflectionCopyInput(PipelineObject*);

	void sig_python_smooth(PipelineObject*, int, double);

	void sig_python_streamline_vector(PipelineObject* tep_piplnObj, QString val);
	void sig_python_streamline_integration_direction(PipelineObject* tep_piplnObj, IntegrationDirection val);
	void sig_python_streamline_integration_type(PipelineObject* tep_piplnObj, IntegratorType val);
	void sig_python_streamline_integration_stepUnit(PipelineObject* tep_piplnObj, IntegrationStepUnit val);
	void sig_python_streamline_integration_initStepLen(PipelineObject* tep_piplnObj, double val);
	void sig_python_streamline_integration_miniStepLen(PipelineObject* tep_piplnObj, double val);
	void sig_python_streamline_integration_maxiStepLen(PipelineObject* tep_piplnObj, double val);
	void sig_python_streamline_stream_maxiSteps(PipelineObject* tep_piplnObj, int val);
	void sig_python_streamline_stream_maxiStreamLen(PipelineObject* tep_piplnObj, double val);
	void sig_python_streamline_stream_terminalSpeed(PipelineObject* tep_piplnObj, double val);
	void sig_python_streamline_stream_maxiError(PipelineObject* tep_piplnObj, double val);
	void sig_python_streamline_seeds_type(PipelineObject* tep_piplnObj, SpeedType val);
	void sig_python_streamline_seeds_mPoint(PipelineObject* tep_piplnObj, double* val);
	void sig_python_streamline_seeds_num_points(PipelineObject* tep_piplnObj, int val);
	void sig_python_streamline_seeds_radius(PipelineObject* tep_piplnObj, double val);
	void sig_python_streamline_vorticity(PipelineObject* tep_piplnObj, bool val);
	void sig_python_streamline_interpolatorType(PipelineObject* tep_piplnObj, InterpolatorType val);
	void sig_python_streamline_surface_streamLines(PipelineObject* tep_piplnObj, bool val);
	void sig_python_streamline_reflection(PipelineObject* tep_piplnObj, int val);
	void sig_python_streamline_reflectionAxes(PipelineObject* tep_piplnObj, int val);
	//PYTHON
};

#endif // PIPELINEOBJPROPWIDGET_H
