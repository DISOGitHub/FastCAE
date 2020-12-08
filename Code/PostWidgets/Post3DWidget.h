///************************
///三维后处理窗口
///*************************
#ifndef _POST3DWINDOW_H_
#define _POST3DWINDOW_H_

#include "PostWidgetsAPI.h"
#include "moduleBase/graphWindowBase.h"
#include <QWidget>
#include <QList>
#include <QColor>

class graph_MainWindow;
class PipelineObject;
class QMdiSubWindow;
class QToolBar;
class QDockWidget;
class vtkDataSet;

namespace GUI
{
	class MainWindow;
}
namespace Ui
{
	class Post3DWidget;
}

namespace Post
{
	class POSTAPI Post3DWindow  : /*public QWidget, */public ModuleBase::GraphWindowBase
	{
		Q_OBJECT
	public:
		Post3DWindow(GUI::MainWindow* mainwindow, int proID);
		~Post3DWindow();
	 
		
	public slots:
		///打开文件
		void openFile(QString files ,bool apply = true);
		//应用
		void applyClick();
		//开始动画
		bool startAnimate();
		//停止动画
		bool stopAnimate();
		//获取树的widget
		QWidget* getTreeWidget();
		//更新pipeline Object
		void updatePipelineObjeDataSet(PipelineObject* obj, QString fileName);
		//获取当前Pipeline object
		PipelineObject* getCurrentPipelineObj();
		///获取树 Prop widget
		QWidget* getPropWidget();
		///获取菜单栏
		QList<QToolBar*> getToolBars();
		///清空所有绘图
		void clearAll();
		///设置背景色
		void setBackGround(QColor color1, QColor color2) override;
		///保存图片
		void saveImage(QString fileName, int width, int heigh, bool showDlg) override;
		//获取ScalarBar范围
		void getScalarRange(PipelineObject* obj, double value[2]);
		//设置ScalarBar范围
		void reScaleScalarRange(PipelineObject* obj, double min, double max);
		//显示颜色条
		void displayLegendBar(bool on);
		//展示云图
		void viewCounter(PipelineObject* obj, QString var);
		//展示矢量图
		void viewVector(PipelineObject* obj, QString var);
		//语言切换
		void reTranslate() override;
		//获取节点数据
		vtkDataSet* getPipelineObjDataSet(PipelineObject* obj);

		void showAnimationToolBar(bool show);
	
		///<MG 获取节点数据个数
		int getNumOfGeometry();

		///<MG 根据index获取数据节点
		PipelineObject * getGeometryByIndex(int index);
		//
		void Properties_Opacity(int obj_id, double mOpacity);
		//
		void Properties_colorColumn(int obj_id, QString mColorColumnStyle);
		//
		void Properties_scalarBarTitle(int obj_id, QString scolName, QString sm_titile);
		//
		void Properties_scalarBarFontSize(int obj_id, QString scolName, int m_fontSize);
		//
		void Properties_scalarBarNumLables(int obj_id, QString scolName, int m_numLables);
		//
		void Properties_lineWidth(int obj_id, int mLineWidth);
		//
		void Properties_pointSize(int obj_id, int mPointSize);
		//
		void Properties_translate(int obj_id, double x, double y, double z);
		//
		void Properties_origin(int obj_id, double x, double y, double z);
		//
		void Properties_scale(int obj_id, double x, double y, double z);
		//
		void Properties_orientation(int obj_id, double x, double y, double z);
		//
		void Properties_representation(int obj_id, int m_enum_representationtype);
		//
		void Properties_specular(int obj_id, double mSpecular);
		//////////////////////
		void Properties_diffuse(int obj_id, double mDiffuse);
		//
		void Properties_ambient(int obj_id, double mAmbient);
		//
		void Properties_specularPower(int obj_id, int mSpecularPower);
		//
		void Properties_specularColor(int obj_id, int r, int g, int b);
		//
		void Properties_solidColor(int obj_id, int r, int g, int b);
		//
		void Properties_edgeColor(int obj_id, int r, int g, int b);
		//
		void Properties_interpolation(int obj_id, int m_enum_interpolationtype);
		//
		void Properties_Flag_scalarBar(int obj_id, QString smColorColumnStyle);
		//
		void Properties_EnableOpacityMap(int obj_id, bool val);
		//
		void Properties_visible(int obj_id, bool flag_show_actors);
		//
		void Properties_show_scalarBars(int obj_id, bool mScalarBarVisible);
		//
		void Properties_show_cubeAxes(int obj_id, bool flag_cubeAxes);
		//
		void Properties_scalarBarPosition(int obj_id, QString scolName, int tep_orietation, double pos0, double pos1, double pos2, double pos3);
		//
		void FilterClip(int);
		//
		void FilterSlice(int);
		//
		void FilterContour(int);
		//
		void FilterVector(int);
		//
		void FilterReflection(int);
		//
		void FilterSmooth(int);
		//
		void FilterStreamLine(int);
		//
		void Properties_vector_GlyphVector(int, QString);
		//
		void Properties_vector_scalar(int, QString);
		//
		void Properties_vector_normal(int,QString);
		//
		void Properties_vector_numPoints(int, int);
		//
		void Properties_vector_glyph_type(int, int);
		//
		void Properties_vector_glyph_tipRes(int, int);
		//
		void Properties_vector_glyph_tipRad(int, double);
		//
		void Properties_vector_glyph_tipLen(int, double);
		//
		void Properties_vector_glyph_shaftRes(int, int);
		//
		void Properties_vector_glyph_shaftRad(int, double);
		//////////////
		void Properties_view_backgroundTyped(int);
		//
		void Properties_view_backgroundColor(int, int, int);
		//
		void Properties_view_background2Color(int, int, int);
		//
		void Properties_view_axesVisible(int);
		//
		void Properties_view_cameraParallel(int);
		//
		void Properties_view_interaction(int);
		//
		void Properties_renderView();
		//
		void Camera_Position(double, double, double);
		//
		void Camera_FocalPoint(double, double, double);
		//
		void Camera_ClippingRange(double, double);
		//
		void Camera_ViewUp(double, double, double);
		//
		void Camera_ViewAngle(double);
		//
		void Camera_Zoom(double);
		//
		void Camera_Reset();
		//
		void Properties_planeOrigin(int, double, double, double);
		//
		void Properties_planeNormal(int,  double, double, double);
		//
		void Properties_planeVisible(int, int);
		//
		void Properties_insideOut(int, int);
		//
		void Properties_contourColumn(int, QString);
		//
		void Properties_contourValue(int, double);
		//
		void Properties_contour_reflection(int, int);
		//
		void Properties_contour_reflectionAxes(int, int);
		//
		void Properties_reflectionAxes(int, int);
		//
		void Properties_smooth(int, int, double);
		////////////
		void Properties_streamline_vector(int, QString);
		//
		void Properties_streamline_integration_direction(int, int);
		//
		void Properties_streamline_integration_type(int, int);
		//
		void Properties_streamline_integration_stepUnit(int, int);
		//
		void Properties_streamline_integration_initStepLen(int, double);
		//
		void Properties_streamline_integration_miniStepLen(int, double);
		//
		void Properties_streamline_integration_maxiStepLen(int, double);
		//
		void Properties_streamline_stream_maxiSteps(int, int);
		//
		void Properties_streamline_stream_maxiStreamLen(int, double);
		//
		void Properties_streamline_stream_terminalSpeed(int, double);
		//
		void Properties_streamline_stream_maxiError(int, double);
		//
		void Properties_streamline_seeds_type(int, int);
		//
		void Properties_streamline_seeds_mPoint(int, double, double, double);
		//
		void Properties_streamline_seeds_num_points(int, int);
		//
		void Properties_streamline_seeds_radius(int, double);
		//
		void Properties_streamline_vorticity(int, bool);
		//
		void Properties_streamline_interpolatorType(int, int);
		//
		void Properties_streamline_surface_streamLines(int, bool);
		//
		void Properties_streamline_reflection(int, bool);
		//
		void Properties_streamline_reflectionAxes(int, int);
	signals:
		//动画当前帧，index从0开始
		void animateFrameSig(int index);
		//发送三维拾取曲线窗口
		void createPickCurveWidgetSig(QWidget* w); 
		//AVI动画保存完成信号
		void AVIFileFinishedSig(QString fileName);
		//刷新三维渲染区域
		void refreshViewFromMainWindowSig(QList<PipelineObject*>);
		//打开文件
		void openFileSig(QString f, bool apply);
		//apply
		void applyClickSig();
		//
		void Properties_OpacitySig(int, double );
		//
		void Properties_colorColumnSig(int, QString );
		//
		void Properties_scalarBarTitleSig(int, QString, QString);
		//  
		void Properties_scalarBarFontSizeSig(int, QString, int);
		//
		void Properties_scalarBarNumLablesSig(int, QString, int);
		// 
		void Properties_lineWidthSig(int, int);
		//
		void Properties_pointSizeSig(int, int);
		//
		void Properties_translateSig(int, double, double, double);
		//
		void Properties_originSig(int, double, double, double);
		//
		void Properties_scaleSig(int, double, double, double);
		//
		void Properties_orientationSig(int, double, double, double);
		//
		void Properties_representationSig(int, int);
		//
		void  Properties_specularSig(int, double);
		///////////
		void Properties_diffuseSig(int, double);
		//
		void Properties_ambientSig(int, double);
		//
		void Properties_specularPowerSig(int, int);
		//
		void Properties_specularColorSig(int, int, int, int);
		//
		void Properties_solidColorSig(int, int, int, int);
		//
		void Properties_edgeColorSig(int, int, int, int);
		//
		void Properties_interpolationSig(int, int);
		//
		void Properties_Flag_scalarBarSig(int, QString);
		//
		void Properties_EnableOpacityMapSig(int, bool);
		//
		void Properties_visibleSig(int, bool);
		//
		void Properties_show_scalarBarsSig(int, bool);
		//
		void Properties_show_cubeAxesSig(int, bool);
		//
		void Properties_scalarBarPositionSig(int, QString, int, double, double, double, double);
		//
		void FilterClipSig(int);
		//
		void FilterSliceSig(int);
		//
		void FilterContourSig(int);
		//
		void FilterVectorSig(int);
		//
		void FilterReflectionSig(int);
		//
		void FilterSmoothSig(int);
		//
		void FilterStreamLineSig(int);
		//
		void Properties_vector_GlyphVectorSig(int, QString);
		//
		void Properties_vector_scalarSig(int, QString);
		//
		void Properties_vector_normalSig(int, QString);
		//
		void Properties_vector_numPointsSig(int, int);
		//
		void Properties_vector_glyph_typeSig(int, int);
		//
		void Properties_vector_glyph_tipResSig(int, int);
		//
		void Properties_vector_glyph_tipRadSig(int, double);
		//
		void Properties_vector_glyph_tipLenSig(int, double);
		//
		void Properties_vector_glyph_shaftResSig(int, int);
		//
		void Properties_vector_glyph_shaftRadSig(int, double);
		////////
		void Properties_view_backgroundTypedSig(int);
		//
		void Properties_view_backgroundColorSig(int, int, int);
		//
		void Properties_view_background2ColorSig(int, int, int);
		//
		void Properties_view_axesVisibleSig(int);
		//
		void Properties_view_cameraParallelSig(int);
		//
		void Properties_view_interactionSig(int);
		//
		void Properties_renderViewSig();
		//
		void Camera_PositionSig(double, double, double);
		//
		void Camera_FocalPointSig(double, double, double);
		//
		void Camera_ClippingRangeSig(double, double);
		//
		void Camera_ViewUpSig(double, double, double);
		//
		void Camera_ViewAngleSig(double);
		//
		void Camera_ZoomSig(double);
		//
		void Camera_ResetSig();
		//
		void Properties_planeOriginSig(int, double, double, double);
		//
		void Properties_planeNormalSig(int,  double, double, double);
		//
		void Properties_planeVisibleSig(int, int);
		//
		void Properties_insideOutSig(int, int);
		//
		void Properties_contourColumnSig(int, QString);
		//
		void Properties_contourValueSig(int, double);
		//
		void Properties_contour_reflectionSig(int, int);
		//
		void Properties_contour_reflectionAxesSig(int, int);
		//
		void Properties_reflectionAxesSig(int, int);
		//
		void Properties_smoothSig(int, int, double);
		//
		void Properties_streamline_vectorSig(int, QString);
		//
		void Properties_streamline_integration_directionSig(int, int);
		//
		void Properties_streamline_integration_typeSig(int, int);
		//
		void Properties_streamline_integration_stepUnitSig(int, int);
		//
		void Properties_streamline_integration_initStepLenSig(int, double);
		//
		void Properties_streamline_integration_miniStepLenSig(int, double);
		//
		void Properties_streamline_integration_maxiStepLenSig(int, double);
		//
		void Properties_streamline_stream_maxiStepsSig(int, int);
		//
		void Properties_streamline_stream_maxiStreamLenSig(int, double);
		//
		void Properties_streamline_stream_terminalSpeedSig(int, double);
		//
		void Properties_streamline_stream_maxiErrorSig(int, double);
		//
		void Properties_streamline_seeds_typeSig(int, int);
		//
		void Properties_streamline_seeds_mPointSig(int, double, double, double);
		//
		void Properties_streamline_seeds_num_pointsSig(int, int);
		//
		void Properties_streamline_seeds_radiusSig(int, double);
		//
		void Properties_streamline_vorticitySig(int, bool);
		//
		void Properties_streamline_interpolatorTypeSig(int, int);
		//
		void Properties_streamline_surface_streamLinesSig(int, bool);
		//
		void Properties_streamline_reflectionSig(int, bool);
		//
		void Properties_streamline_reflectionAxesSig(int, int);


	private:
		void fitView() override;
		void setViewXPlus() override;
		void setViewXMiuns() override;
		void setViewYPlus() override;
		void setViewYMiuns() override;
		void setViewZPlus() override;
		void setViewZMiuns() override;
		void setViewValue(int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3) override;//liu

	private slots:
		//处理拾取曲线窗口，删除头
		void handlePickCurveWodget(QDockWidget* w);
		//添加颜色编辑框
		void createScalarBarEditWidget(QDockWidget *w);
		//处理发送的脚本
		void handleScriptCode(QString code);

	private:
		Ui::Post3DWidget* _ui{};

		graph_MainWindow* _graphWindow{};
//		QList<QToolBar*> _toolBars{};
	};

}


#endif